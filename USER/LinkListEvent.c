
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
tLoraNodeEvent *	Event_Head[ LoraEventPriorities ];
uint8_t			Event_Count[ LoraEventPriorities ];
uint8_t			Polling_Index = 0;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


extern __IO tLoraRunningEvent		LoraRunningEvent;


void		LoraLinkListEvent_Initialization( void )
{
	uint8_t		i;

	for( i = 0; i < LoraEventPriorities; i++ ) {
		Event_Head[i] = NULL;
		Event_Count[i] = 0;
	}
}


tLoraNodeEvent *	LoraLinkListEvent__Get_Tail( tLoraNodeEvent *event_chain )
{
	tLoraNodeEvent	*ret;
	tLoraNodeEvent	*tmp;

	if( ! event_chain ) return( NULL );

	ret = event_chain;
	while( ret->Next ) {
		tmp = ret->Next;
		ret = tmp;
	}

	return( ret );
}


uint16_t		LoraLinkListEvent__Count_Events( tLoraNodeEvent *event_chain )
{
	tLoraNodeEvent	*p;
	tLoraNodeEvent	*tmp;
	uint16_t	r;

	if( ! event_chain ) return( 0 );

	p = event_chain;
	r = 1;
	while( p->Next ) {
		r++;
		tmp = p->Next;
		p = tmp;
	}

	return( r );
}


bool			LoraLinkListEvent__Append( tLoraNodeEvent *event_chain, tLoraNodeEvent *event_node )
{
	tLoraNodeEvent	*tail;

	if( ! event_node ) return( false );

	tail = LoraLinkListEvent__Get_Tail( event_chain );
	if( ! tail ) return( false );

	tail->Next = event_node;
	event_node->Previous = tail;
	return( true );
}


void			LoraLinkListEvent__Free_Event( tLoraNodeEvent *event )
{
	if( event->NodeData ) {
		free( event->NodeData );
		event->NodeData = NULL;
	}
	free( event );
}


tLoraNodeEvent *	LoraLinkListEvent__Malloc_Event( uint8_t Num, uint8_t Event, const uint8_t *Addr, const uint8_t *Data, uint8_t DataSize )
{
	tLoraNodeEvent *	temp;
	uint8_t			i;

	temp = (tLoraNodeEvent *) malloc( sizeof(tLoraNodeEvent) );
	if( ! temp ) return( NULL );		// malloc for link node fail

	temp->NodeNumber = Num;
	temp->NodeEvent = Event;
	temp->NodeAddr[0] = Addr[0];
	temp->NodeAddr[1] = Addr[1];
	temp->NodeAddr[2] = Addr[2];
	temp->NodeData = NULL;
	temp->NodeDataSize = 0;
	temp->Next = NULL;
	temp->Previous = NULL;

	if( DataSize && Data ) {
		temp->NodeData = (uint8_t *) malloc( DataSize );
		if( ! temp->NodeData ) {		// malloc for payload fail
			free( temp );
			return( NULL );
		}

		temp->NodeDataSize = DataSize;
		for( i = 0 ; i < DataSize ; i++ ) temp->NodeData[i] = Data[i];
	}

	return( temp );
}


bool		LoraLinkListEvent_CreateEvent( uint8_t priority, uint8_t Num, uint8_t Event, const uint8_t *Addr, const uint8_t *Data, uint8_t *DataSize )
{
	uint16_t		n;
	tLoraNodeEvent *	temp = NULL;
	tLoraNodeEvent **	pEvent_Head;
	uint8_t *		pEvent_Count;

	pEvent_Head = & Event_Head[ priority ];
	pEvent_Count = & Event_Count[ priority ];

	n = LoraLinkListEvent__Count_Events( *pEvent_Head );
	if( ( n + 1 ) < MAX_LoraEventCount ) {
		temp = LoraLinkListEvent__Malloc_Event( Num, Event, Addr, Data, *DataSize );
		if( ! temp ) return( false );

		if( ! LoraLinkListEvent__Append( *pEvent_Head, temp ) ) {
			*pEvent_Head = temp;
			*pEvent_Count = 1;
			return( true );
		}

		*pEvent_Count += 1;
		return( true );
	}
	return( false );
}


