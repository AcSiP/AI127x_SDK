/**
  ******************************************************************************
  * @file    Project/ARM-Lora/gpio.h 
  * @author  JC
  * @version V1.0.0
  * @date    20-Jan-2016
  * @brief   Header for GPIO.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H

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
void	GPIO_PinInitialization(void);
void	GPIO_UM402toNormalMode(void);
void	GPIO_UM402toWakeupMode(void);
void	GPIO_UM402toLowPowerMode(void);
void	GPIO_UM402toConfigMode(void);
void	EXTI4_15_IRQHandler(void);



#endif /* __GPIO_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


