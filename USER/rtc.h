
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
#ifndef USER_RTC_H_
#define USER_RTC_H_

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Exported define -----------------------------------------------------------*/
#define SecondOfOneTimes	(2)

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	RTC_IRQ_Config( void );

void	RTC_TimerConfig(void);
void	RTC_AlarmConfig(void);					// Triggered every 1 second
void	RTC_AlarmConfig_Timeout( uint32_t timeout_sec );
void	RTC_AlarmRun(void);
void	RTC_AlarmStop(void);

#ifdef STM32F401xx
void	RTC_Alarm_IRQHandler(void);
#elif STM32F072
void	RTC_IRQHandler(void);
#endif

#endif		// USER_RTC_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
