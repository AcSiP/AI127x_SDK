
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
#define FirmwareVersion				("2.12.2017.0617")
#define ComPortBaudRate				(38400)
#define GPSnoLocated_RunningTime		(60)		// Unit:s
#define	DEF_Allowed_LoRa_Rx_Failure_Times_Times	(5)		// Unit: polling round
#define	CONSOLE_TIMER__CCR1_DelayForCMD		(3)		// second


#ifdef STM32F401xx
// 	#define		USBD_VCP_Console		(1)
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
#define		MAX_HOPPING_CHANNELS	64

#ifdef STM32F072
	#define MAX_LoraNodeNum			(16)
#endif

#ifdef STM32F401xx
	#define MAX_LoraNodeNum			(32)
#endif



#define MaxPacketSize				(255)
/* Fix by JC in 20170616 */
//#define MaxMsgDataSize				(243)		// 255(Total Size = MaxPacketSize) - 9(Length:1、Flag:1、SN:1、NodeAdd:3、NextHoppingChannel:1、CRC:2) - 3(MyAdd) = 243
								// 在預設的一個訊框 255bytes 長度,一個 LoraNodeData 的訊框可以放進 243bytes 的 BASE64 的資料長度。

#define MaxMsgDataSize				(246)		// 255(Total Size = MaxPacketSize) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 246
/* Fix end */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif		// USER_CONFIG_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
