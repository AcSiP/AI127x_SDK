
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
  * @file    Project/ARM-Lora/RandomHopStartChannel.c 
  * @author  JC
  * @version V1.0.0
  * @date    05-Jul-2016
  * @brief   Random Hopping Start Channel program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern bool		EnableMaster;		// 1=Master or 0=Slave selection
extern const int32_t	HoppingFrequencies[];
extern tLoRaSettings	LoRaSettings;
extern __IO uint8_t	SLAVE_LoraHoppingStartChannel;		// for SLAVE
__IO uint8_t		Running_HoppingStartChannel = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***************************************************************************************************
 *  Function Name: RandomHopStartChannel_SetChannelAndFreq
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void		RandomHopStartChannel_SetHoppingStartChannelFreq(uint8_t channel)
{
	if(LoRaSettings.FreqHopOn == false) return;
	if(channel >= 64) return;

	if(Running_HoppingStartChannel != channel) {
		Running_HoppingStartChannel = channel;
		SX1276LoRaSetRFFrequency( HoppingFrequencies[Running_HoppingStartChannel] );

		// 20160715 for the test
		char		str[6];
		snprintf( str, sizeof(str), "%d", Running_HoppingStartChannel );
		Console_Output_String( "SetHoppingChannel=" );
		Console_Output_String( str );
		Console_Output_String( "\r\n" );
		// 20160715 test end
	}
}


/***************************************************************************************************
 *  Function Name: RandomHopStartChannel_SlaveDefaultHoppingChannel
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void		RandomHopStartChannel_SlaveDefaultHoppingChannel(void)
{
  // if( ( SystemOperMode != SystemInNormal ) || (EnableMaster != false) ) return;

	SLAVE_LoraHoppingStartChannel = 0;
	RandomHopStartChannel_SetHoppingStartChannelFreq(SLAVE_LoraHoppingStartChannel);
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
