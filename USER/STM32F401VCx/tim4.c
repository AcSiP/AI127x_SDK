/**
  ******************************************************************************
  * @file    Project/ARM-Lora/tim4.c 
  * @author  JC
  * @version V1.0.0
  * @date    10-Mar-2016
  * @brief   TIM4 program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "config.h"
#include "tim4.h"
#include "usart2.h"
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//const uint16_t CCR1_Val = 3200;
const uint16_t CCR1_Val = 2000;
__IO uint16_t capture = 0;
__IO uint16_t CLI_DelayCounter;
__IO FunctionalState Timer4_Status;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/***************************************************************************************************
 *  Function Name: TIM4_TimerConfig
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void TIM4_TimerConfig(void) {
	
	__IO uint16_t PrescalerValue = 0;
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	/* --------------------------NVIC Configuration -------------------------------*/
  /* Enable the TIM4 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
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
		/* そΑ场だ紅 F401 絛ㄒ,龟代ぃ非,珿度ㄑ把σ */
	
	//PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 32000) - 1;
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* Prescaler configuration */
  //TIM_PrescalerConfig(TIM4, PrescalerValue, TIM_PSCReloadMode_Immediate);
	
	/* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
	
	Timer4_Status = DISABLE;
	
}



/***************************************************************************************************
 *  Function Name: TIM4_TimerRunOrStop
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void TIM4_TimerRunOrStop(FunctionalState NewState) {
	
	TIM_ITConfig(TIM4, TIM_IT_CC1, NewState);
	TIM_Cmd(TIM4, NewState);
	Timer4_Status = NewState;
	CLI_DelayCounter = 0;
	
	if(NewState == DISABLE) {
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		capture = TIM_GetCapture1(TIM4);
		TIM_SetCompare1(TIM4, capture + CCR1_Val);
	}
	
}



/***************************************************************************************************
 *  Function Name: TIM4_ClearDelayCounter
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void TIM4_ClearDelayCounter(void) {
	
	CLI_DelayCounter = 0;
	
}



/***************************************************************************************************
 *  Function Name: TIM4_IRQHandler
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void TIM4_IRQHandler(void) {
	
#if 1
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) {
		
    /* Clear TIM4 update interrupt pending bit*/
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		
		capture = TIM_GetCapture1(TIM4);
		TIM_SetCompare1(TIM4, capture + CCR1_Val);
		
		CLI_DelayCounter++;
		if(CLI_DelayCounter >= TIM4CCR1_DelayForCMD) {
			TIM4_TimerRunOrStop(DISABLE);
			USART2_EnvironmentVariableSet();
			USART2_UartWrite((uint8_t *)unkownCMD, strlen(unkownCMD));
		}
		//GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_3)));  //test
  }
#else   //for test
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) {
		/* Clear TIM4 update interrupt pending bit*/
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		//PB3 for TIM4 rate in pin output
		GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_3)));
	}
#endif
	
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


