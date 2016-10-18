/**
  ******************************************************************************
  * @file    Project/ARM-Lora/tim14.c 
  * @author  JC
  * @version V1.0.0
  * @date    29-Jan-2016
  * @brief   TIM14 program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

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


/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "config.h"
#include "tim14.h"
#include "usart2.h"

#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint16_t		CCR1_Val = 1000;
__IO uint16_t		capture = 0;
__IO uint16_t		CLI_DelayCounter;
__IO FunctionalState	Timer14_Status;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/***************************************************************************************************
 *  Function Name: TIM14_TimerConfig
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM14_TimerConfig( void )
{
	__IO uint16_t		PrescalerValue = 0;

	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;

	/* TIM14 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	/* --------------------------NVIC Configuration -------------------------------*/
	/* Enable the TIM14 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* ------------------- TIM14 Configuration:Output Compare Timing Mode ---------*/
	/* TIM14 Configuration: 
		The objective is to get TIM14 counter clock at 1 KHz:
		  - Prescaler = (TIM14CLK / TIM14 counter clock) - 1
		 And generate delays:
		 TIM14_CH1 delay = CCR1_Val/TIM14 counter clock = 1000 ms
	*/

	PrescalerValue = (uint16_t) (SystemCoreClock / 1000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Disable);

	Timer14_Status = DISABLE;
}



/***************************************************************************************************
 *  Function Name: TIM14_TimerRunOrStop
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM14_TimerRunOrStop( FunctionalState NewState )
{
	TIM_ITConfig( TIM14, TIM_IT_CC1, NewState );
	TIM_Cmd( TIM14, NewState );
	Timer14_Status = NewState;
	CLI_DelayCounter = 0;

	if( NewState == DISABLE ){
		TIM_ClearITPendingBit(TIM14, TIM_IT_CC1);
		capture = TIM_GetCapture1(TIM14);
		TIM_SetCompare1(TIM14, capture + CCR1_Val);
	}
}



/***************************************************************************************************
 *  Function Name: TIM14_ClearDelayCounter
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM14_ClearDelayCounter( void )
{
	CLI_DelayCounter = 0;
}



/***************************************************************************************************
 *  Function Name: TIM14_IRQHandler
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM14_IRQHandler( void )
{

#if 1
	if (TIM_GetITStatus(TIM14, TIM_IT_CC1) != RESET) {

		/* Clear TIM14 update interrupt pending bit*/
		TIM_ClearITPendingBit( TIM14, TIM_IT_CC1 );

		capture = TIM_GetCapture1( TIM14 );
		TIM_SetCompare1( TIM14, capture + CCR1_Val );

		CLI_DelayCounter++;
		if( CLI_DelayCounter >= TIM14CCR1_DelayForCMD ){
			TIM14_TimerRunOrStop(DISABLE);
			USART2_EnvironmentVariableSet();
			USART2_UartWrite( (uint8_t *)unkownCMD, strlen(unkownCMD) );
		}
	}

#else	//for test

	if( TIM_GetITStatus(TIM14, TIM_IT_CC1) != RESET ){
		/* Clear TIM14 update interrupt pending bit*/
		TIM_ClearITPendingBit( TIM14, TIM_IT_CC1 );
		GPIO_WriteBit( GPIOB, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_2)) );
	}

#endif

}



/************************ Copyright 2016(C) AcSiP Technology Inc. ******************END OF FILE****/


