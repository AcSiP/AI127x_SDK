
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
  * @file    Project/ARM-Lora/tim9.h 
  * @author  JC
  * @version V1.0.0
  * @date    24-May-2016
  * @brief   Header for tim9.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_STM32F401VCX_TIM9_H_
#define USER_STM32F401VCX_TIM9_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	TIM9_TimerConfig(void);
void	TIM9_TimerRunOrStop(FunctionalState);
void	TIM9_ClearDelayCounter(void);
void	TIM1_BRK_TIM9_IRQHandler (void);

#endif		// USER_STM32F401VCX_TIM9_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
