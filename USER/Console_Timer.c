
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
  * @file    Project/ARM-Lora/tim4.c 
  * @author  JC
  * @version V1.0.0
  * @date    10-Mar-2016
  * @brief   CONSOLE_TIMER program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "config.h"
#include "Console_Timer.h"
#include "UART_Console.h"

#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef STM32F072
	#define		CONSOLE_TIMER			TIM14
	#define		CONSOLE_TIMER__IRQ_Handler	TIM14_IRQHandler
	#define		CONSOLE_TIMER__SYSCLK_Divder	1
#endif

#ifdef STM32F401xx
	#define		CONSOLE_TIMER			TIM4
	#define		CONSOLE_TIMER__IRQ_Handler	TIM4_IRQHandler
	#define		CONSOLE_TIMER__SYSCLK_Divder	2
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#ifdef STM32F072
	const uint16_t		CCR1_Val = 1000;
#endif

#ifdef STM32F401xx
	const uint16_t		CCR1_Val = 2000;
#endif

__IO uint16_t		capture = 0;
__IO uint16_t		CLI_DelayCounter;
__IO FunctionalState	Console_Timer_Status;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***************************************************************************************************
 *  Function Name: CmdTIMER_TimerConfig
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	CmdTIMER_TimerConfig( void )
{
	__IO uint16_t	PrescalerValue = 0;

	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	NVIC_InitTypeDef		NVIC_InitStructure;

	/* Timer clock enable */
#ifdef STM32F072
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM14, ENABLE );
#endif

#ifdef STM32F401xx
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );
#endif

	/* --------------------------NVIC Configuration -------------------------------*/

#ifdef STM32F072
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
#endif

#ifdef STM32F401xx
	/* Enable the TIM4 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
#endif
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( & NVIC_InitStructure );


#ifdef STM32F072
	/* ------------------- TIM14 Configuration:Output Compare Timing Mode ---------*/
	/* TIM14 Configuration: 
		The objective is to get TIM14 counter clock at 1 KHz:
		  - Prescaler = (TIM14CLK / TIM14 counter clock) - 1
		 And generate delays:
		 TIM14_CH1 delay = CCR1_Val/TIM14 counter clock = 1000 ms
	*/
#endif

#ifdef STM32F401xx
	/* ------------------- TIM4 Configuration:Output Compare Timing Mode ---------*/
	/* TIM4 Configuration: 
		The objective is to set TIM4 counter clock at 1KHz(TIM4 counter clock = 1KHz):
			  - TIM4CLK = 2 * PCLK1 = 2 * (HCLK / 4) = HCLK / 2 = SystemCoreClock /2
			  - PrescalerValue = (TIM4CLK / TIM4 counter clock) - 1
				- CC1 update rate = TIM4 counter clock / CCR1_Val
			 And generate delays:
			 TIM4CLK = SystemCoreClock / 2 = 84MHz /2 = 42MHz
			 PrescalerValue = (TIM4CLK / TIM4 counter clock) - 1 = (42MHz / 1KHz) - 1 = 41999
			 TIM4_CH1 update rate = TIM4 counter clock / CCR1_Val = 1000Hz / 1000 = 1s
	*/
	/* 以上公式部分出自原廠 F401 範例,但實測不準,故僅供參考 */

	// PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 32000) - 1;
#endif


	PrescalerValue = (uint16_t) ( ( SystemCoreClock / CONSOLE_TIMER__SYSCLK_Divder ) / 1000 ) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( CONSOLE_TIMER, & TIM_TimeBaseStructure );

	/* Prescaler configuration */
	// TIM_PrescalerConfig( CONSOLE_TIMER, PrescalerValue, TIM_PSCReloadMode_Immediate );

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(CONSOLE_TIMER, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(CONSOLE_TIMER, TIM_OCPreload_Disable);
	Console_Timer_Status = DISABLE;
}


/***************************************************************************************************
 *  Function Name: CmdTIMER_TimerRunOrStop
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	CmdTIMER_TimerRunOrStop( FunctionalState NewState )
{
	TIM_ITConfig( CONSOLE_TIMER, TIM_IT_CC1, NewState );
	TIM_Cmd( CONSOLE_TIMER, NewState );
	Console_Timer_Status = NewState;
	CLI_DelayCounter = 0;

	if( NewState == DISABLE ) {
		TIM_ClearITPendingBit( CONSOLE_TIMER, TIM_IT_CC1 );
		capture = TIM_GetCapture1( CONSOLE_TIMER );
		TIM_SetCompare1( CONSOLE_TIMER, capture + CCR1_Val );
	}
}


/***************************************************************************************************
 *  Function Name: CmdTIMER_ClearDelayCounter
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	CmdTIMER_ClearDelayCounter( void )
{
	CLI_DelayCounter = 0;
}


/***************************************************************************************************
 *  Function Name: CmdTIMER_IRQHandler
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	CONSOLE_TIMER__IRQ_Handler( void )
{
	if ( TIM_GetITStatus( CONSOLE_TIMER, TIM_IT_CC1 ) != RESET ) {
		/* Clear CONSOLE_TIMER update interrupt pending bit*/
		TIM_ClearITPendingBit( CONSOLE_TIMER, TIM_IT_CC1 );

		capture = TIM_GetCapture1( CONSOLE_TIMER );
		TIM_SetCompare1( CONSOLE_TIMER, capture + CCR1_Val );

		CLI_DelayCounter++;
		if( CLI_DelayCounter >= CONSOLE_TIMER__CCR1_DelayForCMD ) {
			CmdTIMER_TimerRunOrStop( DISABLE );
			CmdUART__Clear_Buffer();
			CmdUART__UartWrite( (uint8_t *)unkownCMD, strlen(unkownCMD) );
		}
		// GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_3)));  //test
	}
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