void		LoraLinkListEvent_DestroyHeadEvent( uint8_t priority )
{
	tLoraNodeEvent *	temp = NULL;
	tLoraNodeEvent **	pEvent_Head;
	uint8_t *		pEvent_Count;

	pEvent_Head = & Event_Head[ priority ];
	pEvent_Count = & Event_Count[ priority ];

	if( *pEvent_Count == 0 ) return;

	if( *pEvent_Count == 1 ) {
		temp = *pEvent_Head;
		*pEvent_Head = NULL;
		*pEvent_Count = 0;
	} else {
		temp = *pEvent_Head;
		*pEvent_Head = temp->Next;
		(*pEvent_Head)->Previous = NULL;
		*pEvent_Count -= 1;
	}

	LoraLinkListEvent__Free_Event( temp );
}


uint8_t		LoraLinkListEvent_ComputeEvent(tLoraNodeEvent *EventHead, uint8_t *EventCount)
{
	uint8_t			count;
	tLoraNodeEvent *	temp;

	temp = EventHead;
	count = 0;

	while(temp != NULL) {
		count++;
		temp = temp->Next;
	}

	*EventCount = count;
	return *EventCount;
}


bool		LoraLinkListEvent_BuildLoraEvent( uint8_t Priority, uint8_t Num, uint8_t Event, const uint8_t *Addr, const uint8_t *Data, uint8_t *DataSize )
{
	bool	result = false;

	switch(Event) {
	case Master_AcsipProtocol_Broadcast:
	case Master_AcsipProtocol_Join:
		result = LoraLinkListEvent_CreateEvent( Priority, Num, Event, Addr, Data, DataSize );
		break;

	case Master_AcsipProtocol_Poll:
	case Master_AcsipProtocol_Data:
	case Master_AcsipProtocol_Leave:
	case Master_AcsipProtocol_Interval:
		if( Device_Information[Num].Flag_Valid ) {
			result = LoraLinkListEvent_CreateNodeEvent( Priority, Num, Event, Data, DataSize );
		}
		break;

	default:
		break;
	}

	return result;
}


void		Copy_NodeEvent_2_RunningEvent( const tLoraNodeEvent *event )
{
	uint8_t		i;

	LoraRunningEvent.RunNodeNumber = event->NodeNumber;
	LoraRunningEvent.RunNodeEvent = event->NodeEvent;
	LoraRunningEvent.RunNodeAddr[0] = event->NodeAddr[0];
	LoraRunningEvent.RunNodeAddr[1] = event->NodeAddr[1];
	LoraRunningEvent.RunNodeAddr[2] = event->NodeAddr[2];

	LoraRunningEvent.RunNodeDataSize = 0;
	if( event->NodeData ) {
		for( i = 0 ; i < event->NodeDataSize && i < MaxMsgDataSize ; i++ ) {
			LoraRunningEvent.RunNodeData[i] = event->NodeData[i];
		}
		LoraRunningEvent.RunNodeDataSize = i;
	}
}


bool		Dispatch__Lora_Event( uint8_t priority )
{
	tLoraNodeEvent *	pHead;
	uint8_t			i;
// 	char			cs[64];

	// Check Non group node event
	pHead = Event_Head[ priority ];
	if( pHead ) {
		Copy_NodeEvent_2_RunningEvent( pHead );
		LoraLinkListEvent_DestroyHeadEvent( priority );

// 		snprintf( cs, sizeof(cs), "%d, p=%d \r\n", __LINE__, priority );
// 		Console_Output_String( cs );
		return( true );
	}

	// Check group node event
	for( i = 0; i < MAX_LoraNodeNum; i++ ) {
		if( ! Device_Information[i].Flag_Valid ) continue;				// Slave is not exist
		if( Device_Information[i].Node_Sleep_Hop.isNowSleeping ) continue;		// Slave is sleeping

		pHead = Device_Information[i].Node_Sleep_Hop.Event_Head[ priority ];
		if( pHead ) {
			Copy_NodeEvent_2_RunningEvent( pHead );
			LoraLinkListEvent_DestroyNodeHeadEvent( priority, i );

// 			if( i == 0 ) Console_Output_String( "\r\n" );
// 			snprintf( cs, sizeof(cs), "%d, p=%d, i=%d \r\n", __LINE__, priority, i );
// 			Console_Output_String( cs );
			return( true );
		}
	}

	return( false );
}


