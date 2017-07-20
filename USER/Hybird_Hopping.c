
//---------------------------------------------------------------------------
/*
//==========================================
// Author : JC<jc@acsip.com.tw>
// Copyright 2017(C) AcSiP Technology Inc.
// 版權所有：群登科技股份有限公司
// http://www.acsip.com.tw
//==========================================
*/
//---------------------------------------------------------------------------

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "config.h"

#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

#include "Hybird_Hopping.h"
#include "main.h"
#include "UART_Console.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool HybirdHoppingChannels_2or4 = FCC_HybirdHopping_2or4Channels;      // 2:false, 4:true
static uint8_t HybirdHopping_TXcount = 0;
extern bool EnableMaster;			// 1=Master or 0=Slave selection
extern tLoRaSettings		LoRaSettings;



/***************************************************************************************************
 *  Function Name: HybirdHopping_SetBack
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void HybirdHopping_SetBack(void)
{

  LoRaSettings.Power = 20;
  LoRaSettings.PayloadLength = MaxPacketSize;
	LoRaSettings.MaxPayloadLength = MaxPacketSize;
  LoRaSettings.RxPacketTimeout = LoRaSettings.TxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
  LoRaSettings.RFFrequency = Lora_RFFrequency;
  
  LoRaSettings.HybirdHoppingMaxPayloadSize = 0;
  LoRaSettings.HybirdHoppingDataPayloadSize = 0;
  LoRaSettings.HybirdHoppingTxDelay = 0;
  
  Clear_FHSS_Channel_List();
  Load_Default_FHSS_Channel_List();
  
}



/***************************************************************************************************
 *  Function Name: HybirdHopping_Initialization
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void HybirdHopping_Initialization(bool ToDoHybirdHopping)
{
  
  if(ToDoHybirdHopping == false) {
    goto InitFailureBack;
  }
  
  Clear_FHSS_Channel_List();
  if( ! Append_FHSS_Channel_into_List( FCC_HybirdHopping_Frequency0_Hz ) ) goto InitFailureBack;
  if( ! Append_FHSS_Channel_into_List( FCC_HybirdHopping_Frequency1_Hz ) ) goto InitFailureBack;
  if(HybirdHoppingChannels_2or4) {      // If 4 channels, add another 2 channels
    if( ! Append_FHSS_Channel_into_List( FCC_HybirdHopping_Frequency2_Hz ) ) goto InitFailureBack;
    if( ! Append_FHSS_Channel_into_List( FCC_HybirdHopping_Frequency3_Hz ) ) goto InitFailureBack;
  }
  
  switch(LoRaSettings.SpreadingFactor) {
    case 7:
      if(LoRaSettings.SignalBw == 7) {
        if(HybirdHoppingChannels_2or4 == false) {      // 2:false, 4:true
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF7_BW125_Tx2times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF7_BW125_Tx2times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF7_BW125_Tx2timesDelay;
        } else {
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF7_BW125_Tx4times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF7_BW125_Tx4times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF7_BW125_Tx4timesDelay;
        }
      } else 
      if(LoRaSettings.SignalBw == 8) {
        if(HybirdHoppingChannels_2or4 == false) {      // 2:false, 4:true
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF7_BW250_Tx2times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF7_BW250_Tx2times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF7_BW250_Tx2timesDelay;
        } else {
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF7_BW250_Tx4times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF7_BW250_Tx4times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF7_BW250_Tx4timesDelay;
        }
      } else {
        goto InitFailureBack;
      }
      break;
    case 8:
      if(LoRaSettings.SignalBw == 7) {
        if(HybirdHoppingChannels_2or4 == false) {      // 2:false, 4:true
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF8_BW125_Tx2times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF8_BW125_Tx2times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF8_BW125_Tx2timesDelay;
        } else {
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF8_BW125_Tx4times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF8_BW125_Tx4times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF8_BW125_Tx4timesDelay;
        }
      } else 
      if(LoRaSettings.SignalBw == 8) {
        if(HybirdHoppingChannels_2or4 == false) {      // 2:false, 4:true
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF8_BW250_Tx2times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF8_BW250_Tx2times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF8_BW250_Tx2timesDelay;
        } else {
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF8_BW250_Tx4times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF8_BW250_Tx4times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF8_BW250_Tx4timesDelay;
        }
      } else {
        goto InitFailureBack;
      }
      break;
    case 9:
      if(LoRaSettings.SignalBw == 7) {
        if(HybirdHoppingChannels_2or4 == false) {      // 2:false, 4:true
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF9_BW125_Tx2times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF9_BW125_Tx2times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF9_BW125_Tx2timesDelay;
        } else {
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF9_BW125_Tx4times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF9_BW125_Tx4times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF9_BW125_Tx4timesDelay;
        }
      } else 
      if(LoRaSettings.SignalBw == 8) {
        if(HybirdHoppingChannels_2or4 == false) {      // 2:false, 4:true
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF9_BW250_Tx2times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF9_BW250_Tx2times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF9_BW250_Tx2timesDelay;
        } else {
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF9_BW250_Tx4times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF9_BW250_Tx4times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF9_BW250_Tx4timesDelay;
        }
      } else {
        goto InitFailureBack;
      }
      break;
    case 10:
      if(LoRaSettings.SignalBw == 8) {
        if(HybirdHoppingChannels_2or4 == false) {      // 2:false, 4:true
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF10_BW250_Tx2times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF10_BW250_Tx2times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF10_BW250_Tx2timesDelay;
        } else {
          LoRaSettings.HybirdHoppingMaxPayloadSize = MaxPacketSize_Hybird_SF10_BW250_Tx4times;
          LoRaSettings.HybirdHoppingDataPayloadSize = MsgDataSize_Hybird_SF10_BW250_Tx4times;
          LoRaSettings.HybirdHoppingTxDelay = HybirdHopping_SF10_BW250_Tx4timesDelay;
        }
      } else {
        goto InitFailureBack;
      }
      break;
    default:
      goto InitFailureBack;
  }
  
  //LoRaSettings.Power = 8;
  LoRaSettings.Power = 20;
  LoRaSettings.FreqHopOn = 0;			// [0: OFF, 1: ON]
  LoRaSettings.RFFrequency = LoRaSettings.Channel_List[0];
  
  if(LoRaSettings.Channel_Lists == 0) {
    goto InitFailureBack;
  }
  //LoRaSettings.RxPacketTimeout = LoRaSettings.TxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings) * LoRaSettings.Channel_Lists;
  LoRaSettings.RxPacketTimeout = LoRaSettings.TxPacketTimeout = 1000 * LoRaSettings.Channel_Lists;
  
  HybirdHopping_SetRXChannel();
  
  LoRaSettings.HybirdHoppingGo = true;
  return;
  
InitFailureBack:
  HybirdHopping_SetBack();
  LoRaSettings.HybirdHoppingGo = false;
  return;
  
}



/***************************************************************************************************
 *  Function Name: HybirdHopping_TX
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
bool HybirdHopping_TX(void)
{
  
  static uint8_t channel[4] = {0};
  
  if(LoRaSettings.HybirdHoppingGo == false) {
    return false;
  }
  
  if(HybirdHoppingChannels_2or4 == false) {     // 2:false, 4:true
    if(HybirdHopping_TXcount >= 2) {
      HybirdHopping_TXcount = 0;
      memset( (void *) channel, 0, 4 );
      return false;
    }
  } else {
    if(HybirdHopping_TXcount >= 4) {
      HybirdHopping_TXcount = 0;
      memset( (void *) channel, 0, 4 );
      return false;
    }
  }
  
  //SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
  
  //MASTER:3、2、1、0  SLAVE:0、1、2、3
  /*
  if(EnableMaster == 1) {       // 1=Master or 0=Slave selection
    LoRaSettings.RFFrequency = LoRaSettings.Channel_List[LoRaSettings.Channel_Lists - HybirdHopping_TXcount - 1];
    SX1276LoRaSetRFFrequency(LoRaSettings.RFFrequency);
  } else {
    LoRaSettings.RFFrequency = LoRaSettings.Channel_List[HybirdHopping_TXcount];
    SX1276LoRaSetRFFrequency(LoRaSettings.RFFrequency);
  }
  */
  
  //
  if(EnableMaster == 1) {       // 1=Master or 0=Slave selection
    if(HybirdHoppingChannels_2or4 == false) {     // 2:false, 4:true
      channel[HybirdHopping_TXcount] = LoRaSettings.Channel_Lists - HybirdHopping_TXcount - 1;
    } else {
      switch(HybirdHopping_TXcount) {
        case 0:
          channel[0] = (rand() % 3) + 1;
          break;
        case 1:
          channel[1] = (rand() % 3) + 1;
          while(1) {
            if(channel[0] != channel[1]) {
              break;
            } else {
              channel[1] = (rand() % 3) + 1;
            }
          }
          break;
        case 2:
          channel[2] = 6 - channel[0] - channel[1];
          break;
        case 3:
          channel[3] = 0;
          break;
        default:
          return false;
      }
    }
  } else {
    if(HybirdHoppingChannels_2or4 == false) {     // 2:false, 4:true
      channel[HybirdHopping_TXcount] = HybirdHopping_TXcount;
    } else {
      switch(HybirdHopping_TXcount) {
        case 0:
          channel[0] = rand() % 4;
          break;
        case 1:
          channel[1] = rand() % 4;
          while(1) {
            if(channel[0] != channel[1]) {
              break;
            } else {
              channel[1] = rand() % 4;
            }
          }
          break;
        case 2:
          channel[2] = rand() % 4;
          while(1) {
            if((channel[0] != channel[2]) && (channel[1] != channel[2])) {
              break;
            } else {
              channel[2] = rand() % 4;
            }
          }
          break;
        case 3:
          channel[3] = 6 - channel[0] - channel[1] - channel[2];
          break;
        default:
          return false;
      }
    }
  }
  LoRaSettings.RFFrequency = LoRaSettings.Channel_List[channel[HybirdHopping_TXcount]];
  SX1276LoRaSetRFFrequency(LoRaSettings.RFFrequency);
  //
  
  HybirdHopping_TXcount++;
  
  return true;
  
}



/***************************************************************************************************
 *  Function Name: HybirdHopping_SetRXChannel
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void HybirdHopping_SetRXChannel(void)
{
  
  //SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
  SX1276LoRaSetRFFrequency(LoRaSettings.Channel_List[0]);
  LoRaSettings.RFFrequency = LoRaSettings.Channel_List[0];
  
}



/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/


