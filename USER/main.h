
//---------------------------------------------------------------------------
/*
//==========================================
// Author : JC<jc@acsip.com.tw>
// Copyright 2016(C) AcSiP Technology Inc.
// 版權所有：群登科技股份有限公司
// http://www.acsip.com.tw
//==========================================
*/
//---------------------------------------------------------------------------

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_MAIN_H_
#define USER_MAIN_H_

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
#include "sx1276-Hal.h"
#include "platform.h"
#include "radio.h"
#include "board.h"
#include "base64.h"
#include "acsip_protocol.h"
#include "normalSlave.h"
#include "normalMaster.h"
#include "LinkListEvent.h"
#include "gps.h"
#include "save_record.h"
#include "bluetooth.h"
#include "led.h"
#include "button.h"
#include "rtc.h"
#include "RandomHopStartChannel.h"
#include "sleep.h"

//#include "usart1.h"
#include "UART_Console.h"
#include "Console_Timer.h"

#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"

	#include "usb_bsp.h"
	#include "usbd_cdc_core.h"
	#include "usbd_cdc_vcp.h"
	#include "usbd_usr.h"
	#include "usbd_desc.h"
	#include "console_function.h"
	#include "tim9.h"
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
#define enableGlobalInterrupts()	__set_PRIMASK(0);
#define disableGlobalInterrupts()	__set_PRIMASK(1);

/* Exported functions ------------------------------------------------------- */
void		LoraPara_LoadAndConfiguration(void);

#endif		// USER_MAIN_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