bool		LoraLinkListEvent_DispatcherLoraEvent(void)
{
	if( Dispatch__Lora_Event( LoraEventPriority0 ) ) return( true );
	if( Dispatch__Lora_Event( LoraEventPriority1 ) ) return( true );
	if( Dispatch__Lora_Event( LoraEventPriority2 ) ) return( true );

	return( false );
}


void		LoraLinkListEvent_LoraEventDelete( uint8_t Priority, const uint8_t *Addr )
{
	tLoraNodeEvent *	HeadTemp = NULL;
	tLoraNodeEvent *	PreTemp = NULL;
	tLoraNodeEvent *	NxtTemp = NULL;
	tLoraNodeEvent *	temp;

	switch(Priority) {
	case LoraEventPriority0:
		temp = Event_Head[0];
		break;

	case LoraEventPriority1:
		temp = Event_Head[1];
		break;

	case LoraEventPriority2:
		temp = Event_Head[2];
		break;

	default:
		return;
	}

	while( temp ) {
		PreTemp = temp->Previous;
		NxtTemp = temp->Next;
		if( temp->NodeAddr[0] == Addr[0] && temp->NodeAddr[1] == Addr[1] && temp->NodeAddr[2] == Addr[2] ) {
			if( PreTemp ) PreTemp->Next = NxtTemp;
			if( NxtTemp ) NxtTemp->Previous = PreTemp;

			LoraLinkListEvent__Free_Event( temp );
			temp = NxtTemp;
		} else {
			if( ! HeadTemp ) HeadTemp = temp;
			temp = NxtTemp;
		}
	}
}



void		LoraLinkListEvent_LoraEventReconfirm(uint8_t *Addr)
{
	LoraLinkListEvent_LoraEventDelete(LoraEventPriority0, Addr);
	LoraLinkListEvent_LoraEventDelete(LoraEventPriority1, Addr);
	LoraLinkListEvent_LoraEventDelete(LoraEventPriority2, Addr);

	LoraLinkListEvent_ComputeEvent( Event_Head[0], & Event_Count[0] );
	LoraLinkListEvent_ComputeEvent( Event_Head[1], & Event_Count[1] );
	LoraLinkListEvent_ComputeEvent( Event_Head[2], & Event_Count[2] );
}


void		LoraLinkListEvent_LoraEventClearAll(void)
{
	uint8_t			priority;
	tLoraNodeEvent *	HeadTemp = NULL;
	tLoraNodeEvent *	NxtTemp = NULL;

	for(priority = 0 ; priority < 3 ; priority++) {
		switch(priority) {
		case 0:
			HeadTemp = Event_Head[0];
			break;

		case 1:
			HeadTemp = Event_Head[1];
			break;

		case 2:
			HeadTemp = Event_Head[2];
			break;

		default:
			goto ClearAllOut;
		}

		while(HeadTemp != NULL) {
			NxtTemp = HeadTemp->Next;
			LoraLinkListEvent__Free_Event( HeadTemp );
			HeadTemp = NxtTemp;
		}
	}

ClearAllOut:
	Event_Count[0] = 0;
	Event_Count[1] = 0;
	Event_Count[2] = 0;
}


