/**
  ******************************************************************************
  * @file    Project/TESTmain.h 
  * @author  
  * @version 
  * @date    
  * @brief   Header for TESTmain.c module
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TESTMAIN_H
#define __TESTMAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "TESTcli.h"
#include "sx1276-LoRaMisc.h"
#include "sx1276-LoRa.h"
#include "sx1276.h"
#include "platform.h"
#include "radio.h"
#include "board.h"
#include "base64.h"

//#include "gpio.h"    //for test

#ifdef STM32F072
  #include "stm32f0xx.h"
	#include "usart1.h"
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
}tAllSystemOperationMode;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define enableGlobalInterrupts()   __set_PRIMASK(0);
#define disableGlobalInterrupts()  __set_PRIMASK(1);

/* Exported functions ------------------------------------------------------- */

#endif /* __TESTMAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
