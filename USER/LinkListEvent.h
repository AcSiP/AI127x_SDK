/**
  ******************************************************************************
  * @file    Project/ARM-Lora/LinkListEvent.h 
  * @author  JC
  * @version 
  * @date    
  * @brief   Header for LinkListEvent.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LINK_LIST_EVENT_H
#define __LINK_LIST_EVENT_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#ifdef STM32F072
  #include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif
#include "acsip_protocol.h"

/* Exported types ------------------------------------------------------------*/
#define MAX_LoraEventCount             (5)      //5 * 3 = 15, 15 * 8 = 120, �C�@�u���v�̦h5�Өƥ�,�@�� SLAVE Node ���T���u���v
                                                //�T���u���v�@�̦h15�Өƥ�,�̦h�� 7 �� SLAVE Node,�[�W�@�P�Ƶ{,�T�� 8 ��
//Event Priority
#define LoraEventPriority0             (0)       //�u���v0�ŧO(�̰��u���v�ŧO)(�W���u���T���u���v�ŧO,0~2,�̰��u���v��0,�̧C��2)
#define LoraEventPriority1             (1)       //�u���v1�ŧO(�����u���v�ŧO)
#define LoraEventPriority2             (2)       //�u���v2�ŧO(�̧C�u���v�ŧO)

typedef struct linklist {
  uint8_t NodeNumber;
  uint8_t NodeEvent;
  uint8_t NodeAddr[3];
  uint8_t *NodeData;
  uint8_t NodeDataSize;
  struct linklist *Next;
  struct linklist *Previous;
}tLoraNodeEvent;

typedef struct LoraRunEvent {
  uint8_t RunNodeNumber;
  uint8_t RunNodeEvent;
  uint8_t RunNodeAddr[3];
  uint8_t RunNodeData[MaxMsgDataSize];
  uint8_t RunNodeDataSize;
}tLoraRunningEvent;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoraLinkListEvent_Initialization(void);
//bool LoraLinkListEvent_CreateEvent(tLoraNodeEvent *EventHead, tLoraNodeEvent *EventTail, uint8_t *EventCount, uint8_t Num, uint8_t Event, uint8_t *Addr, uint8_t *Data, uint8_t *DataSize);
bool LoraLinkListEvent_CreateEvent(uint8_t Priority, uint8_t Num, uint8_t Event, uint8_t *Addr, uint8_t *Data, uint8_t *DataSize);
//void LoraLinkListEvent_DestroyHeadEvent(tLoraNodeEvent *EventHead, tLoraNodeEvent *EventTail, uint8_t *EventCount);
void LoraLinkListEvent_DestroyHeadEvent(uint8_t);
//void LoraLinkListEvent_DestroyTailEvent(tLoraNodeEvent *EventHead, tLoraNodeEvent *EventTail, uint8_t *EventCount);
void LoraLinkListEvent_DestroyTailEvent(uint8_t);
uint8_t LoraLinkListEvent_ComputeEvent(tLoraNodeEvent *EventHead, uint8_t *EventCount);
bool LoraLinkListEvent_isEventPriority0BufferFull(void);
bool LoraLinkListEvent_isEventPriority1BufferFull(void);
bool LoraLinkListEvent_isEventPriority2BufferFull(void);
bool LoraLinkListEvent_BuildLoraEvent(uint8_t Priority, uint8_t Num, uint8_t Event, uint8_t *Addr, uint8_t *Data, uint8_t *DataSize);
bool LoraLinkListEvent_DispatcherLoraEvent(void);
void LoraLinkListEvent_LoraEventDelete(uint8_t Priority, uint8_t *Addr);
void LoraLinkListEvent_LoraEventReconfirm(uint8_t *Addr);
bool LoraLinkListEvent_CreateNodeEvent(uint8_t, uint8_t, uint8_t, uint8_t *, uint8_t *);
void LoraLinkListEvent_DestroyNodeHeadEvent(uint8_t, uint8_t);
void LoraLinkListEvent_DestroyNodeTailEvent(uint8_t, uint8_t);
bool LoraLinkListEvent_isNodeEventPriority0BufferFull(uint8_t);
bool LoraLinkListEvent_isNodeEventPriority1BufferFull(uint8_t);
bool LoraLinkListEvent_isNodeEventPriority2BufferFull(uint8_t);
void LoraLinkListEvent_LoraNodeEventDelete(uint8_t);



#endif /* __LINK_LIST_EVENT_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


