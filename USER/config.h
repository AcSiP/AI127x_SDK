
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
#ifndef USER_CONFIG_H_
#define USER_CONFIG_H_

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

#include "console_function.h"



/* Exported define -----------------------------------------------------------*/
#define FirmwareVersion				("1.4.2017.0301")
#define ComPortBaudRate				(38400)
#define GPSnoLocated_RunningTime		(60)		// Unit:s
#define	DEF_Allowed_LoRa_Rx_Failure_Times_Times	(5)		// Unit: polling round
#define	CONSOLE_TIMER__CCR1_DelayForCMD		(3)		// second


#ifdef STM32F401xx
//	#define		TIM4CCR1_DelayForCMD		(3)		// second
//	#define		TIM9CCR1_DelayForCMD		(TIM4CCR1_DelayForCMD)		// second
 	#define		USBD_VCP_Console		(1)
 	#undef		USBD_VCP_Console
#endif

#define		unkownCMD		("\r\nUNKNOW\r\n")
#define		okCMD			("\r\nOK\r\n")
#define		errorCMD		("\r\nERROR\r\n")

// #define	Board__A22_Tracker	(1)
#undef		Board__A22_Tracker

#define		Console__Use_UART2
#undef		Console__Use_UART1


#define		USE_HARDWARE__USART1
#define		USE_HARDWARE__USART2
#define		USE_HARDWARE__ADC1


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif		// USER_CONFIG_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
