
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

#include "config.h"
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
extern __IO uint16_t			Running_TimeCount;			// for MASTER & SLAVE
extern uint8_t				LoraNodeCount;				// for MASTER
extern tLoraDeviceNode *		LoraGateWay;				// for SLAVE
extern tDeviceNodeSensor *		MySensor;				// for SLAVE

extern __IO uint8_t			Running_HoppingStartChannel;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void		NormalMaster_RecordNodeRandomHoppingChannel( const uint8_t *NodeAddr, uint8_t Channel );


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
	static uint8_t		base64_data[((MaxMsgDataSize/3)*4)];

	tDevice_Information *	ptr;
	size_t			dsize;
	uint8_t			result;
	char			str[64];
	int8_t			count;

	result = AcsipProtocol_NoAction;
	ptr = & Device_Information[ Event->RunNodeNumber ];

	switch( Event->RunNodeEvent ) {
	case Master_AcsipProtocol_Broadcast:
		break;

	case Master_AcsipProtocol_Join:
		// result = AcsipProtocol_LoraJoinNetworkResponseCB(&RxFrame, TxFrame.NodeAddress);
		result = AcsipProtocol_LoraJoinNetworkResponseCB( & RxFrame, & TxFrame );
		Console_Output_String( "Node=" );
		for(count = 2 ; count >= 0 ; count--) {
			snprintf( str, sizeof(str), "%02x", Event->RunNodeAddr[count] );
			Console_Output_String( str );
		}
		if( result == AcsipProtocol_OK ) {
			NormalMaster_RecordNodeRandomHoppingChannel( (uint8_t *) Event->RunNodeAddr, TxFrame.LoraRF_NextChannel );
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
			snprintf( str, sizeof(str), "%d, result = %d \r\n", __LINE__, result );
			Console_Output_String( str );
		}
		//或是存下來或是透過藍芽傳出去
		break;

	case Master_AcsipProtocol_Poll:
		if( ptr->Flag_Valid ) {
			// result = AcsipProtocol_LoraPollResponseCB(LoraNodeDevice[Event->RunNodeNumber], DeviceNodeSensor[Event->RunNodeNumber], &RxFrame);
			
      /* Fix by JC in 20170711 */
			//result = AcsipProtocol_LoraPollResponseCB( & ptr->Node_MAC, & ptr->Node_Sensor, & RxFrame, & TxFrame );
      if((LoRaSettings.HybirdHoppingGo == true) && (LoRaSettings.FreqHopOn == 0)) {
        result = AcsipProtocol_LoraPollResponseCB_example( & ptr->Node_MAC, & ptr->Node_Sensor, & RxFrame, & TxFrame );
      } else {
        result = AcsipProtocol_LoraPollResponseCB( & ptr->Node_MAC, & ptr->Node_Sensor, & RxFrame, & TxFrame );
      }
      /* Fix end */

			if( result == AcsipProtocol_OK ) {
				NormalMaster_RecordNodeRandomHoppingChannel( (uint8_t *) Event->RunNodeAddr, TxFrame.LoraRF_NextChannel );
//				ptr->Node_Sleep_Hop.Last_LoraHoppingStartChannel = -1;

				if( ptr->Node_Sensor.UTC && ptr->Node_MAC.Interval ) {
					ptr->Node_Sleep_Hop.isNowSleeping = true;
					ptr->Node_Sleep_Hop.WakeUpTimePoint = (uint16_t)ceil(((double) LoRaSettings.TxPacketTimeout) * 0.001 );
					ptr->Node_Sleep_Hop.WakeUpTimePoint += (Running_TimeCount + ptr->Node_MAC.Interval);
				}

				Console_Output_String( "Node=" );
				for(count = 2 ; count >= 0 ; count--) {
					snprintf( str, sizeof(str), "%02x", Event->RunNodeAddr[count] );
					Console_Output_String( str );
				}

#ifdef Board__A22_Tracker
				Console_Output_String( " EVT=GPS " );
#else
				Console_Output_String( " EVT=Poll " );
#endif

				snprintf( str, sizeof(str), "%d", ptr->Node_Sensor.GPS_Latitude );
				Console_Output_String( str );
				Console_Output_String( " " );
				snprintf( str, sizeof(str), "%d", ptr->Node_Sensor.GPS_Longitude );
				Console_Output_String( str );
				Console_Output_String( " " );
//				snprintf( str, sizeof(str), "%u", ptr->Node_Sensor.UTC );
//				Console_Output_String( str );
//				Console_Output_String( " " );
				snprintf( str, sizeof(str), "%u", ptr->Node_Sensor.Battery );
				Console_Output_String( str );
				Console_Output_String( " " );

				if( LoRaSettings.FreqHopOn ){
					snprintf( str, sizeof(str), ", Freq= %d ", LoRaSettings.Channel_List[Running_HoppingStartChannel] / 1000 );
					Console_Output_String( str );
				} else {
					snprintf( str, sizeof(str), ", Freq= %d ", LoRaSettings.RFFrequency / 1000 );
					Console_Output_String( str );
				}

				snprintf( str, sizeof(str), ", SNR= %d ", ptr->Node_Sensor.Packet_SNR );
				Console_Output_String( str );

				snprintf( str, sizeof(str), ", RSSI= %3.1f ", ptr->Node_Sensor.RSSI );
				Console_Output_String( str );
				Console_Output_String( "\r\n" );
				// 或是存下來或是透過藍芽傳出去
			}
		}
		break;

	case Master_AcsipProtocol_Data:
		if( ptr->Flag_Valid ) {
			// result = AcsipProtocol_LoraDataResponseCB(LoraNodeDevice[Event->RunNodeNumber], &RxFrame);
			result = AcsipProtocol_LoraDataResponseCB( & ptr->Node_MAC, & RxFrame, & TxFrame );
			if(result == AcsipProtocol_OK) {
				NormalMaster_RecordNodeRandomHoppingChannel( (uint8_t *) Event->RunNodeAddr, TxFrame.LoraRF_NextChannel );
//				ptr->Node_Sleep_Hop.Last_LoraHoppingStartChannel = -1;

				if(RxFrame.MsgLength > 0) {
					memset((void *)base64_data, 0, ((MaxMsgDataSize/3)*4));
					if(Base64_encode( base64_data, ((MaxMsgDataSize/3)*4), &dsize, (const uint8_t *) RxFrame.MsgData, RxFrame.MsgLength ) == 0 ) {
						Console_Output_String( "Node=" );
						for(count = 2 ; count >= 0 ; count--) {
							snprintf( str, sizeof(str), "%02x", Event->RunNodeAddr[count] );
							Console_Output_String( str );
						}
						Console_Output_String( "  DataLength=" );
						snprintf( str, sizeof(str), "%u", dsize );
						Console_Output_String( str );
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
		if( ptr->Flag_Valid ) {
			result = AcsipProtocol_LoraLeaveNetworkResponseCB( & ptr->Node_MAC, & RxFrame );
			Console_Output_String( "Node=" );
			for(count = 2 ; count >= 0 ; count--) {
				snprintf( str, sizeof(str), "%02x", Event->RunNodeAddr[count] );
				Console_Output_String( str );
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
		if( ptr->Flag_Valid ) {
			// result = AcsipProtocol_LoraIntervalResponseCB(LoraNodeDevice[Event->RunNodeNumber], &RxFrame);
			result = AcsipProtocol_LoraIntervalResponseCB( & ptr->Node_MAC, & RxFrame, & TxFrame );
			Console_Output_String( "Node=" );
			for(count = 2 ; count >= 0 ; count--) {
				snprintf( str, sizeof(str), "%02x", Event->RunNodeAddr[count] );
				Console_Output_String( str );
			}
			// Console_Output_String( "NormalMaster_Interval\r\n" );		// test output
			if(result == AcsipProtocol_OK) {
				NormalMaster_RecordNodeRandomHoppingChannel( (uint8_t *) Event->RunNodeAddr, TxFrame.LoraRF_NextChannel );
//				ptr->Node_Sleep_Hop.Last_LoraHoppingStartChannel = -1;
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
void		NormalMaster_RecordNodeRandomHoppingChannel( const uint8_t *NodeAddr, uint8_t Channel )
{
	int16_t				i;
	tDeviceNodeSleepAndRandomHop *	p;

	i = AcsipProtocol__Find_Node( (uint8_t *) NodeAddr );

	if( 0 <= i && i < MAX_LoraNodeNum ) {
		p = & Device_Information[i].Node_Sleep_Hop;

		p->HoppingChannel_Last = p->HoppingChannel;
		p->HoppingChannel_Next = -1;
		p->HoppingChannel = Channel;
	}
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
