/**
  ******************************************************************************
  * @file    Project/ARM-Lora/crc.h 
  * @author  JC
  * @version V1.0.0
  * @date    08-Mar-2016
  * @brief   CRC program body
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRC_H
#define __CRC_H

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
void CRC16(__IO uint8_t *, uint16_t, uint16_t *);



#endif /* __CRC_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


