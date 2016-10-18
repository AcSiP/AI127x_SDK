/**
  ******************************************************************************
  * @file    Project/ARM-Lora/bluetooth.h  
  * @author  JC
  * @version V1.0.0
  * @date    11-May-2016
  * @brief   Header for bluetooth.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BlueTooth_H
#define __BlueTooth_H

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
void BlueTooth_PinInitialization(void);
void BlueTooth_DA14580Reset(void);
void BlueTooth_DA14580Enable(void);
void BlueTooth_DA14580Disable(void);
void BlueTooth_DA14580Run(uint32_t);
void BlueTooth_DA14580Stop(void);



#endif /* __BlueTooth_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


