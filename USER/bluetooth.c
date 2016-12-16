
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

#ifdef Board__A22_Tracker

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "sx1276-Hal.h"
#include "bluetooth.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Const variables -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern bool		EnableMaster;		// Master/Slave selection
extern __IO uint32_t	SystemOperMode;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***************************************************************************************************
 *  Function Name: BlueTooth_PinInitialization
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	BlueTooth_PinInitialization(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

#ifdef STM32F401xx
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
#elif STM32F071 || STM32F072
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
#endif

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_WriteBit( GPIOA, GPIO_Pin_4, Bit_RESET );
	GPIO_WriteBit( GPIOA, GPIO_Pin_5, Bit_RESET );
}


/***************************************************************************************************
 *  Function Name: BlueTooth_DA14580Reset
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	BlueTooth_DA14580Reset(void)
{
	uint32_t	count = 0;

	if((SystemOperMode == SystemInNormal) && (EnableMaster == false)) return;

	GPIO_WriteBit( GPIOA, GPIO_Pin_5, Bit_SET );

	/*
	do{
		asm("NOP");
		count++;
	} while ( count < 200000 );
	*/
	count = GET_TICK_COUNT( );
	while( ( GET_TICK_COUNT( ) - count ) < TICK_RATE_MS( 100 ) ) {
	}
	GPIO_WriteBit( GPIOA, GPIO_Pin_5, Bit_RESET );
}


/***************************************************************************************************
 *  Function Name: BlueTooth_DA14580Enable
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	BlueTooth_DA14580Enable(void)
{
	uint32_t	count = 0;

	if((SystemOperMode == SystemInNormal) && (EnableMaster == false)) return;

	GPIO_WriteBit( GPIOA, GPIO_Pin_4, Bit_SET );

	/*
	do{
		asm("NOP");
		count++;
	} while ( count < 200000 );
	*/
	count = GET_TICK_COUNT( );
	while( ( GET_TICK_COUNT( ) - count ) < TICK_RATE_MS( 200 ) ) {
	}
}


/***************************************************************************************************
 *  Function Name: BlueTooth_DA14580Disable
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	BlueTooth_DA14580Disable(void)
{
	GPIO_WriteBit( GPIOA, GPIO_Pin_4, Bit_RESET );
	Led_GreenLedLightOff();
}


/***************************************************************************************************
 *  Function Name: BlueTooth_DA14580Run
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	BlueTooth_DA14580Run(uint32_t BaudRate)
{
	uint32_t	value = 0, total = 0;

	BlueTooth_DA14580Stop();
	if((SystemOperMode == SystemInNormal) && (EnableMaster == false)) {
		Led_GreenLedLightOff();
		return;
	}

	Led_GreenLedLightOn();

	// delay 500ms (25 * 20 = 500ms)
	do {
		value = GET_TICK_COUNT( );
		while( ( GET_TICK_COUNT( ) - value ) < TICK_RATE_MS( 20 ) ) {
		}
		total++;
	} while ( total < 25 );

	BlueTooth_PinInitialization();
	BlueTooth_DA14580Enable();
	BlueTooth_DA14580Reset();

	// delay 1s (50 * 20 = 1000ms)
	total = 0;
	do {
		value = GET_TICK_COUNT( );
		while( ( GET_TICK_COUNT( ) - value ) < TICK_RATE_MS( 20 ) ) {
		}
		total++;
	} while ( total < 50 );

	CmdUART_UartInit(BaudRate);
	CmdTIMER_TimerConfig();
}


/***************************************************************************************************
 *  Function Name: BlueTooth_DA14580Stop
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	BlueTooth_DA14580Stop(void)
{
	CmdUART_UartEnableOrDisable(DISABLE);
	CmdTIMER_TimerRunOrStop(DISABLE);
	USART_DeInit( USART2 );
	BlueTooth_DA14580Disable();
}

#endif		// #Board__A22_Tracker

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
