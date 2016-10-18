/**
  ******************************************************************************
  * @file    Project/ARM-Lora/normalSlave.h 
  * @author  JC
  * @version 
  * @date    
  * @brief   Header for normalSlave.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NORMAL_SLAVE_H
#define __NORMAL_SLAVE_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#ifdef STM32F072
  #include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
//General parameters definition
//SX1276 LoRa General parameters definition
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t NormalSlave(void);



#endif /* __NORMAL_SLAVE_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


