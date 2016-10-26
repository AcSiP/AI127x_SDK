
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
  * @file    Project/ARM-Lora/tim9.c 
  * @author  JC
  * @version V1.0.0
  * @date    24-May-2016
  * @brief   TIM9 program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "config.h"
#include "tim9.h"
#include "stm32f4xx.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// const uint16_t	TIM9_CCR1_Val = 3200;
const uint16_t		TIM9_CCR1_Val = 2000;
__IO uint16_t		TIM9_capture = 0;
__IO uint16_t		USB_CLI_DelayCounter;
__IO FunctionalState	Timer9_Status;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***************************************************************************************************
 *  Function Name: TIM9_TimerConfig
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM9_TimerConfig(void)
{
	__IO uint16_t			PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	NVIC_InitTypeDef		NVIC_InitStructure;

	/* TIM9 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);

	/* --------------------------NVIC Configuration -------------------------------*/
	/* Enable the TIM9 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* ------------------- TIM9 Configuration:Output Compare Timing Mode ---------*/
	/* TIM9 Configuration: 
		The objective is to set TIM9 counter clock at 1KHz(TIM9 counter clock = 1KHz):
			  - TIM9CLK = 2 * PCLK1 = 2 * (HCLK / 4) = HCLK / 2 = SystemCoreClock /2
			  - PrescalerValue = (TIM9CLK / TIM9 counter clock) - 1
				- CC1 update rate = TIM9 counter clock / TIM9_CCR1_Val
			 And generate delays:
			 TIM9CLK = SystemCoreClock / 2 = 84MHz /2 = 42MHz
			 PrescalerValue = (TIM9CLK / TIM9 counter clock) - 1 = (42MHz / 1KHz) - 1 = 41999
			 TIM9_CH1 update rate = TIM9 counter clock / TIM9_CCR1_Val = 1000Hz / 1000 = 1s
		*/
		/* 以上公式部分出自原廠 F401 範例,但實測不準,故僅供參考 */

	// PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 32000) - 1;
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);

	/* Prescaler configuration */
	// TIM_PrescalerConfig(TIM9, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_Pulse = TIM9_CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Disable);

	Timer9_Status = DISABLE;
}


/***************************************************************************************************
 *  Function Name: TIM9_TimerRunOrStop
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM9_TimerRunOrStop(FunctionalState NewState)
{
	TIM_ITConfig(TIM9, TIM_IT_CC1, NewState);
	TIM_Cmd(TIM9, NewState);
	Timer9_Status = NewState;
	USB_CLI_DelayCounter = 0;
	if(NewState == DISABLE) {
		TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);
		TIM9_capture = TIM_GetCapture1(TIM9);
		TIM_SetCompare1(TIM9, TIM9_capture + TIM9_CCR1_Val);
	}
}


/***************************************************************************************************
 *  Function Name: TIM9_ClearDelayCounter
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM9_ClearDelayCounter(void)
{
	USB_CLI_DelayCounter = 0;
}


/***************************************************************************************************
 *  Function Name: TIM1_BRK_TIM9_IRQHandler 
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	TIM1_BRK_TIM9_IRQHandler (void)
{
#if 1
	if (TIM_GetITStatus(TIM9, TIM_IT_CC1) != RESET) {
		/* Clear TIM9 update interrupt pending bit*/
		TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);

		TIM9_capture = TIM_GetCapture1(TIM9);
		TIM_SetCompare1(TIM9, TIM9_capture + TIM9_CCR1_Val);

		USB_CLI_DelayCounter++;
		if( USB_CLI_DelayCounter >= CONSOLE_TIMER__CCR1_DelayForCMD ) {
			TIM9_TimerRunOrStop(DISABLE);
			VCP_EnvironmentVariableSet();
			VCP_DataTx((uint8_t *)unkownCMD, strlen(unkownCMD));
		}
		// GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_3)));  //test
	}

#else		// for test
	if (TIM_GetITStatus(TIM9, TIM_IT_CC1) != RESET) {
		/* Clear TIM9 update interrupt pending bit*/
		TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);
		// PB3 for TIM9 rate in pin output
		GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_3)));
	}
#endif
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
