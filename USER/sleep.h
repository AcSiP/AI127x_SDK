
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
typedef struct sDeviceNodeSleepAndRandomHop {
	// for sleep
	bool			isNowSleeping;
	uint16_t		WakeUpTimePoint;

	// for Lora disconnect
	uint16_t		DefineLoraRxFailureTimes;
	bool			isLoraDisconnecting;
	uint16_t		LoraRxFailureTimes;

	// for random hopping start channel
	uint8_t			LoraHoppingStartChannel;

	// for Lora Event
	tLoraNodeEvent *	EventHeadPriority0;
	tLoraNodeEvent *	EventTailPriority0;
	uint8_t			EventCountPriority0;

	tLoraNodeEvent *	EventHeadPriority1;
	tLoraNodeEvent *	EventTailPriority1;
	uint8_t			EventCountPriority1;

	tLoraNodeEvent *	EventHeadPriority2;
	tLoraNodeEvent *	EventTailPriority2;
	uint8_t			EventCountPriority2;
} tDeviceNodeSleepAndRandomHop;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	SLEEP_SYSCLKConfigFromSTOPMode(void);
void	SLEEP_SlaveSleepVariableSet(void);
void	SLEEP_SlaveSleep_STOP_Mode(uint16_t *);
void	SLEEP_SlaveSleep_STANDBY_Mode( uint16_t *SleepTime_sec );
void	SLEEP_SlaveSleepAandRandomHopChannelProcedure(uint16_t *);
void	SLEEP_MasterSleepProcedure(void);

#endif		// USER_SLEEP_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
