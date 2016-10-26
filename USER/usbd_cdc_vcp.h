
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
  * @file    usbd_cdc_vcp.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   Header for usbd_cdc_vcp.c file.
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_USBD_CDC_VCP_H_
#define USER_USBD_CDC_VCP_H_

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

#include "usbd_cdc_core.h"
#include "usbd_conf.h"
//#include "UART_Console.h"


/* Exported typef ------------------------------------------------------------*/
/* The following structures groups all needed parameters to be configured for the 
   ComPort. These parameters can modified on the fly by the host through CDC class
   command class requests. */
typedef struct
{
	uint32_t	bitrate;
	uint8_t		format;
	uint8_t		paritytype;
	uint8_t		datatype;
} LINE_CODING;

/* Exported constants --------------------------------------------------------*/

#define DEFAULT_CONFIG			0
#define OTHER_CONFIG			1

#define	VCP_RX_BufLength		256


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void		VCP_EnvironmentVariableSet( void );
uint16_t	VCP_DataTx ( uint8_t* Buf, uint32_t Len );

/* Exported variables ------------------------------------------------------------*/
extern	CDC_IF_Prop_TypeDef	VCP_fops;


extern	uint16_t		VCP_count;
extern	uint16_t		VCP_RX_Length;
extern	uint8_t			VCP_RX_Buf1[VCP_RX_BufLength];
extern	uint8_t			VCP_RX_Buf2[VCP_RX_BufLength];
extern	bool			isVCP_RX_Buf1Full;
extern	bool			isVCP_RX_Buf2Full;
extern	bool			*isVCP_RX_BOOL;
extern	bool			USBD_VCP_EchoOn;


#endif		// USER_USBD_CDC_VCP_H_

/************************ (C) COPYRIGHT STMicroelectronics *********/
/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
