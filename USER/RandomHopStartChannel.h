
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
#ifndef USER_RANDOMHOPSTARTCHANNEL_H_
#define USER_RANDOMHOPSTARTCHANNEL_H_

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	RandomHopStartChannel_SetHoppingStartChannelFreq(uint8_t);
void	RandomHopStartChannel_SlaveDefaultHoppingChannel(void);

#endif		// USER_RANDOMHOPSTARTCHANNEL_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
