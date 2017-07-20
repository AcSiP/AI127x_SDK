
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

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

#include "config.h"
#include "radio.h"
#include "acsip_protocol.h"
#include "main.h"
#include "normalSlave.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO bool			Slave_PollEvent = false;
/* Add by JC in 20170711 */
extern tLoRaSettings		LoRaSettings;
/* Add end */
extern __IO bool 		LoraNeedTxData;
extern __IO size_t		LoraTxPayloadSize;
extern __IO size_t		LoraRxPayloadSize;
extern uint8_t			LoraTxBuffer[LoraBufferLength];
extern uint8_t			LoraRxBuffer[LoraBufferLength];
extern __IO tAcsipProtocolFrame	TxFrame;
extern __IO tAcsipProtocolFrame RxFrame;
extern uint8_t			LoraNodeCount;					// for MASTER
extern tLoraDeviceNode *	LoraGateWay;					// for SLAVE
extern tDeviceNodeSensor *	MySensor;					// for SLAVE
extern uint8_t			TxDataArray[MaxMsgDataSize];
extern uint8_t			TxDataSize;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***************************************************************************************************
 *  Function Name: NormalSlave
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for SLAVE
 **************************************************************************************************/
uint8_t		NormalSlave(void)
{
	uint8_t		result, tx_size;
	// int8_t	str[10];		// test output

	result = AcsipProtocol_NoAction;

	switch(RxFrame.FrameFlag) {
	case FrameFlag_Join:
		result = AcsipProtocol_LoraJoinNetworkResponse(&RxFrame, true, &TxFrame, LoraTxBuffer, &tx_size);
		LoraTxPayloadSize = tx_size;
		if(result == AcsipProtocol_OK) {
			Console_Output_String( "Join the AcsipLoraNet SUCCESSED!\r\n" );
		} else {
			Console_Output_String( "Join the AcsipLoraNet FAILED!\r\n" );
		}
		// 或是存下來或是透過藍芽傳出去
		break;

	case FrameFlag_Poll:
		if(LoraGateWay != NULL) {
			// Console_Output_String( "error6\r\n" );		// test output
      
			/* Fix by JC in 20170711 */
			//result = AcsipProtocol_LoraPollResponse(&RxFrame, &TxFrame, LoraTxBuffer, &tx_size);
      if((LoRaSettings.HybirdHoppingGo == true) && (LoRaSettings.FreqHopOn == 0)) {
        result = AcsipProtocol_LoraPollResponse_example(&RxFrame, &TxFrame, LoraTxBuffer, &tx_size, LoRaSettings.HybirdHoppingDataPayloadSize);
      } else {
        result = AcsipProtocol_LoraPollResponse(&RxFrame, &TxFrame, LoraTxBuffer, &tx_size);
      }
      /* Fix end */
      
			// snprintf( (char *)str, sizeof(str), "%02x", result);		//test output
			// Console_Output_String( "result=" );			//test output
			// Console_Output_String( (const char *)str );		//test output
			// Console_Output_String( "\r\n" );			//test output
			LoraTxPayloadSize = tx_size;
			if(result == AcsipProtocol_OK) {
				Slave_PollEvent = true;
			} else {
				Slave_PollEvent = false;
			}
		}
		break;

	case FrameFlag_Data:
		if(LoraGateWay != NULL) {
			result = AcsipProtocol_LoraDataResponse(&RxFrame, TxDataArray, &TxDataSize, &TxFrame, LoraTxBuffer, &tx_size);
			LoraTxPayloadSize = tx_size;
			LoraNeedTxData = false;
			TxDataSize = 0;
			memset((void *)TxDataArray, 0, MaxMsgDataSize);
		}
		break;

	case FrameFlag_Leave:
		if(LoraGateWay != NULL) {
			result = AcsipProtocol_LoraLeaveNetworkResponse(&RxFrame, true, &TxFrame, LoraTxBuffer, &tx_size);
			LoraTxPayloadSize = tx_size;

			if(result == AcsipProtocol_OK) {
				Console_Output_String( "Leave the AcsipLoraNet SUCCESSED!\r\n" );
				// Console_Output_String( "NormalSlaveLeave\r\n" );			// for test
			} else {
				Console_Output_String( "Leave the AcsipLoraNet FAILED!\r\n" );
			}
			// 或是存下來或是透過藍芽傳出去
		}
		break;

	case FrameFlag_Interval:
		if(LoraGateWay != NULL) {
			result = AcsipProtocol_LoraIntervalResponse(&RxFrame, &TxFrame, LoraTxBuffer, &tx_size);
			LoraTxPayloadSize = tx_size;

			// Console_Output_String( "NormalSlave_Interval\r\n" );		// test output
			if(result == AcsipProtocol_OK) {
				Console_Output_String( "Interval setting SUCCESSED!\r\n" );
				SLEEP_SlaveSleepVariableSet();
			} else {
				Console_Output_String( "Interval setting FAILED!\r\n" );
			}
		}
		break;

	default:
		break;
	}
	return result;
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
