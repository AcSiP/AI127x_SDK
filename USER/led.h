
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
#ifndef USER_LED_H_
#define USER_LED_H_

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
void	Led_PinInitialization(void);
void	Led_BootFlashLed(void);
void	Led_GreenLedLightOn(void);
void	Led_GreenLedLightOff(void);
void	Led_BlueLedLightOn(void);
void	Led_BlueLedLightOff(void);

#endif		// USER_LED_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
