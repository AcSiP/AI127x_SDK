/**
  ******************************************************************************
  * @file    Project/ARM-Lora/normalSlave.c 
  * @author  JC
  * @version 
  * @date    
  * @brief   normalSlave.c program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

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
#include "radio.h"
#include "acsip_protocol.h"
#include "main.h"
#include "normalSlave.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO bool Slave_PollEvent = false;
extern __IO bool LoraNeedTxData;
extern __IO size_t LoraTxPayloadSize;
extern __IO size_t LoraRxPayloadSize;
extern uint8_t LoraTxBuffer[LoraBufferLength];
extern uint8_t LoraRxBuffer[LoraBufferLength];
extern __IO tAcsipProtocolFrame TxFrame;
extern __IO tAcsipProtocolFrame RxFrame;
extern tLoraDeviceNode *LoraNodeDevice[MAX_LoraNodeNum];  //for MASTER
extern tDeviceNodeSensor *DeviceNodeSensor[MAX_LoraNodeNum];  //for MASTER
extern uint8_t LoraNodeCount;  //for MASTER
extern tLoraDeviceNode *LoraGateWay;  //for SLAVE
extern tDeviceNodeSensor *MySensor;  //for SLAVE
extern uint8_t TxDataArray[MaxMsgDataSize];
extern uint8_t TxDataSize;

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
uint8_t NormalSlave(void) {
  
  uint8_t result, tx_size;
  //int8_t str[10];    //test output
  
  result = AcsipProtocol_NoAction;
  
  switch(RxFrame.FrameFlag) {
    case FrameFlag_Join:
      result = AcsipProtocol_LoraJoinNetworkResponse(&RxFrame, true, &TxFrame, LoraTxBuffer, &tx_size);
      LoraTxPayloadSize = tx_size;
      if(result == AcsipProtocol_OK) {
        CmdUART_UartWrite((uint8_t *)"Join the AcsipLoraNet SUCCESSED!\r\n", strlen("Join the AcsipLoraNet SUCCESSED!\r\n"));
      } else {
        CmdUART_UartWrite((uint8_t *)"Join the AcsipLoraNet FAILED!\r\n", strlen("Join the AcsipLoraNet FAILED!\r\n"));
      }
      //或是存下來或是透過藍芽傳出去
      break;
    case FrameFlag_Poll:
      if(LoraGateWay != NULL) {
        //CmdUART_UartWrite((uint8_t *)"error6\r\n", strlen("error6\r\n"));    //test output
        result = AcsipProtocol_LoraPollResponse(&RxFrame, &TxFrame, LoraTxBuffer, &tx_size);
        //sprintf((char *)str, "%02x", result);    //test output
        //CmdUART_UartWrite((uint8_t *)"result=", strlen("result="));    //test output
        //CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));    //test output
        //CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));    //test output
        LoraTxPayloadSize = tx_size;
        if((LoraGateWay->Interval != 0) && (result == AcsipProtocol_OK)) {
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
          CmdUART_UartWrite((uint8_t *)"Leave the AcsipLoraNet SUCCESSED!\r\n", strlen("Leave the AcsipLoraNet SUCCESSED!\r\n"));
          //CmdUART_UartWrite((uint8_t *)"NormalSlaveLeave\r\n", strlen("NormalSlaveLeave\r\n"));     // for test
        } else {
          CmdUART_UartWrite((uint8_t *)"Leave the AcsipLoraNet FAILED!\r\n", strlen("Leave the AcsipLoraNet FAILED!\r\n"));
        }
        //或是存下來或是透過藍芽傳出去
      }
      break;
    case FrameFlag_Interval:
      if(LoraGateWay != NULL) {
        result = AcsipProtocol_LoraIntervalResponse(&RxFrame, &TxFrame, LoraTxBuffer, &tx_size);
        LoraTxPayloadSize = tx_size;
        
        //CmdUART_UartWrite((uint8_t *)"NormalSlave_Interval\r\n", strlen("NormalSlave_Interval\r\n"));    //test output
        if(result == AcsipProtocol_OK) {
          CmdUART_UartWrite((uint8_t *)"Interval setting SUCCESSED!\r\n", strlen("Interval setting SUCCESSED!\r\n"));
          SLEEP_SlaveSleepVariableSet();
        } else {
          CmdUART_UartWrite((uint8_t *)"Interval setting FAILED!\r\n", strlen("Interval setting FAILED!\r\n"));
        }
      }
      break;
    default:
      break;
  }
  
  return result;
  
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


