
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



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_STM32F401VCX_BASE_DRIVER__UART1_H_
#define USER_STM32F401VCX_BASE_DRIVER__UART1_H_

#ifdef USE_HARDWARE__USART1

/* Includes ------------------------------------------------------------------*/
#include "Byte_Queue.h"

/* Exported types ------------------------------------------------------------*/
typedef	void	(*Fn_UART1__RX_Callback)( uint8_t data );

/* Exported define -----------------------------------------------------------*/
#define		UART1__TX_Buffer_Size		256

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern	Byte_Queue_Struct	UART1__TX_Queue;


/* Exported functions ------------------------------------------------------- */
void	UART1__Init( Fn_UART1__RX_Callback cb, Byte_Queue_Struct *RX_Queue );
void	UART1__Open( uint32_t BaudRate );
void	UART1__Close( void );
void	UART1__UartWrite( const uint8_t * Str, uint32_t Length );

#endif		// USE_HARDWARE__USART1

#endif		// USER_STM32F401VCX_BASE_DRIVER__UART1_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
