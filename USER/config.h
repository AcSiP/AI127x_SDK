/**
  ******************************************************************************
  * @file    Project/ARM-Lora/config.h 
  * @author  JC
  * @version V1.0.0
  * @date    29-Jan-2016
  * @brief   All system config program body
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
#ifndef __config_H
#define __config_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
  #include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif

/* Exported define -----------------------------------------------------------*/
#define FirmwareVersion                     ("0.5.160630")
#define ComPortBaudRate                     (38400)

#define GPSnoLocated_RunningTime            (60)       // Unit:s

#ifdef STM32F072
	#define		TIM14CCR1_DelayForCMD		(3)		//second
	#define		USART2_EchoStatus		(true)
	#define		USART2_JudgmentSuffixNotEnter	(false)
#endif

#ifdef STM32F401xx
	#define		TIM4CCR1_DelayForCMD		(3)		//second
  #define		TIM9CCR1_DelayForCMD		(TIM4CCR1_DelayForCMD)		//second
	#define		USART2_EchoStatus		(true)
	#define		USART2_JudgmentSuffixNotEnter	(false)
  //#define   USBD_VCP_Console      (1)
  #undef    USBD_VCP_Console
#endif

#define		unkownCMD		("\r\nUNKNOW\r\n")
#define		okCMD			("\r\nOK\r\n")
#define		errorCMD		("\r\nERROR\r\n")

/* Define Name */
#ifdef STM32F072
  /* USART Communication Interfaces */
  #define CmdUART		USART2
	/* Define USART2 Const & Variable Name */
	#define CmdUART_RX_BufLength		USART2_RX_BufLength
	#define CmdUART_RX_Length		USART2_RX_Length
	#define CmdUART_EchoOn			USART2_EchoOn
	/* Define USART2 Function Name */
	#define CmdUART_PinInitialization	USART2_PinInitialization
	#define CmdUART_UartConfig		USART2_UartConfig
	#define CmdUART_UartEnableOrDisable	USART2_UartEnableOrDisable
	#define CmdUART_UartInit		USART2_UartInit
	#define CmdUART_UartWrite		USART2_UartWrite
	#define CmdUART_IRQHandler		USART2_IRQHandler
	#define CmdUART_EnvironmentVariableSet	USART2_EnvironmentVariableSet
	#define CmdUART_VariableSwap		USART2_VariableSwap
	#define CmdUART_DigitalClear		USART2_DigitalClear
	/* Define TIM14 Function Name */
	#define CmdTIMER_TimerConfig		TIM14_TimerConfig
	#define CmdTIMER_TimerRunOrStop		TIM14_TimerRunOrStop
	#define CmdTIMER_ClearDelayCounter	TIM14_ClearDelayCounter
	#define CmdTIMER_IRQHandler		TIM14_IRQHandler
#endif

#ifdef STM32F401xx
  /* USART Communication Interfaces */
  #define CmdUART		USART2
	/* Define USART2 Const & Variable Name */
	#define CmdUART_RX_BufLength		USART2_RX_BufLength
	#define CmdUART_RX_Length		USART2_RX_Length
	#define CmdUART_EchoOn			USART2_EchoOn
	/* Define USART2 Function Name */
	#define CmdUART_PinInitialization	USART2_PinInitialization
	#define CmdUART_UartConfig		USART2_UartConfig
	#define CmdUART_UartEnableOrDisable	USART2_UartEnableOrDisable
	#define CmdUART_UartInit		USART2_UartInit
	//#define CmdUART_UartWrite		USART2_UartWrite
  #define CmdUART_UartWrite		ConsoleFunction_CmdOutput
	#define CmdUART_IRQHandler		USART2_IRQHandler
	#define CmdUART_EnvironmentVariableSet	USART2_EnvironmentVariableSet
	#define CmdUART_VariableSwap		USART2_VariableSwap
	#define CmdUART_DigitalClear		USART2_DigitalClear
	/* Define TIM4 Function Name */
	#define CmdTIMER_TimerConfig		TIM4_TimerConfig
	#define CmdTIMER_TimerRunOrStop		TIM4_TimerRunOrStop
	#define CmdTIMER_ClearDelayCounter	TIM4_ClearDelayCounter
	#define CmdTIMER_IRQHandler		TIM4_IRQHandler
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */



#endif /* __config_H */



/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/


