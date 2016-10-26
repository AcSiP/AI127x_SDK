
//---------------------------------------------------------------------------
/*
//==========================================
// Author : JC<jc@acsip.com.tw>
// Copyright 2016(C) AcSiP Technology Inc.
// 唳垀衄ㄩ腎褪撮嘖爺衄癹鼠侗
// http://www.acsip.com.tw
//==========================================
*/
//---------------------------------------------------------------------------

/**
  ******************************************************************************
  * @file    Project/ARM-Lora/tim2.c 
  * @author  JC
  * @version V1.0.0
  * @date    14-Apr-2016
  * @brief   TIM2 program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "tim2.h"
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  CCR2_Val		(2000)
// #define  GPS_Run15		(15)
// #define  GPS_Run60		(60)
// #define  GPS_NotActionTimes	(5)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t	GPS_DelayCounter;
// uint8_t	GPS_RunTime;
// uint8_t	GPS_NotAction;
// bool		GPS_NowLocated;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***************************************************************************************************
 *  Function Name: TIM2_TimerConfig
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM2_TimerConfig(void)
{
	__IO uint16_t			PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	NVIC_InitTypeDef		NVIC_InitStructure;

	// GPS_RunTime = GPS_Run15;
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* --------------------------NVIC Configuration -------------------------------*/
	/* Enable the TIM2 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* ------------------- TIM2 Configuration:Output Compare Timing Mode ---------*/
	/* TIM2 Configuration: 
		The objective is to set TIM2 counter clock at 1KHz(TIM2 counter clock = 1KHz):
			  - TIM2CLK = 2 * PCLK1 = 2 * (HCLK / 4) = HCLK / 2 = SystemCoreClock /2
			  - PrescalerValue = (TIM2CLK / TIM2 counter clock) - 1
				- CC1 update rate = TIM2 counter clock / CCR1_Val
			 And generate delays:
			 TIM2CLK = SystemCoreClock / 2 = 84MHz /2 = 42MHz
			 PrescalerValue = (TIM2CLK / TIM2 counter clock) - 1 = (42MHz / 1KHz) - 1 = 41999
			 TIM2_CH1 update rate = TIM2 counter clock / CCR1_Val = 1000Hz / 1000 = 1s
		*/
		/* 以上公式部分出自原廠 F401 範例,但實測不準,故僅供參考 */

	// PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 32000) - 1;
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Prescaler configuration */
	// TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
}


/***************************************************************************************************
 *  Function Name: TIM2_TimerRunOrStop
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM2_TimerRunOrStop(FunctionalState NewState)
{
	__IO uint16_t	capture;

	TIM_ITConfig(TIM2, TIM_IT_CC1, NewState);
	TIM_Cmd(TIM2, NewState);
	GPS_DelayCounter = 0;
	GPS_NowLocated = false;

	if(NewState == DISABLE) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		capture = TIM_GetCapture1(TIM2);
		TIM_SetCompare1(TIM2, capture + CCR2_Val);
	}
}


/***************************************************************************************************
 *  Function Name: TIM2_ClearDelayCounter
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM2_ClearDelayCounter(void)
{
	GPS_DelayCounter = 0;
}


/***************************************************************************************************
 *  Function Name: TIM2_IRQHandler
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM2_IRQHandler(void)
{
	__IO uint16_t	capture;

#if 1
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
		/* Clear TIM2 update interrupt pending bit*/
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		capture = TIM_GetCapture1(TIM2);
		TIM_SetCompare1(TIM2, capture + CCR2_Val);
		GPS_DelayCounter++;
		if(GPS_DelayCounter >= GPS_RunTime) {
			TIM2_TimerRunOrStop(DISABLE);
			GPS_MT3333Disable();
			if(GPS_NowLocated == false) GPS_NotAction++;
			if(GPS_NotAction >= GPS_NotActionTimes) {
				GPS_NotAction = 0;
				GPS_RunTime = GPS_Run60;
			}
		}
		// GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_3)));  //test
	}

#else		// for test

	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
		/* Clear TIM14 update interrupt pending bit*/
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		// PB3 for TIM2 rate in pin output
		GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_3)));
	}
#endif
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
