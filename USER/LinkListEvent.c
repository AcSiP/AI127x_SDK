/**
  ******************************************************************************
  * @file    Project/ARM-Lora/LinkListEvent.c 
  * @author  JC
  * @version 
  * @date    
  * @brief   LinkListEvent.c program body
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
#include "main.h"
#include "acsip_protocol.h"
#include "normalMaster.h"
#include "LinkListEvent.h"
#include "sleep.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
tLoraNodeEvent *EventHeadPriority0, *EventHeadPriority1, *EventHeadPriority2;
tLoraNodeEvent *EventTailPriority0, *EventTailPriority1, *EventTailPriority2;
uint8_t EventCountPriority0, EventCountPriority1, EventCountPriority2;
static tLoraNodeEvent *temp = NULL;
extern __IO tLoraRunningEvent LoraRunningEvent;
extern tLoraDeviceNode *LoraNodeDevice[MAX_LoraNodeNum];      //for MASTER
extern tDeviceNodeSleepAndRandomHop *DeviceNodeSleepAndRandomHop[MAX_LoraNodeNum];  //for MASTER 



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_Initialization
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
void LoraLinkListEvent_Initialization(void) {
  
  EventHeadPriority0 = NULL;
  EventTailPriority0 = NULL;
  EventCountPriority0 = 0;
  
  EventHeadPriority1 = NULL;
  EventTailPriority1 = NULL;
  EventCountPriority1 = 0;
  
  EventHeadPriority2 = NULL;
  EventTailPriority2 = NULL;
  EventCountPriority2 = 0;
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_CreateEvent
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
//bool LoraLinkListEvent_CreateEvent(tLoraNodeEvent *EventHead, tLoraNodeEvent *EventTail, uint8_t *EventCount, uint8_t Num, uint8_t Event, uint8_t *Addr, uint8_t *Data, uint8_t *DataSize) {
bool LoraLinkListEvent_CreateEvent(uint8_t Priority, uint8_t Num, uint8_t Event, uint8_t *Addr, uint8_t *Data, uint8_t *DataSize) {
  
  //int8_t str[5];    //test output
  uint8_t count;
  //tLoraNodeEvent *temp = NULL;
  tLoraNodeEvent **EventHead = NULL;
  tLoraNodeEvent **EventTail = NULL;
  uint8_t *EventCount = NULL;
  
  temp = NULL;
  
  switch(Priority) {
    case LoraEventPriority0:
       EventHead = &EventHeadPriority0;
       EventTail = &EventTailPriority0;
       EventCount = &EventCountPriority0;
      break;
    case LoraEventPriority1:
      EventHead = &EventHeadPriority1;
      EventTail = &EventTailPriority1;
      EventCount = &EventCountPriority1;
      break;
    case LoraEventPriority2:
      EventHead = &EventHeadPriority2;
      EventTail = &EventTailPriority2;
      EventCount = &EventCountPriority2;
      break;
    default:
      break;
  }
  
  if(*EventCount == 0) {
    temp = (tLoraNodeEvent *) malloc(sizeof(tLoraNodeEvent));
    if(temp != NULL) {
      temp->NodeNumber = Num;
      temp->NodeEvent = Event;
      temp->NodeAddr[0] = Addr[0];
      temp->NodeAddr[1] = Addr[1];
      temp->NodeAddr[2] = Addr[2];
      if((DataSize != NULL) && (Data != NULL)) {
        temp->NodeData = (uint8_t *) malloc((*DataSize) * sizeof(uint8_t));
        if(temp->NodeData != NULL) {
          temp->NodeDataSize = *DataSize;
          for(count = 0 ; count < *DataSize ; count++) {
            temp->NodeData[count] = Data[count];
          }
        } else {
          temp->NodeData = NULL;
          temp->NodeDataSize = 0;
        }
      } else  {
        temp->NodeData = NULL;
        temp->NodeDataSize = 0;
      }
      *EventHead = *EventTail = temp;
      (*EventHead)->Next = NULL;
      (*EventHead)->Previous = NULL;
      *EventCount += 1;
      return true;
    }
  } else if((*EventCount > 0) && (*EventCount < MAX_LoraEventCount)) {
    temp = (tLoraNodeEvent *) malloc(sizeof(tLoraNodeEvent));
    if(temp != NULL) {
      temp->NodeNumber = Num;
      temp->NodeEvent = Event;
      temp->NodeAddr[0] = Addr[0];
      temp->NodeAddr[1] = Addr[1];
      temp->NodeAddr[2] = Addr[2];
      if((DataSize  != NULL) && (Data != NULL)) {
        temp->NodeData = (uint8_t *) malloc((*DataSize) * sizeof(uint8_t));
        if(temp->NodeData != NULL) {
          temp->NodeDataSize = *DataSize;
          for(count = 0 ; count < *DataSize ; count++) {
            temp->NodeData[count] = Data[count];
          }
        } else {
          temp->NodeData = NULL;
          temp->NodeDataSize = 0;
        }
      } else  {
        temp->NodeData = NULL;
        temp->NodeDataSize = 0;
      }
      (*EventTail)->Next = temp;
      temp->Previous = *EventTail;
      temp->Next = NULL;
      *EventTail = temp;
      *EventCount += 1;
      return true;
    }
  }
  
  //CmdUART_UartWrite((uint8_t *)"LoraLinkListEvent_CreateEvent()\r\n", strlen("LoraLinkListEvent_CreateEvent()\r\n"));    //test output
  //sprintf((char *)str, "%02d", EventCountPriority2);    //test output
  //CmdUART_UartWrite((uint8_t *)"EventCountPriority2=", strlen("EventCountPriority2="));    //test output
  //CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));    //test output
  //CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));    //test output
  
  /*
  switch(Priority) {
    case LoraEventPriority0:
      EventHeadPriority0 = EventHead;
      EventTailPriority0 = EventTail;
      return true;
    case LoraEventPriority1:
      EventHeadPriority1 = EventHead;
      EventTailPriority1 = EventTail;
      return true;
    case LoraEventPriority2:
      EventHeadPriority2 = EventHead;
      EventTailPriority2 = EventTail;
      return true;
    default:
      break;
  }
  */
  
  return false;
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_DestroyHeadEvent
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
//void LoraLinkListEvent_DestroyHeadEvent(tLoraNodeEvent *EventHead, tLoraNodeEvent *EventTail, uint8_t *EventCount) {
void LoraLinkListEvent_DestroyHeadEvent(uint8_t Priority) {
  
  //int8_t str[5];    //test output
  //tLoraNodeEvent *temp = NULL;
  tLoraNodeEvent **EventHead = NULL;
  tLoraNodeEvent **EventTail = NULL;
  uint8_t *EventCount = NULL;
  
  temp = NULL;
  
  switch(Priority) {
    case LoraEventPriority0:
       EventHead = &EventHeadPriority0;
       EventTail = &EventTailPriority0;
       EventCount = &EventCountPriority0;
      break;
    case LoraEventPriority1:
      EventHead = &EventHeadPriority1;
      EventTail = &EventTailPriority1;
      EventCount = &EventCountPriority1;
      break;
    case LoraEventPriority2:
      EventHead = &EventHeadPriority2;
      EventTail = &EventTailPriority2;
      EventCount = &EventCountPriority2;
      break;
    default:
      break;
  }
  
  if(*EventCount == 0) {
    return;
  } else if(*EventCount == 1) {
    temp = *EventHead;
    *EventHead = NULL;
    *EventTail = NULL;
    *EventCount = 0;
  } else {
    temp = *EventHead;
    *EventHead = temp->Next;
    (*EventHead)->Previous = NULL;
    *EventCount -= 1;
  }
  
  free(temp->NodeData);
  free(temp);
  
  //CmdUART_UartWrite((uint8_t *)"LoraLinkListEvent_DestroyHeadEvent()\r\n", strlen("LoraLinkListEvent_DestroyHeadEvent()\r\n"));    //test output
  //sprintf((char *)str, "%02d", EventCountPriority2);    //test output
  //CmdUART_UartWrite((uint8_t *)"EventCountPriority2=", strlen("EventCountPriority2="));    //test output
  //CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));    //test output
  //CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));    //test output
  
  /*
  switch(Priority) {
    case LoraEventPriority0:
      EventHeadPriority0 = EventHead;
      EventTailPriority0 = EventTail;
    case LoraEventPriority1:
      EventHeadPriority1 = EventHead;
      EventTailPriority1 = EventTail;
    case LoraEventPriority2:
      EventHeadPriority2 = EventHead;
      EventTailPriority2 = EventTail;
    default:
      break;
  }
  */
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_DestroyTailEvent
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
//void LoraLinkListEvent_DestroyTailEvent(tLoraNodeEvent *EventHead, tLoraNodeEvent *EventTail, uint8_t *EventCount) {
void LoraLinkListEvent_DestroyTailEvent(uint8_t Priority) {
  
  //int8_t str[5];    //test output
  //tLoraNodeEvent *temp = NULL;
  tLoraNodeEvent **EventHead = NULL;
  tLoraNodeEvent **EventTail = NULL;
  uint8_t *EventCount = NULL;
  
  temp = NULL;
  
  switch(Priority) {
    case LoraEventPriority0:
      EventHead = &EventHeadPriority0;
      EventTail = &EventTailPriority0;
      EventCount = &EventCountPriority0;
      break;
    case LoraEventPriority1:
      EventHead = &EventHeadPriority1;
      EventTail = &EventTailPriority1;
      EventCount = &EventCountPriority1;
      break;
    case LoraEventPriority2:
      EventHead = &EventHeadPriority2;
      EventTail = &EventTailPriority2;
      EventCount = &EventCountPriority2;
      break;
    default:
      break;
  }
  
  if(*EventCount == 0) {
    return;
  } else if(*EventCount == 1){
    temp = *EventTail;
    *EventHead = NULL;
    *EventTail = NULL;
    *EventCount = 0;
  } else {
    temp = *EventTail;
    *EventTail = temp->Previous;
    (*EventTail)->Next = NULL;
    *EventCount -= 1;
  }
  
  free(temp->NodeData);
  free(temp);
  
  //CmdUART_UartWrite((uint8_t *)"LoraLinkListEvent_DestroyTailEvent()\r\n", strlen("LoraLinkListEvent_DestroyTailEvent()\r\n"));    //test output
  //sprintf((char *)str, "%02d", EventCountPriority2);    //test output
  //CmdUART_UartWrite((uint8_t *)"EventCountPriority2=", strlen("EventCountPriority2="));    //test output
  //CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));    //test output
  //CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));    //test output
  
  /*
  switch(Priority) {
    case LoraEventPriority0:
      EventHeadPriority0 = EventHead;
      EventTailPriority0 = EventTail;
    case LoraEventPriority1:
      EventHeadPriority1 = EventHead;
      EventTailPriority1 = EventTail;
    case LoraEventPriority2:
      EventHeadPriority2 = EventHead;
      EventTailPriority2 = EventTail;
    default:
      break;
  }
  */
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_ComputeEvent
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
uint8_t LoraLinkListEvent_ComputeEvent(tLoraNodeEvent *EventHead, uint8_t *EventCount) {
  
  uint8_t count;
  //tLoraNodeEvent *temp;
  
  temp = EventHead;
  count = 0;
  
  while(temp != NULL) {
    count++;
    temp = temp->Next;
  }
  
  *EventCount = count;
  return *EventCount;
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_isEventPriority0BufferFull
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
bool LoraLinkListEvent_isEventPriority0BufferFull(void) {
  
  if(LoraLinkListEvent_ComputeEvent(EventHeadPriority0, &EventCountPriority0) >= MAX_LoraEventCount) {
    return true;
  }
  
  return false;
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_isEventPriority1BufferFull
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
bool LoraLinkListEvent_isEventPriority1BufferFull(void) {
  
  if(LoraLinkListEvent_ComputeEvent(EventHeadPriority1, &EventCountPriority1) >= MAX_LoraEventCount) {
    return true;
  }
  
  return false;
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_isEventPriority2BufferFull
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
bool LoraLinkListEvent_isEventPriority2BufferFull(void) {
  
  if(LoraLinkListEvent_ComputeEvent(EventHeadPriority2, &EventCountPriority2) >= MAX_LoraEventCount) {
    return true;
  }
  
  return false;
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_BuildLoraEvent
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
bool LoraLinkListEvent_BuildLoraEvent(uint8_t Priority, uint8_t Num, uint8_t Event, uint8_t *Addr, uint8_t *Data, uint8_t *DataSize) {
  
  bool result = false;
  
  switch(Event) {
    case Master_AcsipProtocol_Broadcast:
    case Master_AcsipProtocol_Join:
      result = LoraLinkListEvent_CreateEvent(Priority, Num, Event, Addr, Data, DataSize);
      break;
    case Master_AcsipProtocol_Poll:
    case Master_AcsipProtocol_Data:
    case Master_AcsipProtocol_Leave:
    case Master_AcsipProtocol_Interval:
      if(DeviceNodeSleepAndRandomHop[Num] != NULL) {
        result = LoraLinkListEvent_CreateNodeEvent(Priority, Num, Event, Data, DataSize);
      }
      break;
    default:
      break;
  }
  
  /*
  switch(Priority) {
    case 0:
      temp = LoraLinkListEvent_CreateEvent(EventHeadPriority0, EventTailPriority0, &EventCountPriority0, Num, Event, Addr, Data, DataSize);
      break;
    case 1:
      temp = LoraLinkListEvent_CreateEvent(EventHeadPriority1, EventTailPriority1, &EventCountPriority1, Num, Event, Addr, Data, DataSize);
      break;
    case 2:
      temp = LoraLinkListEvent_CreateEvent(EventHeadPriority2, EventTailPriority2, &EventCountPriority2, Num, Event, Addr, Data, DataSize);
      break;
    default:
      temp = false;
      break;
  }
  */
  
  return result;
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_DispatcherLoraEvent
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
bool LoraLinkListEvent_DispatcherLoraEvent(void) {
#pragma O0
  
  static int8_t Priority0Num = -1, Priority1Num = -1, Priority2Num = -1;
  int8_t *PriorityNum;
  uint8_t count, temp;
  tLoraNodeEvent *HeadTemp = NULL;
  
  count = 0;
  PriorityNum = &Priority0Num;
  while(count < 3) {
    if(*PriorityNum < 0) {
      switch(count) {
        case 0:
          if((EventHeadPriority0 != NULL) && (EventCountPriority0 > 0)) {
            HeadTemp = EventHeadPriority0;
            goto EventPackage;
          }
          break;
        case 1:
          if((EventHeadPriority1 != NULL) && (EventCountPriority1 > 0)) {
            HeadTemp = EventHeadPriority1;
            goto EventPackage;
          }
          break;
        case 2:
          if((EventHeadPriority2 != NULL) && (EventCountPriority2 > 0)) {
            HeadTemp = EventHeadPriority2;
            goto EventPackage;
          }
          break;
        default:
          break;
      }
    } else if((*PriorityNum >= 0) && (*PriorityNum < MAX_LoraNodeNum)) {
      if((DeviceNodeSleepAndRandomHop[*PriorityNum] != NULL) && (DeviceNodeSleepAndRandomHop[*PriorityNum]->isNowSleeping == false)) {
        switch(count) {                                          //上面判斷式表示 SLAVE 端是存在的,且現在是醒著狀態
          case 0:
            if((DeviceNodeSleepAndRandomHop[*PriorityNum]->EventHeadPriority0 != NULL) && (DeviceNodeSleepAndRandomHop[*PriorityNum]->EventCountPriority0 > 0)) {
              HeadTemp = DeviceNodeSleepAndRandomHop[*PriorityNum]->EventHeadPriority0;
              goto EventPackage;
            }
            break;
          case 1:
            if((DeviceNodeSleepAndRandomHop[*PriorityNum]->EventHeadPriority1 != NULL) && (DeviceNodeSleepAndRandomHop[*PriorityNum]->EventCountPriority1 > 0)) {
              HeadTemp = DeviceNodeSleepAndRandomHop[*PriorityNum]->EventHeadPriority1;
              goto EventPackage;
            }
            break;
          case 2:
            if((DeviceNodeSleepAndRandomHop[*PriorityNum]->EventHeadPriority2 != NULL) && (DeviceNodeSleepAndRandomHop[*PriorityNum]->EventCountPriority2 > 0)) {
              HeadTemp = DeviceNodeSleepAndRandomHop[*PriorityNum]->EventHeadPriority2;
              goto EventPackage;
            }
            break;
          default:
            break;
        }
      }
    } else {
      break;
    }
    
    (*PriorityNum)++;
    if(*PriorityNum >= MAX_LoraNodeNum) {
      *PriorityNum = -1;
      count++;
      switch(count) {
        case 0:
          PriorityNum = &Priority0Num;
          break;
        case 1:
          PriorityNum = &Priority1Num;
          break;
        case 2:
          PriorityNum = &Priority2Num;
          break;
        default:
          break;
      }
    }
  }
  
  return false;
  
EventPackage:
  if(HeadTemp == NULL) {
    return false;
  }
  LoraRunningEvent.RunNodeNumber = HeadTemp->NodeNumber;
  LoraRunningEvent.RunNodeEvent = HeadTemp->NodeEvent;
  LoraRunningEvent.RunNodeAddr[0] = HeadTemp->NodeAddr[0];
  LoraRunningEvent.RunNodeAddr[1] = HeadTemp->NodeAddr[1];
  LoraRunningEvent.RunNodeAddr[2] = HeadTemp->NodeAddr[2];
  if((HeadTemp->NodeDataSize > 0) && (HeadTemp->NodeData != NULL)) {
    LoraRunningEvent.RunNodeDataSize = HeadTemp->NodeDataSize;
    for(temp = 0 ; temp < HeadTemp->NodeDataSize ; temp++) {
      LoraRunningEvent.RunNodeData[temp] = HeadTemp->NodeData[temp];
    }
  } else  {
    LoraRunningEvent.RunNodeDataSize = 0;
  }
  
  if(*PriorityNum < 0) {
    switch(count) {
      case 0:
        LoraLinkListEvent_DestroyHeadEvent(LoraEventPriority0);
        break;
      case 1:
        LoraLinkListEvent_DestroyHeadEvent(LoraEventPriority1);
        break;
      case 2:
        LoraLinkListEvent_DestroyHeadEvent(LoraEventPriority2);
        break;
      default:
        return false;
    }
  } else if((*PriorityNum >= 0) && (*PriorityNum < MAX_LoraNodeNum)) {
    switch(count) {
      case 0:
        LoraLinkListEvent_DestroyNodeHeadEvent(LoraEventPriority0, *PriorityNum);
        break;
      case 1:
        LoraLinkListEvent_DestroyNodeHeadEvent(LoraEventPriority1, *PriorityNum);
        break;
      case 2:
        LoraLinkListEvent_DestroyNodeHeadEvent(LoraEventPriority2, *PriorityNum);
        break;
      default:
        return false;
    }
  } else {
    return false;
  }
  
  return true;
  
#pragma O2
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_LoraEventDelete
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
void LoraLinkListEvent_LoraEventDelete(uint8_t Priority, uint8_t *Addr) {
  
  tLoraNodeEvent *HeadTemp = NULL;
  tLoraNodeEvent *TailTemp = NULL;
  tLoraNodeEvent *PreTemp = NULL;
  tLoraNodeEvent *NxtTemp = NULL;
  
  switch(Priority) {
    case LoraEventPriority0:
       temp = EventHeadPriority0;
      break;
    case LoraEventPriority1:
      temp = EventHeadPriority1;
      break;
    case LoraEventPriority2:
      temp = EventHeadPriority2;
      break;
    default:
      return;
  }
  
  while(temp != NULL) {
    PreTemp = temp->Previous;
    NxtTemp = temp->Next;
    if((temp->NodeAddr[0] == Addr[0]) && (temp->NodeAddr[1] == Addr[1]) && (temp->NodeAddr[2] == Addr[2])) {
      if(PreTemp != NULL) {
        PreTemp->Next = NxtTemp;
      }
      if(NxtTemp != NULL) {
        NxtTemp->Previous = PreTemp;
      }
      free(temp->NodeData);
      free(temp);
      temp = NxtTemp;
    } else {
      if(HeadTemp == NULL) {
        HeadTemp = temp;
      }
      TailTemp = temp;
      temp = NxtTemp;
    }
  }
  
  switch(Priority) {
    case LoraEventPriority0:
      EventHeadPriority0 = HeadTemp;
      EventTailPriority0 = TailTemp;
      break;
    case LoraEventPriority1:
      EventHeadPriority1 = HeadTemp;
      EventTailPriority1 = TailTemp;
      break;
    case LoraEventPriority2:
      EventHeadPriority2 = HeadTemp;
      EventTailPriority2 = TailTemp;
      break;
    default:
      break;
  }
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_LoraEventReconfirm
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
void LoraLinkListEvent_LoraEventReconfirm(uint8_t *Addr) {
  
  LoraLinkListEvent_LoraEventDelete(LoraEventPriority0, Addr);
  LoraLinkListEvent_LoraEventDelete(LoraEventPriority1, Addr);
  LoraLinkListEvent_LoraEventDelete(LoraEventPriority2, Addr);
  
  LoraLinkListEvent_ComputeEvent(EventHeadPriority0, &EventCountPriority0);
  LoraLinkListEvent_ComputeEvent(EventHeadPriority1, &EventCountPriority1);
  LoraLinkListEvent_ComputeEvent(EventHeadPriority2, &EventCountPriority2);
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_CreateNodeEvent
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
bool LoraLinkListEvent_CreateNodeEvent(uint8_t Priority, uint8_t Num, uint8_t Event, uint8_t *Data, uint8_t *DataSize) {
  
  uint8_t count;
  tLoraNodeEvent **EventHead = NULL;
  tLoraNodeEvent **EventTail = NULL;
  uint8_t *EventCount = NULL;
  
  temp = NULL;
  
  switch(Priority) {
    case LoraEventPriority0:
      EventHead = &DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority0;
      EventTail = &DeviceNodeSleepAndRandomHop[Num]->EventTailPriority0;
      EventCount = &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority0;
      break;
    case LoraEventPriority1:
      EventHead = &DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority1;
      EventTail = &DeviceNodeSleepAndRandomHop[Num]->EventTailPriority1;
      EventCount = &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority1;
      break;
    case LoraEventPriority2:
      EventHead = &DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority2;
      EventTail = &DeviceNodeSleepAndRandomHop[Num]->EventTailPriority2;
      EventCount = &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority2;
      break;
    default:
      break;
  }
  
  temp = (tLoraNodeEvent *) malloc(sizeof(tLoraNodeEvent));
  if(temp != NULL) {
    temp->NodeNumber = Num;
    temp->NodeEvent = Event;
    temp->NodeAddr[0] = LoraNodeDevice[Num]->NodeAddress[0];
    temp->NodeAddr[1] = LoraNodeDevice[Num]->NodeAddress[1];
    temp->NodeAddr[2] = LoraNodeDevice[Num]->NodeAddress[2];
    if((DataSize != NULL) && (Data != NULL)) {
      temp->NodeData = (uint8_t *) malloc((*DataSize) * sizeof(uint8_t));
      if(temp->NodeData != NULL) {
        temp->NodeDataSize = *DataSize;
        for(count = 0 ; count < *DataSize ; count++) {
          temp->NodeData[count] = Data[count];
        }
      } else {
        temp->NodeData = NULL;
        temp->NodeDataSize = 0;
      }
    } else  {
      temp->NodeData = NULL;
      temp->NodeDataSize = 0;
    }
  }
  
  if(*EventCount == 0) {
    if(temp != NULL) {
      *EventHead = *EventTail = temp;
      (*EventHead)->Next = NULL;
      (*EventHead)->Previous = NULL;
      *EventCount += 1;
      return true;
    }
  } else if((*EventCount > 0) && (*EventCount < MAX_LoraEventCount)) {
    if(temp != NULL) {
      (*EventTail)->Next = temp;
      temp->Previous = *EventTail;
      temp->Next = NULL;
      *EventTail = temp;
      *EventCount += 1;
      return true;
    }
  } else {
    free(temp->NodeData);
    free(temp);
    temp = NULL;
  }
  
  return false;
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_DestroyNodeHeadEvent
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
void LoraLinkListEvent_DestroyNodeHeadEvent(uint8_t Priority, uint8_t Num) {
  
  tLoraNodeEvent **EventHead = NULL;
  tLoraNodeEvent **EventTail = NULL;
  uint8_t *EventCount = NULL;
  
  temp = NULL;
  
  switch(Priority) {
    case LoraEventPriority0:
      EventHead = &DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority0;
      EventTail = &DeviceNodeSleepAndRandomHop[Num]->EventTailPriority0;
      EventCount = &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority0;
      break;
    case LoraEventPriority1:
      EventHead = &DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority1;
      EventTail = &DeviceNodeSleepAndRandomHop[Num]->EventTailPriority1;
      EventCount = &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority1;
      break;
    case LoraEventPriority2:
      EventHead = &DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority2;
      EventTail = &DeviceNodeSleepAndRandomHop[Num]->EventTailPriority2;
      EventCount = &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority2;
      break;
    default:
      break;
  }
  
  if(*EventCount == 0) {
    return;
  } else if(*EventCount == 1) {
    temp = *EventHead;
    *EventHead = NULL;
    *EventTail = NULL;
    *EventCount = 0;
  } else {
    temp = *EventHead;
    *EventHead = temp->Next;
    (*EventHead)->Previous = NULL;
    *EventCount -= 1;
  }
  
  free(temp->NodeData);
  free(temp);
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_DestroyNodeTailEvent
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
void LoraLinkListEvent_DestroyNodeTailEvent(uint8_t Priority, uint8_t Num) {
  
  tLoraNodeEvent **EventHead = NULL;
  tLoraNodeEvent **EventTail = NULL;
  uint8_t *EventCount = NULL;
  
  temp = NULL;
  
  switch(Priority) {
    case LoraEventPriority0:
      EventHead = &DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority0;
      EventTail = &DeviceNodeSleepAndRandomHop[Num]->EventTailPriority0;
      EventCount = &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority0;
      break;
    case LoraEventPriority1:
      EventHead = &DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority1;
      EventTail = &DeviceNodeSleepAndRandomHop[Num]->EventTailPriority1;
      EventCount = &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority1;
      break;
    case LoraEventPriority2:
      EventHead = &DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority2;
      EventTail = &DeviceNodeSleepAndRandomHop[Num]->EventTailPriority2;
      EventCount = &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority2;
      break;
    default:
      break;
  }
  
  if(*EventCount == 0) {
    return;
  } else if(*EventCount == 1){
    temp = *EventTail;
    *EventHead = NULL;
    *EventTail = NULL;
    *EventCount = 0;
  } else {
    temp = *EventTail;
    *EventTail = temp->Previous;
    (*EventTail)->Next = NULL;
    *EventCount -= 1;
  }
  
  free(temp->NodeData);
  free(temp);
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_isNodeEventPriority0BufferFull
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
bool LoraLinkListEvent_isNodeEventPriority0BufferFull(uint8_t Num) {
  
  if(LoraLinkListEvent_ComputeEvent(DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority0, &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority0) >= MAX_LoraEventCount) {
    return true;
  }
  
  return false;
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_isNodeEventPriority1BufferFull
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
bool LoraLinkListEvent_isNodeEventPriority1BufferFull(uint8_t Num) {
  
  if(LoraLinkListEvent_ComputeEvent(DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority1, &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority1) >= MAX_LoraEventCount) {
    return true;
  }
  
  return false;
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_isNodeEventPriority2BufferFull
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
bool LoraLinkListEvent_isNodeEventPriority2BufferFull(uint8_t Num) {
  
  if(LoraLinkListEvent_ComputeEvent(DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority2, &DeviceNodeSleepAndRandomHop[Num]->EventCountPriority2) >= MAX_LoraEventCount) {
    return true;
  }
  
  return false;
  
}



/***************************************************************************************************
 *  Function Name: LoraLinkListEvent_LoraNodeEventDelete
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 *  Readme: for MASTER
 **************************************************************************************************/
void LoraLinkListEvent_LoraNodeEventDelete(uint8_t Num) {
  
  tLoraNodeEvent *HeadTemp = NULL;
  tLoraNodeEvent *NxtTemp = NULL;
  
  if(Num >= MAX_LoraNodeNum) {
    return;
  }
  
  if(DeviceNodeSleepAndRandomHop[Num] == NULL) {
    return;
  }
  
  HeadTemp = DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority0;
  while(HeadTemp != NULL) {
    NxtTemp = HeadTemp->Next;
    free(HeadTemp->NodeData);
    free(HeadTemp);
    HeadTemp = NxtTemp;
  }
  
  HeadTemp = DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority1;
  while(HeadTemp != NULL) {
    NxtTemp = HeadTemp->Next;
    free(HeadTemp->NodeData);
    free(HeadTemp);
    HeadTemp = NxtTemp;
  }
  
  HeadTemp = DeviceNodeSleepAndRandomHop[Num]->EventHeadPriority2;
  while(HeadTemp != NULL) {
    NxtTemp = HeadTemp->Next;
    free(HeadTemp->NodeData);
    free(HeadTemp);
    HeadTemp = NxtTemp;
  }
  
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