bool		LoraLinkListEvent_CreateNodeEvent( uint8_t Priority, uint8_t Num, uint8_t Event, const uint8_t *Data, uint8_t *DataSize )
{
	tLoraNodeEvent **	EventHead = NULL;
	uint8_t *		EventCount = NULL;
	tLoraNodeEvent *	temp;
	char			cs[48];

	switch( Priority ) {
	case LoraEventPriority0:
		EventHead = & Device_Information[Num].Node_Sleep_Hop.Event_Head[0];
		EventCount = & Device_Information[Num].Node_Sleep_Hop.Event_Count[0];
		break;

	case LoraEventPriority1:
		EventHead = & Device_Information[Num].Node_Sleep_Hop.Event_Head[1];
		EventCount = & Device_Information[Num].Node_Sleep_Hop.Event_Count[1];
		break;

	case LoraEventPriority2:
		EventHead = & Device_Information[Num].Node_Sleep_Hop.Event_Head[2];
		EventCount = & Device_Information[Num].Node_Sleep_Hop.Event_Count[2];
		break;

	default:
		return( false );
	}

	temp = LoraLinkListEvent__Malloc_Event( Num, Event, Device_Information[Num].Node_MAC.NodeAddress, Data, *DataSize );
	if( ! temp ) {
		snprintf( cs, sizeof(cs), "%d, %s() \r\n", __LINE__, __FUNCTION__ );
		Console_Output_String( cs );
		return( false );
	}

	if( LoraLinkListEvent__Append( *EventHead, temp ) ) {
		*EventCount += 1;
	} else {
		*EventHead = temp;
		*EventCount = 1;
	}
	return( true );
}



void		LoraLinkListEvent_DestroyNodeHeadEvent( uint8_t priority, uint8_t num )
{
	tLoraNodeEvent *	temp = NULL;
	tLoraNodeEvent **	EventHead = NULL;
	uint8_t *		EventCount = NULL;

	EventHead = & Device_Information[num].Node_Sleep_Hop.Event_Head[ priority ];
	EventCount = & Device_Information[num].Node_Sleep_Hop.Event_Count[ priority ];

	if( *EventCount == 0 ) return;

	if( *EventCount == 1 ) {
		temp = *EventHead;
		*EventHead = NULL;
		*EventCount = 0;
	} else {
		temp = *EventHead;
		*EventHead = temp->Next;
		(*EventHead)->Previous = NULL;
		*EventCount -= 1;
	}

	LoraLinkListEvent__Free_Event( temp );
}


void		LoraLinkListEvent_LoraNodeEventDelete(uint8_t Num)
{
	tLoraNodeEvent *	HeadTemp = NULL;
	tLoraNodeEvent *	NxtTemp = NULL;

	if( Num >= MAX_LoraNodeNum ) return;
	if( ! Device_Information[Num].Flag_Valid ) return;

	HeadTemp = Device_Information[Num].Node_Sleep_Hop.Event_Head[0];
	while(HeadTemp != NULL) {
		NxtTemp = HeadTemp->Next;
		LoraLinkListEvent__Free_Event( HeadTemp );
		HeadTemp = NxtTemp;
	}
	Device_Information[Num].Node_Sleep_Hop.Event_Count[0] = 0;

	HeadTemp = Device_Information[Num].Node_Sleep_Hop.Event_Head[1];
	while(HeadTemp != NULL) {
		NxtTemp = HeadTemp->Next;
		LoraLinkListEvent__Free_Event( HeadTemp );
		HeadTemp = NxtTemp;
	}
	Device_Information[Num].Node_Sleep_Hop.Event_Count[1] = 0;

	HeadTemp = Device_Information[Num].Node_Sleep_Hop.Event_Head[2];
	while(HeadTemp != NULL) {
		NxtTemp = HeadTemp->Next;
		LoraLinkListEvent__Free_Event( HeadTemp );
		HeadTemp = NxtTemp;
	}
	Device_Information[Num].Node_Sleep_Hop.Event_Count[2] = 0;
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
