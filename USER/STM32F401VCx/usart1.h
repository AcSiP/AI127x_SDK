
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


/**
  ******************************************************************************
  * @file    Project/ARM-Lora/usart1.h  
  * @author  JC
  * @version V1.0.0
  * @date    12-Apr-2016
  * @brief   Header for usart1.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_STM32F401VCX_USART1_H_
#define USER_STM32F401VCX_USART1_H_

#ifdef Board__A22_Tracker


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define		USART1_RX_BufLength		(100)

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	USART1_PinInitialization(void);
void	USART1_UartConfig(uint32_t);
void	USART1_UartEnableOrDisable(FunctionalState);
void	USART1_UartInit(uint32_t);
void	USART1_UartWrite(uint8_t *, uint32_t);
void	USART1_IRQHandler(void);
void	USART1_EnvironmentVariableSet(void);
void	USART1_VariableSwap(void);

#endif		// Board__A22_Tracker

#endif		// USER_STM32F401VCX_USART1_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
