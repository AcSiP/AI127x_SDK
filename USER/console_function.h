
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
  * @file    Project/ARM-Lora/console_function.h
  * @author  JC
  * @version V1.0.0
  * @date    24-May-2016
  * @brief   Header for console_function.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_CONSOLE_FUNCTION_H_
#define USER_CONSOLE_FUNCTION_H_

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	Console_Write( const uint8_t *Str, uint32_t Length );

void	Console_Output_String( const char *msg );
void	Console_Dump_Binary( const uint8_t *buf, uint16_t length );

void	Console_Output_LoraMode( uint8_t mode );

#endif		// USER_CONSOLE_FUNCTION_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
