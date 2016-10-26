
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
  * @file    Project/ARM-Lora/tim4.h 
  * @author  JC
  * @version V1.0.0
  * @date    10-Mar-2016
  * @brief   Header for tim4.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_CONSOLE_TIMER_H_
#define USER_CONSOLE_TIMER_H_

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern __IO FunctionalState	Console_Timer_Status;

/* Exported functions ------------------------------------------------------- */
void	CmdTIMER_TimerConfig( void );
void	CmdTIMER_TimerRunOrStop(FunctionalState);
void	CmdTIMER_ClearDelayCounter(void);
void	CmdTIMER_IRQHandler(void);

#endif		// USER_CONSOLE_TIMER_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
