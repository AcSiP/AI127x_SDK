/**
  ******************************************************************************
  * @file    Project/ARM-Lora/tim4.h 
  * @author  JC
  * @version V1.0.0
  * @date    10-Mar-2016
  * @brief   Header for tim4.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __tim4_H
#define __tim4_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TIM4_TimerConfig(void);
void TIM4_TimerRunOrStop(FunctionalState);
void TIM4_ClearDelayCounter(void);
void TIM4_IRQHandler(void);



#endif /* __tim4_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


