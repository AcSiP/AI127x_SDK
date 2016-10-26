
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
#ifndef USER_GPS_H_
#define USER_GPS_H_

#ifdef Board__A22_Tracker

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
void	GPS_PinInitialization(void);
void	GPS_MT3333Reset(void);
void	GPS_MT3333Enable(void);
void	GPS_MT3333Disable(void);
void	GPS_MT3333Run(void);
void	GPS_MT3333Stop(void);
void	GPS_ReadIn(tDeviceNodeSensor *);

#endif		// Board__A22_Tracker

#endif		// USER_GPS_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
