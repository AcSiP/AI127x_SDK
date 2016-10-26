
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
#ifndef USER_BUTTON_H_
#define USER_BUTTON_H_

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
void	Button_ButtonInitialization(void);
void	Button_Delay(void);
void	Button_PowerButtonAction(void);
#ifdef STM32F401xx
void	EXTI0_IRQHandler(void);
void	EXTI1_IRQHandler(void);
void	EXTI4_IRQHandler(void);
#elif STM32F072
void	EXTI0_1_IRQHandler(void);
void	EXTI4_15_IRQHandler(void);
#endif

#endif		// USER_BUTTON_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
