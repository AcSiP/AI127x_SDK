
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
#include <math.h>
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
#include "radio.h"
#include "acsip_protocol.h"
#include "main.h"
#include "normalMaster.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern tLoRaSettings			LoRaSettings;
extern __IO tAcsipProtocolFrame		TxFrame;
extern __IO tAcsipProtocolFrame		RxFrame;
extern __IO uint16_t			Running_TimeCount;					// for MASTER & SLAVE
extern tLoraDeviceNode *		LoraNodeDevice[MAX_LoraNodeNum];			// for MASTER
extern tDeviceNodeSleepAndRandomHop *	DeviceNodeSleepAndRandomHop[MAX_LoraNodeNum];		// for MASTER
extern tDeviceNodeSensor *		DeviceNodeSensor[MAX_LoraNodeNum];			// for MASTER
extern uint8_t				LoraNodeCount;						// for MASTER
extern tLoraDeviceNode *		LoraGateWay;						// for SLAVE
extern tDeviceNodeSensor *		MySensor;						// for SLAVE

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void	NormalMaster_RecordNodeRandomHoppingChannel(__IO uint8_t *, __IO uint8_t *);


/***************************************************************************************************
 *  Function Name: NormalMaster
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
uint8_t		NormalMaster(__IO tLoraRunningEvent *Event)
{
	static uint8_t	base64_data[((MaxMsgDataSize/3)*4)];
	size_t		dsize;
	uint8_t		result;
	int8_t		str[10];
	int8_t		count;

	result = AcsipProtocol_NoAction;

	switch(Event->RunNodeEvent) {
	case Master_AcsipProtocol_Broadcast:
		break;

	case Master_AcsipProtocol_Join:
		// result = AcsipProtocol_LoraJoinNetworkResponseCB(&RxFrame, TxFrame.NodeAddress);
		result = AcsipProtocol_LoraJoinNetworkResponseCB(&RxFrame, &TxFrame);
		Console_Output_String( "Node=" );
		for(count = 2 ; count >= 0 ; count--) {
			snprintf( (char *)str, sizeof(str), "%02x", Event->RunNodeAddr[count] );
			Console_Output_String( (const char *)str );
		}
		if(result == AcsipProtocol_OK) {
			// NormalMaster_RecordNodeRandomHoppingChannel(&(Event->RunNodeNumber), &(TxFrame.LoraRF_NextChannel));
			NormalMaster_RecordNodeRandomHoppingChannel(Event->RunNodeAddr, &(TxFrame.LoraRF_NextChannel));
			Console_Output_String( " EVT=JoinOK\r\n" );
			// Console_Output_String( "Join the AcsipLoraNet SUCCESSED!\r\n" );
#ifdef STM32F401xx
			SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara();
			SaveRecord_WriteInLoraMode();
#endif
#ifdef STM32F072
			SaveRecord_WriteInLoraNodePara();
#endif

		} else {
			Console_Output_String( " EVT=JoinFAILED\r\n" );
			// Console_Output_String( "Join the AcsipLoraNet FAILED!\r\n" );
		}
		//或是存下來或是透過藍芽傳出去
		break;

	case Master_AcsipProtocol_Poll:
		if(LoraNodeDevice[Event->RunNodeNumber]->NodeAddress != NULL) {
			// result = AcsipProtocol_LoraPollResponseCB(LoraNodeDevice[Event->RunNodeNumber], DeviceNodeSensor[Event->RunNodeNumber], &RxFrame);
			result = AcsipProtocol_LoraPollResponseCB(LoraNodeDevice[Event->RunNodeNumber], DeviceNodeSensor[Event->RunNodeNumber], &RxFrame, &TxFrame);

			if((result == AcsipProtocol_OK) && (DeviceNodeSensor[Event->RunNodeNumber] != NULL)) {
				// NormalMaster_RecordNodeRandomHoppingChannel(&(Event->RunNodeNumber), &(TxFrame.LoraRF_NextChannel));
				NormalMaster_RecordNodeRandomHoppingChannel(Event->RunNodeAddr, &(TxFrame.LoraRF_NextChannel));
				if(DeviceNodeSleepAndRandomHop[Event->RunNodeNumber] != NULL) {
					if((DeviceNodeSensor[Event->RunNodeNumber]->UTC != 0) && (LoraNodeDevice[Event->RunNodeNumber]->Interval != 0)) {
						DeviceNodeSleepAndRandomHop[Event->RunNodeNumber]->isNowSleeping = true;
						DeviceNodeSleepAndRandomHop[Event->RunNodeNumber]->WakeUpTimePoint = (uint16_t)ceil(((double)LoRaSettings.TxPacketTimeout) * 0.001);
						DeviceNodeSleepAndRandomHop[Event->RunNodeNumber]->WakeUpTimePoint += (Running_TimeCount + LoraNodeDevice[Event->RunNodeNumber]->Interval);
					}
				}

				Console_Output_String( "Node=" );
				for(count = 2 ; count >= 0 ; count--) {
					snprintf( (char *)str, sizeof(str), "%02x", Event->RunNodeAddr[count] );
					Console_Output_String( (const char *)str );
				}

#ifdef Board__A22_Tracker
				Console_Output_String( " EVT=GPS " );
				snprintf( (char *)str, sizeof(str), "%d", DeviceNodeSensor[Event->RunNodeNumber]->GPS_Latitude );
				Console_Output_String( (const char *)str );
				Console_Output_String( " " );
				snprintf( (char *)str, sizeof(str), "%d", DeviceNodeSensor[Event->RunNodeNumber]->GPS_Longitude );
				Console_Output_String( (const char *)str );
				Console_Output_String( " " );
				snprintf( (char *)str, sizeof(str), "%u", DeviceNodeSensor[Event->RunNodeNumber]->UTC );
				Console_Output_String( (const char *)str );
				Console_Output_String( " " );
				snprintf( (char *)str, sizeof(str), "%u", DeviceNodeSensor[Event->RunNodeNumber]->Battery );
				Console_Output_String( (const char *)str );
				Console_Output_String( " " );
#else
				Console_Output_String( " EVT=Poll " );
#endif

				snprintf( (char *)str, sizeof(str), "%3.2f", DeviceNodeSensor[Event->RunNodeNumber]->RSSI );
				Console_Output_String( (const char *)str );
				Console_Output_String( "\r\n" );
				// 或是存下來或是透過藍芽傳出去
			}
		}
		break;

	case Master_AcsipProtocol_Data:
		if(LoraNodeDevice[Event->RunNodeNumber]->NodeAddress != NULL) {
			// result = AcsipProtocol_LoraDataResponseCB(LoraNodeDevice[Event->RunNodeNumber], &RxFrame);
			result = AcsipProtocol_LoraDataResponseCB(LoraNodeDevice[Event->RunNodeNumber], &RxFrame, &TxFrame);
			if(result == AcsipProtocol_OK) {
				// NormalMaster_RecordNodeRandomHoppingChannel(&(Event->RunNodeNumber), &(TxFrame.LoraRF_NextChannel));
				NormalMaster_RecordNodeRandomHoppingChannel(Event->RunNodeAddr, &(TxFrame.LoraRF_NextChannel));
				if(RxFrame.MsgLength > 0) {
					memset((void *)base64_data, 0, ((MaxMsgDataSize/3)*4));
					if(Base64_encode( base64_data, ((MaxMsgDataSize/3)*4), &dsize, (const uint8_t	*)RxFrame.MsgData, RxFrame.MsgLength ) == 0) {
						Console_Output_String( "Node=" );
						for(count = 2 ; count >= 0 ; count--) {
							snprintf( (char *)str, sizeof(str), "%02x", Event->RunNodeAddr[count] );
							Console_Output_String( (const char *)str );
						}
						Console_Output_String( "  DataLength=" );
						snprintf( (char *)str, sizeof(str), "%u", dsize );
						Console_Output_String( (const char *)str );
						Console_Output_String( "  Data=" );
						Console_Write( base64_data, dsize );
						Console_Output_String( "\r\n" );
						//或是存下來或是透過藍芽傳出去
					}
				}
			}
		}
		break;

	case Master_AcsipProtocol_Leave:
		if(LoraNodeDevice[Event->RunNodeNumber]->NodeAddress != NULL) {
			result = AcsipProtocol_LoraLeaveNetworkResponseCB(LoraNodeDevice[Event->RunNodeNumber], &RxFrame);
			Console_Output_String( "Node=" );
			for(count = 2 ; count >= 0 ; count--) {
				snprintf( (char *)str, sizeof(str), "%02x", Event->RunNodeAddr[count] );
				Console_Output_String( (const char *)str );
			}

			if(result == AcsipProtocol_OK) {
				Console_Output_String( " EVT=LeaveOK\r\n" );
				// Console_Output_String( "Leave the AcsipLoraNet SUCCESSED!\r\n" );

#ifdef STM32F401xx
				SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara();
				SaveRecord_WriteInLoraMode();
#endif

#ifdef STM32F072
				SaveRecord_WriteInLoraNodePara();
#endif

			} else {
				Console_Output_String( " EVT=LeaveFAILED\r\n" );
				// Console_Output_String( "Leave the AcsipLoraNet FAILED!\r\n" );
			}
			// 或是存下來或是透過藍芽傳出去
		}
		break;

	case Master_AcsipProtocol_Interval:
		if(LoraNodeDevice[Event->RunNodeNumber]->NodeAddress != NULL) {
			// result = AcsipProtocol_LoraIntervalResponseCB(LoraNodeDevice[Event->RunNodeNumber], &RxFrame);
			result = AcsipProtocol_LoraIntervalResponseCB(LoraNodeDevice[Event->RunNodeNumber], &RxFrame, &TxFrame);
			Console_Output_String( "Node=" );
			for(count = 2 ; count >= 0 ; count--) {
				snprintf( (char *)str, sizeof(str), "%02x", Event->RunNodeAddr[count] );
				Console_Output_String( (const char *)str );
			}
			// Console_Output_String( "NormalMaster_Interval\r\n" );		// test output
			if(result == AcsipProtocol_OK) {
				// NormalMaster_RecordNodeRandomHoppingChannel(&(Event->RunNodeNumber), &(TxFrame.LoraRF_NextChannel));
				NormalMaster_RecordNodeRandomHoppingChannel(Event->RunNodeAddr, &(TxFrame.LoraRF_NextChannel));
				Console_Output_String( " EVT=SetNodeOK\r\n" );

#ifdef STM32F401xx
				SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara();
				SaveRecord_WriteInLoraMode();
#endif

#ifdef STM32F072
				SaveRecord_WriteInLoraNodePara();
#endif

			} else {
				Console_Output_String( " EVT=SetNodeFAILED\r\n" );
				// Console_Output_String( " Interval setting FAILED!\r\n" );
			}
		}
		break;

	default:
		break;
	}

	return result;
}



/***************************************************************************************************
 *  Function Name: NormalMaster_RecordNodeRandomHoppingChannel
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
static void	NormalMaster_RecordNodeRandomHoppingChannel(__IO uint8_t *NodeAddr, __IO uint8_t *Channel)
{
	uint8_t		count;

	for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
		if(LoraNodeDevice[count] != NULL) {
			if((LoraNodeDevice[count]->NodeAddress[0] == NodeAddr[0]) && (LoraNodeDevice[count]->NodeAddress[1] == NodeAddr[1]) && (LoraNodeDevice[count]->NodeAddress[2] == NodeAddr[2])) break;
		}
	}

	if(count < MAX_LoraNodeNum) {
		if(DeviceNodeSleepAndRandomHop[count] != NULL) DeviceNodeSleepAndRandomHop[count]->LoraHoppingStartChannel = *Channel;
	}

	// if(DeviceNodeSleepAndRandomHop[*NodeNumber] != NULL) DeviceNodeSleepAndRandomHop[*NodeNumber]->LoraHoppingStartChannel = *Channel;
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
