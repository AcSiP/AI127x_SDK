﻿
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
  * @file    usbd_cdc_vcp.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   Generic media access Layer.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
	#pragma		data_alignment = 4
#endif	/* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>

#include "config.h"
#include "usbd_cdc_vcp.h"
#include "tim9.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LINE_CODING	linecoding = {
	ComPortBaudRate,	/* baud rate*/
	0x00,	/* stop bits-1*/
	0x00,	/* parity - none*/
	0x08	/* nb. of bits 8*/
};

/* These are external variables imported from CDC core to be used for IN  transfer management. */

uint16_t		VCP_count;
uint16_t		VCP_RX_Length;
uint8_t			VCP_RX_Buf1[VCP_RX_BufLength];
uint8_t			VCP_RX_Buf2[VCP_RX_BufLength];
uint8_t *		VCP_RX_BufPointer;
bool			isVCP_RX_Buf1Full;
bool			isVCP_RX_Buf2Full;
bool *			isVCP_RX_BOOL;
bool			USBD_VCP_EchoOn = true;

extern	FunctionalState	Timer9_Status;
extern	uint8_t		APP_Rx_Buffer[];
/* Write CDC received data in this buffer. These data will be sent over USB IN endpoint in the CDC core functions. */

extern uint32_t		APP_Rx_ptr_in;
/* Increment this pointer or roll it back to start address when writing received data in the buffer APP_Rx_Buffer. */

/* Private function prototypes -----------------------------------------------*/
static uint16_t	VCP_Init	(void);
static uint16_t	VCP_DeInit	(void);
static uint16_t	VCP_Ctrl	(uint32_t Cmd, uint8_t* Buf, uint32_t Len);
uint16_t	VCP_DataTx	(uint8_t* Buf, uint32_t Len);
static uint16_t	VCP_DataRx	(uint8_t* Buf, uint32_t Len);


CDC_IF_Prop_TypeDef	VCP_fops = {
	VCP_Init,
	VCP_DeInit,
	VCP_Ctrl,
	VCP_DataTx,
	VCP_DataRx
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  VCP_Init
  *         Initializes the Media on the STM32
  * @param  None
  * @retval Result of the operation (USBD_OK in all cases)
  */
static uint16_t		VCP_Init(void)
{
	return USBD_OK;
}

/**
  * @brief  VCP_DeInit
  *         DeInitializes the Media on the STM32
  * @param  None
  * @retval Result of the operation (USBD_OK in all cases)
  */
static uint16_t		VCP_DeInit(void)
{
	return USBD_OK;
}


/**
  * @brief  VCP_Ctrl
  *         Manage the CDC class requests
  * @param  Cmd: Command code            
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation (USBD_OK in all cases)
  */
static uint16_t		VCP_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{
	switch( Cmd ) {
	case SEND_ENCAPSULATED_COMMAND:
		/* Not  needed for this driver */
		break;

	case GET_ENCAPSULATED_RESPONSE:
		/* Not  needed for this driver */
		break;

	case SET_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case GET_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case CLEAR_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case SET_LINE_CODING:
		linecoding.bitrate = (uint32_t)(Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
		linecoding.format = Buf[4];
		linecoding.paritytype = Buf[5];
		linecoding.datatype = Buf[6];
		break;

	case GET_LINE_CODING:
		Buf[0] = (uint8_t)(linecoding.bitrate);
		Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
		Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
		Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
		Buf[4] = linecoding.format;
		Buf[5] = linecoding.paritytype;
		Buf[6] = linecoding.datatype;
		break;

	case SET_CONTROL_LINE_STATE:
		/* Not  needed for this driver */
		break;

	case SEND_BREAK:
		/* Not  needed for this driver */
		break;

	default:
		break;
	}

	return USBD_OK;
}

void	VCP_EnvironmentVariableSet( void )
{
	memset( (void *)VCP_RX_Buf1, 0, VCP_RX_BufLength );
	memset( (void *)VCP_RX_Buf2, 0, VCP_RX_BufLength );

	VCP_RX_BufPointer = VCP_RX_Buf1;
	isVCP_RX_Buf1Full = false;
	isVCP_RX_Buf2Full = false;
	isVCP_RX_BOOL = & isVCP_RX_Buf1Full;

	VCP_count = 0;
}

void	VCP_VariableSwap( void )
{
	if( ( isVCP_RX_Buf1Full ) && ( ! isVCP_RX_Buf2Full ) ) {
		VCP_RX_BufPointer = VCP_RX_Buf2;
		isVCP_RX_BOOL = & isVCP_RX_Buf2Full;

		return;
	}

	if( ( ! isVCP_RX_Buf1Full ) && ( isVCP_RX_Buf2Full ) ) {
		VCP_RX_BufPointer = VCP_RX_Buf1;
		isVCP_RX_BOOL = &isVCP_RX_Buf1Full;

		return;
	}

	VCP_EnvironmentVariableSet();
}


/**
  * @brief  VCP_DataTx
  *         CDC received data to be send over USB IN endpoint are managed in 
  *         this function.
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else VCP_FAIL
  */
uint16_t		VCP_DataTx ( uint8_t* Buf, uint32_t Len )
{
	uint32_t	i;
	uint8_t		c;

	for( i = 0; Buf && i < Len && i < APP_RX_DATA_SIZE ; i++ ) {
		c = Buf[i];
		if (linecoding.datatype == 7) {
			APP_Rx_Buffer[APP_Rx_ptr_in] = c & 0x7F;
		} else {
			APP_Rx_Buffer[APP_Rx_ptr_in] = c;
		}
		APP_Rx_ptr_in++;

		/* To avoid buffer overflow */
		if( APP_Rx_ptr_in == APP_RX_DATA_SIZE ) APP_Rx_ptr_in = 0;
	}

	return USBD_OK;
}

/**
  * @brief  VCP_DataRx
  *         Data received over USB OUT endpoint are sent over CDC interface 
  *         through this function.
  *           
  *         @note
  *         This function will block any OUT packet reception on USB endpoint 
  *         until exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result 
  *         in receiving more data while previous ones are still not sent.
  *                 
  * @param  Buf: Buffer of data received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else VCP_FAIL
  */
static	uint16_t VCP_DataRx( uint8_t* Buf, uint32_t Len )
{
	uint32_t	i;
	uint8_t		c;

	if( USBD_VCP_EchoOn ) VCP_DataTx( Buf, Len );

	if(Timer9_Status == DISABLE) {
		TIM9_TimerRunOrStop(ENABLE);
	} else {
		TIM9_ClearDelayCounter();
	}

	for( i = 0; i < Len; i++ ) {
		// Read one byte
		c = Buf[i];
		VCP_RX_BufPointer[ VCP_count ] = c;
		VCP_count++;

		if( c == 0x0d ) {
			if( USBD_VCP_EchoOn ) VCP_DataTx((uint8_t *)"\r\n", strlen("\r\n"));

			*isVCP_RX_BOOL = true;
			VCP_RX_Length = VCP_count - 1;
			VCP_RX_BufPointer[ VCP_count - 1] = 0x00;
			VCP_VariableSwap();

			VCP_count = 0;
			TIM9_TimerRunOrStop( DISABLE );
		} else {
			if( VCP_count >= VCP_RX_BufLength ) {
				VCP_EnvironmentVariableSet();
				TIM9_TimerRunOrStop( DISABLE );
				VCP_DataTx((uint8_t *)unkownCMD, strlen(unkownCMD));
			}
		}
	}

	return USBD_OK;
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
