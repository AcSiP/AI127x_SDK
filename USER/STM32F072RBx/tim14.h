/**
  ******************************************************************************
  * @file    Project/ARM-Lora/tim14.h 
  * @author  JC
  * @version V1.0.0
  * @date    29-Jan-2016
  * @brief   TIM14 program body
  ******************************************************************************
  * @attention
  *
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __tim14_H
#define __tim14_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	TIM14_TimerConfig( void );
void	TIM14_TimerRunOrStop( FunctionalState );
void	TIM14_ClearDelayCounter( void );
void	TIM14_IRQHandler( void );



#endif /* __tim14_H */



/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
