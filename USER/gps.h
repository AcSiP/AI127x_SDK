/**
  ******************************************************************************
  * @file    Project/ARM-Lora/gps.h  
  * @author  JC
  * @version V1.0.0
  * @date    12-Apr-2016
  * @brief   Header for gps.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPS_H
#define __GPS_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
  #include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif
#include "acsip_protocol.h"

/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void GPS_PinInitialization(void);
void GPS_MT3333Reset(void);
void GPS_MT3333Enable(void);
void GPS_MT3333Disable(void);
void GPS_MT3333Run(void);
void GPS_MT3333Stop(void);
void GPS_ReadIn(tDeviceNodeSensor *);



#endif /* __GPS_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


