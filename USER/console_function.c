
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
  * @file    Project/ARM-Lora/console_function.c
  * @author  JC
  * @version V1.0.0
  * @date    24-May-2016
  * @brief   console_function program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
// #include <stdbool.h>
#include <stdio.h>
#include <string.h>
// #include <time.h>

#include "config.h"
#include "UART_Console.h"
#include "console_function.h"

#ifdef STM32F401xx
#ifdef USBD_VCP_Console
#include "usbd_cdc_vcp.h"
#endif
#endif


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Const variables -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef STM32F401xx
#ifdef USBD_VCP_Console
__IO bool	UsbDegugOn = true;
#endif
#endif

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void	Console_Write( const uint8_t *Str, uint32_t Length )
{
	CmdUART__UartWrite( Str, Length );

#ifdef STM32F401xx
#ifdef USBD_VCP_Console
	if(UsbDegugOn == true) VCP_DataTx(Str, Length);
#endif
#endif
}

void	Console_Output_String( const char *msg )
{
	if( msg ) Console_Write( (uint8_t *) msg, strlen( msg ) );
}

void	Console_Dump_Binary( const uint8_t *buf, uint16_t length )
{
	char		sbuf[256];
	uint16_t	k, dumped_sz;

	if( ! buf ) return;
	Console_Output_String( "=========================================================================\r\n" );
	for( dumped_sz = 0; dumped_sz < length;  ){
		while( ! CmdUART__is_TX_Queue_Empty() ){
		}

		for( k = 0; k < 16 && dumped_sz < length ; k++ ){
			if( k == 0 ){
				snprintf( sbuf, sizeof(sbuf), "0x%08X  ", dumped_sz );
				Console_Output_String( sbuf );
			}
			if( k == 4 || k == 12 ) Console_Output_String( " -" );
			if( k == 8 )		Console_Output_String( " --" );
			
			snprintf( sbuf, sizeof(sbuf), " %02X", buf[ dumped_sz + k ] );
			Console_Output_String( sbuf );
		}
		Console_Output_String( "\r\n" );
		dumped_sz += k;
	}
	Console_Output_String( "=========================================================================\r\n\r\n" );
}

void	Console_Output_LoraMode( uint8_t mode )
{
	char	buf[32];

	snprintf( buf, sizeof(buf), "RegOPMode=0x%02X, Mode=", mode );
	Console_Output_String( buf );
	switch( mode & 0x07 ){
	case 0:
		Console_Output_String( "Sleep\r\n" );
		break;

	case 1:
		Console_Output_String( "Standby\r\n" );
		break;

	case 2:
		Console_Output_String( "FS-Tx\r\n" );
		break;

	case 3:
		Console_Output_String( "Tx\r\n" );
		break;

	case 4:
		Console_Output_String( "FS-Rx\r\n" );
		break;

	case 5:
		Console_Output_String( "Rx\r\n" );
		break;

	case 6:
		Console_Output_String( "Rx-Single\r\n" );
		break;

	case 7:
		Console_Output_String( "CAD\r\n" );
		break;

	default:
		Console_Output_String( "N/A\r\n" );
	}
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
