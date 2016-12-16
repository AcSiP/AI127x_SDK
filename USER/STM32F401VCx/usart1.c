
//---------------------------------------------------------------------------
/*
//==========================================
// Author : JC<jc@acsip.com.tw>
// Copyright 2016(C) AcSiP Technology Inc.
// 版權所有：群登科技股份有限公司
// http://www.acsip.com.tw
//==========================================
*/
//---------------------------------------------------------------------------

/**
  ******************************************************************************
  * @file    Project/ARM-Lora/usart1.c
  * @author  JC
  * @version V1.0.0
  * @date    12-Apr-2016
  * @brief   USART1 program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>
#include "config.h"
#include "usart1.h"
#include "stm32f4xx.h"

#ifdef Board__A22_Tracker

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t		GPS_USART_count;
__IO uint8_t		GPS_USART_RX_Buf1[USART1_RX_BufLength];
__IO uint8_t		GPS_USART_RX_Buf2[USART1_RX_BufLength];
__IO uint8_t *		GPS_USART_RX_BufPointer;
__IO bool		isGPS_USART_RX_Buf1Full;
__IO bool		isGPS_USART_RX_Buf2Full;
__IO bool *		isGPS_USART_RX_BOOL;
static __IO uint8_t	TailNum0, TailNum1;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***************************************************************************************************
 *  Function Name: USART1_PinInitialization
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	USART1_PinInitialization(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	USART_DeInit(USART1);

	/* Pin configuration */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Alternate function conf */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	/* Enable the USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}


/***************************************************************************************************
 *  Function Name: USART1_UartConfig
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	USART1_UartConfig(uint32_t BaudRate)
{
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;

	/* USART conf */
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


/***************************************************************************************************
 *  Function Name: USART1_UartEnableOrDisable
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	USART1_UartEnableOrDisable(FunctionalState NewState)
{
	/* Enable/disable the USART */
	USART_Cmd(USART1, NewState);
	USART1_EnvironmentVariableSet();
}


/***************************************************************************************************
 *  Function Name: USART1_UartInit
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	USART1_UartInit(uint32_t BaudRate)
{
	USART1_PinInitialization();
	USART1_UartConfig(BaudRate);
	USART1_EnvironmentVariableSet();
}


/***************************************************************************************************
 *  Function Name: USART1_UartWrite
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	USART1_UartWrite(uint8_t *Str, uint32_t Length)
{
	uint32_t	count;

	for(count = 0 ; count < Length ; count++) {
		while ( USART_GetFlagStatus( USART1, USART_FLAG_TXE ) == RESET ) {
		}
		USART_SendData( USART1, Str[count] );
	}
}


/***************************************************************************************************
 *  Function Name: USART1_IRQHandler
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		/* Read one byte from the receive data register */
		GPS_USART_RX_BufPointer[GPS_USART_count] = USART_ReceiveData(USART1);
		/* Clear the USART1 Receive interrupt */
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		TailNum0 = TailNum1;
		TailNum1 = GPS_USART_RX_BufPointer[GPS_USART_count];
		GPS_USART_count++;

		// 判斷接收與長度
		if( (TailNum0 == 0x0D) && (TailNum1 == 0x0A) ) {
			*isGPS_USART_RX_BOOL = true;
			GPS_USART_RX_BufPointer[GPS_USART_count - 2] = 0x00;
			GPS_USART_RX_BufPointer[GPS_USART_count - 1] = 0x00;
			USART1_VariableSwap();
			GPS_USART_count = 0;
			// 傳出已完整接收,切換,傳出長度(傳出的長度為有效長度,需扣掉辨識碼長度),接收的資料去除後面一碼辨識碼,長度清為0
		} else {
			if(GPS_USART_count >= USART1_RX_BufLength) USART1_EnvironmentVariableSet();
		}
	}
}


/***************************************************************************************************
 *  Function Name: USART1_EnvironmentVariableSet
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	USART1_EnvironmentVariableSet(void)
{
	memset((void *)GPS_USART_RX_Buf1, 0, USART1_RX_BufLength);
	memset((void *)GPS_USART_RX_Buf2, 0, USART1_RX_BufLength);

	GPS_USART_RX_BufPointer = GPS_USART_RX_Buf1;
	isGPS_USART_RX_Buf1Full = false;
	isGPS_USART_RX_Buf2Full = false;
	isGPS_USART_RX_BOOL = &isGPS_USART_RX_Buf1Full;
	GPS_USART_count = 0;
}


/***************************************************************************************************
 *  Function Name: USART1_VariableSwap
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	USART1_VariableSwap(void)
{
	if((isGPS_USART_RX_Buf1Full == true) && (isGPS_USART_RX_Buf2Full == false)) {
		GPS_USART_RX_BufPointer = GPS_USART_RX_Buf2;
		isGPS_USART_RX_BOOL = &isGPS_USART_RX_Buf2Full;
	} else {
		if((isGPS_USART_RX_Buf1Full == false) && (isGPS_USART_RX_Buf2Full == true)) {
			GPS_USART_RX_BufPointer = GPS_USART_RX_Buf1;
			isGPS_USART_RX_BOOL = &isGPS_USART_RX_Buf1Full;
		} else {
			USART1_EnvironmentVariableSet();
		}
	}
}

#endif

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
