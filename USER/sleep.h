
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
#ifndef USER_SLEEP_H_
#define USER_SLEEP_H_

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

#include "LinkListEvent.h"


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	SLEEP_SYSCLKConfigFromSTOPMode(void);
void	SLEEP_SlaveSleepVariableSet(void);
void	SLEEP_SlaveSleep_STOP_Mode(uint16_t *);
void	SLEEP_SlaveSleep_Deep_STOP_Mode( uint16_t *SleepTime_sec );
void	SLEEP_SlaveSleep_STANDBY_Mode( uint16_t *SleepTime_sec );
void	SLEEP_SlaveSleepAandRandomHopChannelProcedure(uint16_t *);
void	SLEEP_MasterSleepProcedure(void);

#endif		// USER_SLEEP_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
