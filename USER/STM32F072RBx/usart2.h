/**
  ******************************************************************************
  * @file    Project/ARM-Lora/usart2.h 
  * @author  JC
  * @version V1.0.0
  * @date    15-Jan-2016
  * @brief   Header for usart2.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */

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
#ifndef __USART2_H
#define __USART2_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif


/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define		USART2_RX_BufLength		(256)

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	USART2_PinInitialization(void);
void	USART2_UartConfig(uint32_t);
void	USART2_UartEnableOrDisable(FunctionalState);
void	USART2_UartInit(uint32_t);
void	USART2_UartWrite(uint8_t *, uint32_t);
void	USART2_IRQHandler(void);
void	USART2_EnvironmentVariableSet(void);
void	USART2_VariableSwap(void);
void	USART2_DigitalClear(void);



#endif /* __USART2_H */



/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/


