
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_LINKLISTEVENT_H_
#define USER_LINKLISTEVENT_H_

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
#define MAX_LoraEventCount	5		// 5 * 3 = 15, 15 * 8 = 120, 每一優先權最多5個事件,一個 SLAVE Node 有三種優先權
						// 三種優先權共最多15個事件,最多有 7 個 SLAVE Node,加上共同排程,固有 8 個
// Event Priority
#define LoraEventPriority0	0		// 優先權0級別(最高優先權級別)(規劃只有三種優先權級別,0~2,最高優先權為0,最低為2)
#define LoraEventPriority1	1		// 優先權1級別(中等優先權級別)
#define LoraEventPriority2	2		// 優先權2級別(最低優先權級別)
#define LoraEventPriorities	3		// 優先權級別數


typedef struct linklist {
	uint8_t			NodeNumber;
	uint8_t			NodeEvent;
	uint8_t			NodeAddr[3];
	uint8_t *		NodeData;
	uint8_t			NodeDataSize;
	struct linklist *	Next;
	struct linklist *	Previous;
} tLoraNodeEvent;

typedef struct LoraRunEvent {
	uint8_t		RunNodeNumber;
	uint8_t		RunNodeEvent;
	uint8_t		RunNodeAddr[3];
	uint8_t		RunNodeData[MaxMsgDataSize];
	uint8_t		RunNodeDataSize;
} tLoraRunningEvent;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern	uint8_t		Event_Count[LoraEventPriorities];

/* Exported functions ------------------------------------------------------- */
void	LoraLinkListEvent_Initialization(void);
bool	LoraLinkListEvent_CreateEvent( uint8_t Priority, uint8_t Num, uint8_t Event, const uint8_t *Addr, const uint8_t *Data, uint8_t *DataSize );
void	LoraLinkListEvent_DestroyHeadEvent(uint8_t);

uint8_t	LoraLinkListEvent_ComputeEvent(tLoraNodeEvent *EventHead, uint8_t *EventCount);
bool	LoraLinkListEvent_BuildLoraEvent( uint8_t Priority, uint8_t Num, uint8_t Event, const uint8_t *Addr, const uint8_t *Data, uint8_t *DataSize );
bool	LoraLinkListEvent_DispatcherLoraEvent(void);
void	LoraLinkListEvent_LoraEventDelete( uint8_t Priority, const uint8_t *Addr );
void	LoraLinkListEvent_LoraEventReconfirm(uint8_t *Addr);
void	LoraLinkListEvent_LoraEventClearAll(void);
bool	LoraLinkListEvent_CreateNodeEvent( uint8_t Priority, uint8_t Num, uint8_t Event, const uint8_t *Data, uint8_t *DataSize );
void	LoraLinkListEvent_DestroyNodeHeadEvent(uint8_t, uint8_t);
void	LoraLinkListEvent_LoraNodeEventDelete(uint8_t);

#endif		// USER_LINKLISTEVENT_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
