/**
  ******************************************************************************
  * @file    Project/ARM-Lora/usart2.c 
  * @author  JC
  * @version V1.0.0
  * @date    10-Mar-2016
  * @brief   USART2 program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>
#include "config.h"
#include "usart2.h"
#include "tim4.h"
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t USART2_count;
__IO uint16_t USART2_RX_Length;
__IO uint8_t RX_Buf1[USART2_RX_BufLength];
__IO uint8_t RX_Buf2[USART2_RX_BufLength];
__IO uint8_t *RX_BufPointer;
__IO uint8_t digital0, digital1, digital2, digital3, digital4, digital5, digital6, digital7;
__IO bool isRX_Buf1Full;
__IO bool isRX_Buf2Full;
__IO bool *isRX_BOOL;
__IO bool USART2_EchoOn = false;
static bool USART2_ON = false;
extern __IO FunctionalState Timer4_Status;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/***************************************************************************************************
 *  Function Name: USART2_PinInitialization
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void USART2_PinInitialization(void) {
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	USART_DeInit(USART2);
	
  /* Pin configuration */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Alternate function conf */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	/* Enable the USART2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
}



/***************************************************************************************************
 *  Function Name: USART2_UartConfig
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void USART2_UartConfig(uint32_t BaudRate) {
	
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  /* USART conf */
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
	
	/* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* Enable USART */
  USART_Cmd(USART2, ENABLE);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
}



/***************************************************************************************************
 *  Function Name: USART2_UartEnableOrDisable
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void USART2_UartEnableOrDisable(FunctionalState NewState) {
	
  /* Enable/disable the USART */
  USART_Cmd(USART2, NewState);
  
  if(NewState == ENABLE) {
    USART2_ON = true;
  } else {
    USART2_ON = false;
  }
	
}



/***************************************************************************************************
 *  Function Name: USART2_UartInit
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void USART2_UartInit(uint32_t BaudRate) {
  
  USART2_ON = true;
	
	USART2_PinInitialization();
	USART2_UartConfig(BaudRate);
	
	USART2_EnvironmentVariableSet();
	
}



/***************************************************************************************************
 *  Function Name: USART2_UartWrite
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void USART2_UartWrite(uint8_t *Str, uint32_t Length) {
	
	uint32_t count;
  
  if(USART2_ON == true) {
	  for(count = 0 ; count < Length ; count++) {
		  while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
		  USART_SendData(USART2, Str[count]); 
	  }
  }
  
}



/***************************************************************************************************
 *  Function Name: USART2_IRQHandler
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void USART2_IRQHandler(void) {
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		
		/* Read one byte from the receive data register */
		RX_BufPointer[USART2_count] = USART_ReceiveData(USART2);
		/* Clear the USART2 Receive interrupt */
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		
#if USART2_EchoStatus
		if(USART2_EchoOn == true) {
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
			USART_SendData(USART2, RX_BufPointer[USART2_count]); 
		}
#endif
		
		if(Timer4_Status == DISABLE) {
			TIM4_TimerRunOrStop(ENABLE);
		} else {
			TIM4_ClearDelayCounter();
		}
	  
#if USART2_JudgmentSuffixNotEnter
		digital0 = digital1;
		digital1 = digital2;
		digital2 = digital3;
		digital3 = digital4;
		digital4 = digital5;
		digital5 = digital6;
		digital6 = digital7;
		digital7 = RX_BufPointer[USART2_count];
		
		USART2_count++;
		
		//判斷接收與長度
		if( ((digital0 == 'i') && (digital1 == 's')) && ((digital2 == 'O') && (digital3 == 'v')) && ((digital4 == 'E') && (digital5 == 'r')) && ((digital6 == '*') && (digital7 == 0x0D)) ) {
		  *isRX_BOOL = true;
		  USART2_RX_Length = USART2_count - 8;
		  RX_BufPointer[USART2_count - 1] = 0x00;
		  RX_BufPointer[USART2_count - 2] = 0x00;
		  RX_BufPointer[USART2_count - 3] = 0x00;
		  RX_BufPointer[USART2_count - 4] = 0x00;
		  RX_BufPointer[USART2_count - 5] = 0x00;
		  RX_BufPointer[USART2_count - 6] = 0x00;
		  RX_BufPointer[USART2_count - 7] = 0x00;
		  RX_BufPointer[USART2_count - 8] = 0x00;
		  USART2_VariableSwap();
		  USART2_DigitalClear();
		  USART2_count = 0;
		  TIM4_TimerRunOrStop(DISABLE);
		  //傳出已完整接收,切換,傳出長度(傳出的長度為有效長度,需扣掉辨識碼長度),接收的資料去除後面八碼辨識碼,長度清為0
#else
		digital7 = RX_BufPointer[USART2_count];
		USART2_count++;
		//判斷接收與長度
		if( digital7 == 0x0D ) {
			*isRX_BOOL = true;
			USART2_RX_Length = USART2_count - 1;
			RX_BufPointer[USART2_count - 1] = 0x00;
			USART2_VariableSwap();
			USART2_DigitalClear();
			USART2_count = 0;
			TIM4_TimerRunOrStop(DISABLE);
			//傳出已完整接收,切換,傳出長度(傳出的長度為有效長度,需扣掉辨識碼長度),接收的資料去除後面一碼辨識碼,長度清為0
#endif
		} else if(USART2_count >= USART2_RX_BufLength) {
			USART2_EnvironmentVariableSet();
			TIM4_TimerRunOrStop(DISABLE);
			USART2_UartWrite((uint8_t *)unkownCMD, strlen(unkownCMD));
		}
		
  }
	
}



/***************************************************************************************************
 *  Function Name: USART2_EnvironmentVariableSet
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void USART2_EnvironmentVariableSet(void) {
	
	memset((void *)RX_Buf1, 0, USART2_RX_BufLength);
	memset((void *)RX_Buf2, 0, USART2_RX_BufLength);
	
	RX_BufPointer = RX_Buf1;
	
	isRX_Buf1Full = false;
  isRX_Buf2Full = false;
  isRX_BOOL = &isRX_Buf1Full;
	
	USART2_count = 0;
	
	USART2_DigitalClear();
	
}



/***************************************************************************************************
 *  Function Name: USART2_VariableSwap
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void USART2_VariableSwap(void) {
	
	if((isRX_Buf1Full == true) && (isRX_Buf2Full == false)) {
		RX_BufPointer = RX_Buf2;
		isRX_BOOL = &isRX_Buf2Full;
	} else if((isRX_Buf1Full == false) && (isRX_Buf2Full == true)){
		RX_BufPointer = RX_Buf1;
		isRX_BOOL = &isRX_Buf1Full;
	} else {
		USART2_EnvironmentVariableSet();
	}
	
}



/***************************************************************************************************
 *  Function Name: USART2_DitialClear
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void USART2_DigitalClear(void) {
	
	digital0 = 0;
	digital1 = 0;
	digital2 = 0;
	digital3 = 0;
	digital4 = 0;
	digital5 = 0;
	digital6 = 0;
	digital7 = 0;
	
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


