/**
  ******************************************************************************
  * @file    Project/ARM-Lora/normalMaster.h 
  * @author  JC
  * @version 
  * @date    
  * @brief   Header for normalMaster.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NORMAL_MASTER_H
#define __NORMAL_MASTER_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#ifdef STM32F072
  #include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif
#include "acsip_protocol.h"
#include "LinkListEvent.h"

/* Exported types ------------------------------------------------------------*/
//General parameters definition
//SX1276 LoRa General parameters definition
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t NormalMaster(__IO tLoraRunningEvent *);



#endif /* __NORMAL_MASTER_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


