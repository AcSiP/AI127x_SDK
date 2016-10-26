
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
  * @file    Project/ARM-Lora/led.c
  * @author  JC
  * @version V1.0.0
  * @date    17-May-2016
  * @brief   led program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "sx1276-Hal.h"
#include "led.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Const variables -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***************************************************************************************************
 *  Function Name: Led_PinInitialization
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	Led_PinInitialization(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

#ifdef STM32F401xx
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE );
#elif STM32F071 || STM32F072
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE );
#endif

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_WriteBit( GPIOA, GPIO_Pin_7, Bit_RESET );
	GPIO_WriteBit( GPIOB, GPIO_Pin_2, Bit_RESET );
}

/***************************************************************************************************
 *  Function Name: Led_BootFlashLed
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	Led_BootFlashLed(void)
{
	uint8_t		count;
	uint32_t	delay;

	for(count = 0 ; count < 3 ; count++) {
		GPIO_WriteBit( GPIOA, GPIO_Pin_7, Bit_SET );
		GPIO_WriteBit( GPIOB, GPIO_Pin_2, Bit_SET );

		delay = GET_TICK_COUNT( );
		while( ( GET_TICK_COUNT( ) - delay ) < TICK_RATE_MS( 300 ) ) {
		}

		if(count >= 2) break;

		GPIO_WriteBit( GPIOA, GPIO_Pin_7, Bit_RESET );
		GPIO_WriteBit( GPIOB, GPIO_Pin_2, Bit_RESET );

		delay = GET_TICK_COUNT( );
		while( ( GET_TICK_COUNT( ) - delay ) < TICK_RATE_MS( 300 ) ) {
		}
	}

	GPIO_WriteBit( GPIOA, GPIO_Pin_7, Bit_RESET );
	GPIO_WriteBit( GPIOB, GPIO_Pin_2, Bit_RESET );
}


/***************************************************************************************************
 *  Function Name: Led_GreenLedLightOn
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	Led_GreenLedLightOn(void)
{
	GPIO_WriteBit( GPIOA, GPIO_Pin_7, Bit_SET );
}


/***************************************************************************************************
 *  Function Name: Led_GreenLedLightOff
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	Led_GreenLedLightOff(void)
{
	GPIO_WriteBit( GPIOA, GPIO_Pin_7, Bit_RESET );
}


/***************************************************************************************************
 *  Function Name: Led_BlueLedLightOn
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	Led_BlueLedLightOn(void)
{
	GPIO_WriteBit( GPIOB, GPIO_Pin_2, Bit_SET );
}


/***************************************************************************************************
 *  Function Name: Led_BlueLedLightOff
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	Led_BlueLedLightOff(void)
{
	GPIO_WriteBit( GPIOB, GPIO_Pin_2, Bit_RESET );
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
