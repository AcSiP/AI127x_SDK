
//---------------------------------------------------------------------------
/*
//==========================================
// Author : Fox<shenfa@acsip.com.tw>
// Copyright 2016(C) AcSiP Technology Inc.
// 版權所有：群登科技股份有限公司
// http://www.acsip.com.tw
//==========================================
*/
//---------------------------------------------------------------------------

#if defined( Console__Use_UART1 ) || defined( Console__Use_UART2 )

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_UART_CONSOLE_H_
#define USER_UART_CONSOLE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "Message_Queue.h"

/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define		CmdUART_String_Buffer_Length	256
#define		CmdUART_String_Item_Size	8


/* Exported variables --------------------------------------------------------*/
extern	bool	Console_EchoOn;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
bool	CmdUART__Init( void );
void	CmdUART__Open( uint32_t BaudRate );
void	CmdUART__Close( void );
void	CmdUART__UartWrite( const uint8_t * Str, uint32_t Length );
void	CmdUART__UartWrite_String( const char *str );
bool	CmdUART__UartGet_String( uint8_t *buffer, uint16_t buf_sz, uint16_t *ret_length );
bool	CmdUART__is_TX_Queue_Empty( void );
void	CmdUART__Clear_Buffer( void );

#endif		// USER_UART_CONSOLE_H_

#endif

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
