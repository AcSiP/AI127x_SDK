/**
  ******************************************************************************
  * @file    Project/UnionStructTestMain.h 
  * @author  
  * @version 
  * @date    
  * @brief   Header for UnionStructTestMain.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UnionStructTestMain_H
#define __UnionStructTestMain_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "cli.h"
#include "sx1276-LoRaMisc.h"
#include "sx1276-LoRa.h"
#include "sx1276.h"
#include "platform.h"
#include "radio.h"
#include "board.h"
#include "base64.h"
#include "acsip_protocol.h"
#include "normalSlave.h"
#include "normalMaster.h"
#include "LinkListEvent.h"
#include "gps.h"

#ifdef STM32F072
	#include "stm32f0xx.h"
	#include "usart2.h"
	#include "tim14.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
	#include "usart2.h"
	#include "tim4.h"
#endif



/* Exported types ------------------------------------------------------------*/
typedef enum {
	SystemInPingPognTest,
	SystemInNormal,
	SystemInProductVerification,
	SystemInIdle,
} tAllSystemOperationMode;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define enableGlobalInterrupts()   __set_PRIMASK(0);
#define disableGlobalInterrupts()  __set_PRIMASK(1);

/* Exported functions ------------------------------------------------------- */

#endif /* __UnionStructTestMain_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
