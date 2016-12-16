
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
#ifndef USER_BLUETOOTH_H_
#define USER_BLUETOOTH_H_

#ifdef Board__A22_Tracker

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
void	BlueTooth_PinInitialization(void);
void	BlueTooth_DA14580Reset(void);
void	BlueTooth_DA14580Enable(void);
void	BlueTooth_DA14580Disable(void);
void	BlueTooth_DA14580Run(uint32_t);
void	BlueTooth_DA14580Stop(void);

#endif		// Board__A22_Tracker

#endif		// USER_BLUETOOTH_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
