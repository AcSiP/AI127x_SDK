/**
  ******************************************************************************
  * @file    Project/ARM-Lora/led.h  
  * @author  JC
  * @version V1.0.0
  * @date    17-May-2016
  * @brief   Header for led.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Led_H
#define __Led_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
  #include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Led_PinInitialization(void);
void Led_BootFlashLed(void);
void Led_GreenLedLightOn(void);
void Led_GreenLedLightOff(void);
void Led_BlueLedLightOn(void);
void Led_BlueLedLightOff(void);



#endif /* __Led_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


