
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

/**
  ******************************************************************************
  * @file    Project/ARM-Lora/usart2.c 
  * @author  JC
  * @version V1.0.0
  * @date    10-Mar-2016
  * @brief   USART2 program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>
#include "config.h"
#include "UART_Console.h"
#include "Console_Timer.h"

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

#ifdef STM32F072
	#include "stm32f0xx.h"
#endif


#if defined( Console__Use_UART1 ) || defined( Console__Use_UART2 )


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef	Console__Use_UART1
	#include "Base_Driver__UART1.h"

	#define		UART__Init		UART1__Init
	#define		UART__Open		UART1__Open
	#define		UART__Close		UART1__Close
	#define		UART__UartWrite		UART1__UartWrite
	#define		UART__TX_Queue		UART1__TX_Queue
#endif

#ifdef	Console__Use_UART2
	#include "Base_Driver__UART2.h"

	#define		UART__Init		UART2__Init
	#define		UART__Open		UART2__Open
	#define		UART__Close		UART2__Close
	#define		UART__UartWrite		UART2__UartWrite
	#define		UART__TX_Queue		UART2__TX_Queue
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
Message_Queue_Struct		Console_Cmd_Queue;
Byte_Queue_Struct		Console_UART__RX_Queue;
char				Console_Cmd__Temp_Buffer[ CmdUART_String_Buffer_Length ];

bool				Console_EchoOn = true;

/* Private function prototypes -----------------------------------------------*/
void		UART__RX_Callback( uint8_t data );


/* Private functions ---------------------------------------------------------*/
void		UART__RX_Callback( uint8_t data )
{
	if( Console_EchoOn ) {
		UART__UartWrite( & data , 1 );
	}

	if( Console_Timer_Status == DISABLE ) {
		CmdTIMER_TimerRunOrStop( ENABLE );
	} else {
		CmdTIMER_ClearDelayCounter();
	}

	if( data == 0x0d ) {
		uint16_t	i;

		CmdTIMER_TimerRunOrStop( DISABLE );
		for( i = 0; i < CmdUART_String_Buffer_Length; i++ ) {
			if( ! Get_Byte_from_Queue( & Console_UART__RX_Queue ) ) break;

			Console_Cmd__Temp_Buffer[ i ] = Console_UART__RX_Queue.Output;
		}
		Console_Cmd__Temp_Buffer[ i ] = 0x00;
		i++;

		Put_Message_into_Queue( & Console_Cmd_Queue, ( uint8_t *) Console_Cmd__Temp_Buffer, i );
	} else {
		Put_Byte_into_Queue( & Console_UART__RX_Queue, data );
	}
}


bool	CmdUART__Init( void )
{
	UART__Init( UART__RX_Callback, NULL );
	Init_Byte_Queue( & Console_UART__RX_Queue, CmdUART_String_Buffer_Length );
	return( Init_Message_Queue( & Console_Cmd_Queue, CmdUART_String_Buffer_Length, CmdUART_String_Item_Size ) );
}


void	CmdUART__Open( uint32_t BaudRate )
{
	Clear_Message_Queue( & Console_Cmd_Queue );
	UART__Open( BaudRate );
}

void	CmdUART__Close( void )
{
	UART__Close();
	CmdTIMER_TimerRunOrStop( DISABLE );
}

bool	CmdUART__is_TX_Queue_Empty( void )
{
	return( Is_Byte_Queue_Empty( & UART__TX_Queue ) );
}

void	CmdUART__Clear_Buffer( void )
{
	Clear_Byte_Queue( & Console_UART__RX_Queue );
}

void	CmdUART__UartWrite_String( const char *str )
{
	UART__UartWrite( ( uint8_t *) str, strlen( str ) );
}

void	CmdUART__UartWrite( const uint8_t * Str, uint32_t Length )
{
	UART__UartWrite( Str, Length );
}

bool	CmdUART__UartGet_String( uint8_t *buffer, uint16_t buf_sz, uint16_t *ret_length )
{
	return( Get_Message_from_Queue( buffer, buf_sz, ret_length, & Console_Cmd_Queue ) );
}

#endif		// ( Console__Use_UART1 || Console__Use_UART2 )

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
