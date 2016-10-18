/**
  ******************************************************************************
  * @file    Project/ARM-Lora/normalMaster.c 
  * @author  JC
  * @version 
  * @date    
  * @brief   normalMaster.c program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

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
extern tLoRaSettings	LoRaSettings;
extern __IO tAcsipProtocolFrame TxFrame;
extern __IO tAcsipProtocolFrame RxFrame;
extern __IO uint16_t Running_TimeCount;                   //for MASTER & SLAVE
extern tLoraDeviceNode *LoraNodeDevice[MAX_LoraNodeNum];  //for MASTER
extern tDeviceNodeSleepAndRandomHop *DeviceNodeSleepAndRandomHop[MAX_LoraNodeNum];  //for MASTER
extern tDeviceNodeSensor *DeviceNodeSensor[MAX_LoraNodeNum];  //for MASTER
extern uint8_t LoraNodeCount;  //for MASTER
extern tLoraDeviceNode *LoraGateWay;  //for SLAVE
extern tDeviceNodeSensor *MySensor;  //for SLAVE

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



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
uint8_t NormalMaster(__IO tLoraRunningEvent *Event) {
  
  static uint8_t base64_data[((MaxMsgDataSize/3)*4)];
  size_t dsize;
  uint8_t result;
  int8_t str[10];
  int8_t count;
  
  result = AcsipProtocol_NoAction;
  
  switch(Event->RunNodeEvent) {
    case Master_AcsipProtocol_Broadcast:
      break;
    case Master_AcsipProtocol_Join:
      result = AcsipProtocol_LoraJoinNetworkResponseCB(&RxFrame, TxFrame.NodeAddress);
      CmdUART_UartWrite((uint8_t *)"Node=", strlen("Node="));
      for(count = 2 ; count >= 0 ; count--) {
        sprintf((char *)str, "%02x", Event->RunNodeAddr[count]);
        CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
      }
      if(result == AcsipProtocol_OK) {
        CmdUART_UartWrite((uint8_t *)" EVT=JoinOK\r\n", strlen(" EVT=JoinOK\r\n"));
        //CmdUART_UartWrite((uint8_t *)"Join the AcsipLoraNet SUCCESSED!\r\n", strlen("Join the AcsipLoraNet SUCCESSED!\r\n"));
#ifdef STM32F401xx
        SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara();
        SaveRecord_WriteInLoraMode();
#endif
#ifdef STM32F072
        SaveRecord_WriteInLoraNodePara();
#endif
      } else {
        CmdUART_UartWrite((uint8_t *)" EVT=JoinFAILED\r\n", strlen(" EVT=JoinFAILED\r\n"));
        //CmdUART_UartWrite((uint8_t *)"Join the AcsipLoraNet FAILED!\r\n", strlen("Join the AcsipLoraNet FAILED!\r\n"));
      }
      //或是存下來或是透過藍芽傳出去
      break;
    case Master_AcsipProtocol_Poll:
      if(LoraNodeDevice[Event->RunNodeNumber]->NodeAddress != NULL) {
        result = AcsipProtocol_LoraPollResponseCB(LoraNodeDevice[Event->RunNodeNumber], DeviceNodeSensor[Event->RunNodeNumber], &RxFrame);
        if((result == AcsipProtocol_OK) && (DeviceNodeSensor[Event->RunNodeNumber] != NULL)) {
          
          if(DeviceNodeSleepAndRandomHop[Event->RunNodeNumber] != NULL) {
            if((DeviceNodeSensor[Event->RunNodeNumber]->UTC != 0) && (LoraNodeDevice[Event->RunNodeNumber]->Interval != 0)) {
              DeviceNodeSleepAndRandomHop[Event->RunNodeNumber]->isNowSleeping = true;
              DeviceNodeSleepAndRandomHop[Event->RunNodeNumber]->WakeUpTimePoint = (uint16_t)ceil(((double)LoRaSettings.TxPacketTimeout) * 0.001);
              DeviceNodeSleepAndRandomHop[Event->RunNodeNumber]->WakeUpTimePoint += (Running_TimeCount + LoraNodeDevice[Event->RunNodeNumber]->Interval);
            }
          }
          
          CmdUART_UartWrite((uint8_t *)"Node=", strlen("Node="));
          for(count = 2 ; count >= 0 ; count--) {
            sprintf((char *)str, "%02x", Event->RunNodeAddr[count]);
            CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
          }
          CmdUART_UartWrite((uint8_t *)" EVT=GPS ", strlen(" EVT=GPS "));
          sprintf((char *)str, "%d", DeviceNodeSensor[Event->RunNodeNumber]->GPS_Latitude);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
          CmdUART_UartWrite((uint8_t *)" ", 1);
          sprintf((char *)str, "%d", DeviceNodeSensor[Event->RunNodeNumber]->GPS_Longitude);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
          CmdUART_UartWrite((uint8_t *)" ", 1);
          sprintf((char *)str, "%u", DeviceNodeSensor[Event->RunNodeNumber]->UTC);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
          CmdUART_UartWrite((uint8_t *)" ", 1);
          sprintf((char *)str, "%u", DeviceNodeSensor[Event->RunNodeNumber]->Battery);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
          CmdUART_UartWrite((uint8_t *)" ", 1);
          sprintf((char *)str, "%3.2f", DeviceNodeSensor[Event->RunNodeNumber]->RSSI);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
          CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
          /*
          sprintf((char *)str, "%3.2f", DeviceNodeSensor[Event->RunNodeNumber]->RSSI);
          CmdUART_UartWrite((uint8_t *)"RSSI=", 5);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
          CmdUART_UartWrite((uint8_t *)"  ", 2);
          CmdUART_UartWrite((uint8_t *)"GPS-Latitude=", strlen("GPS-Latitude="));
          sprintf((char *)str, "%d", DeviceNodeSensor[Event->RunNodeNumber]->GPS_Latitude);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
          CmdUART_UartWrite((uint8_t *)"  ", 2);
          CmdUART_UartWrite((uint8_t *)"GPS-Longitude=", strlen("GPS-Longitude="));
          sprintf((char *)str, "%d", DeviceNodeSensor[Event->RunNodeNumber]->GPS_Longitude);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
          CmdUART_UartWrite((uint8_t *)"  ", 2);
          CmdUART_UartWrite((uint8_t *)"UTC-Time=", strlen("UTC-Time="));
          sprintf((char *)str, "%u", DeviceNodeSensor[Event->RunNodeNumber]->UTC);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
          CmdUART_UartWrite((uint8_t *)"  ", 2);
          CmdUART_UartWrite((uint8_t *)"Battery=", strlen("Battery="));
          sprintf((char *)str, "%u", DeviceNodeSensor[Event->RunNodeNumber]->Battery);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
          CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
          */
          //或是存下來或是透過藍芽傳出去
        }
      }
      break;
    case Master_AcsipProtocol_Data:
      if(LoraNodeDevice[Event->RunNodeNumber]->NodeAddress != NULL) {
        result = AcsipProtocol_LoraDataResponseCB(LoraNodeDevice[Event->RunNodeNumber], &RxFrame);
        if((result == AcsipProtocol_OK) && (RxFrame.MsgLength > 0)) {
          memset((void *)base64_data, 0, ((MaxMsgDataSize/3)*4));
          if(Base64_encode( base64_data, ((MaxMsgDataSize/3)*4), &dsize, (const uint8_t	*)RxFrame.MsgData, RxFrame.MsgLength ) == 0) {
            CmdUART_UartWrite((uint8_t *)"Node=", strlen("Node="));
            for(count = 2 ; count >= 0 ; count--) {
              sprintf((char *)str, "%02x", Event->RunNodeAddr[count]);
              CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
            }
            CmdUART_UartWrite((uint8_t *)"  DataLength=", strlen("  DataLength="));
            sprintf((char *)str, "%u", dsize);
            CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
            CmdUART_UartWrite((uint8_t *)"  Data=", strlen("  Data="));
            CmdUART_UartWrite(base64_data, dsize);
            CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
            //CmdUART_UartWrite((uint8_t *)"  ", 2);
            //CmdUART_UartWrite((uint8_t *)"DataLength=", strlen("DataLength="));
            //sprintf((char *)str, "%u", RxFrame.MsgLength);
            //CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
            //CmdUART_UartWrite((uint8_t *)"  ", 2);
            //CmdUART_UartWrite((uint8_t *)"Data=", strlen("Data="));
            //CmdUART_UartWrite((uint8_t *)RxFrame.MsgData, RxFrame.MsgLength);
            //CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
            //或是存下來或是透過藍芽傳出去
          }
        }
      }
      break;
    case Master_AcsipProtocol_Leave:
      if(LoraNodeDevice[Event->RunNodeNumber]->NodeAddress != NULL) {
        result = AcsipProtocol_LoraLeaveNetworkResponseCB(LoraNodeDevice[Event->RunNodeNumber], &RxFrame);
        CmdUART_UartWrite((uint8_t *)"Node=", strlen("Node="));
        for(count = 2 ; count >= 0 ; count--) {
          sprintf((char *)str, "%02x", Event->RunNodeAddr[count]);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
        }
        if(result == AcsipProtocol_OK) {
          CmdUART_UartWrite((uint8_t *)" EVT=LeaveOK\r\n", strlen(" EVT=LeaveOK\r\n"));
          //CmdUART_UartWrite((uint8_t *)"Leave the AcsipLoraNet SUCCESSED!\r\n", strlen("Leave the AcsipLoraNet SUCCESSED!\r\n"));
#ifdef STM32F401xx
          SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara();
          SaveRecord_WriteInLoraMode();
#endif
#ifdef STM32F072
          SaveRecord_WriteInLoraNodePara();
#endif
        } else {
          CmdUART_UartWrite((uint8_t *)" EVT=LeaveFAILED\r\n", strlen(" EVT=LeaveFAILED\r\n"));
          //CmdUART_UartWrite((uint8_t *)"Leave the AcsipLoraNet FAILED!\r\n", strlen("Leave the AcsipLoraNet FAILED!\r\n"));
        }
        //或是存下來或是透過藍芽傳出去
      }
      break;
    case Master_AcsipProtocol_Interval:
      if(LoraNodeDevice[Event->RunNodeNumber]->NodeAddress != NULL) {
        result = AcsipProtocol_LoraIntervalResponseCB(LoraNodeDevice[Event->RunNodeNumber], &RxFrame);
        CmdUART_UartWrite((uint8_t *)"Node=", strlen("Node="));
        for(count = 2 ; count >= 0 ; count--) {
          sprintf((char *)str, "%02x", Event->RunNodeAddr[count]);
          CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
        }
        //CmdUART_UartWrite((uint8_t *)"NormalMaster_Interval\r\n", strlen("NormalMaster_Interval\r\n"));    //test output
        if(result == AcsipProtocol_OK) {
          CmdUART_UartWrite((uint8_t *)" EVT=SetNodeOK\r\n", strlen(" EVT=SetNodeOK\r\n"));
#ifdef STM32F401xx
          SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara();
          SaveRecord_WriteInLoraMode();
#endif
#ifdef STM32F072
          SaveRecord_WriteInLoraNodePara();
#endif
        } else {
          CmdUART_UartWrite((uint8_t *)" EVT=SetNodeFAILED\r\n", strlen(" EVT=SetNodeFAILED\r\n"));
          //CmdUART_UartWrite((uint8_t *)" Interval setting FAILED!\r\n", strlen(" Interval setting FAILED!\r\n"));
        }
      }
      break;
    default:
      break;
  }
  
  return result;
  
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


