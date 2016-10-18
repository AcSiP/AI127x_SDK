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
#ifndef __tim9_H
#define __tim9_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TIM9_TimerConfig(void);
void TIM9_TimerRunOrStop(FunctionalState);
void TIM9_ClearDelayCounter(void);
void TIM1_BRK_TIM9_IRQHandler (void);



#endif /* __tim9_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


