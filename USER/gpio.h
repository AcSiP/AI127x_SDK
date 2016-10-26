
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
#ifndef USER_GPIO_H_
#define USER_GPIO_H_

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	GPIO_PinInitialization(void);
void	GPIO_UM402toNormalMode(void);
void	GPIO_UM402toWakeupMode(void);
void	GPIO_UM402toLowPowerMode(void);
void	GPIO_UM402toConfigMode(void);
void	EXTI4_15_IRQHandler(void);

#endif		// USER_GPIO_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
