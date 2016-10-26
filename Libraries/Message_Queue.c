
//---------------------------------------------------------------------------
/*
//==========================================
// Author : Fox<shenfa@acsip.com.tw>
// Copyright 2016(C) AcSiP Technology Inc.
// 版權所有：群登科技股份有限公司
// http://www.acsip.com.tw
//==========================================
*/
//---------------------------------------------------------------------------


/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "Message_Queue.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
bool	Init_Message_Queue( Message_Queue_Struct *queue, uint16_t buf_size, uint16_t item_size )
{
	if( ! queue || ! buf_size || ! item_size ) return( false );

	if( ! Init_Byte_Queue( &( queue->Message_Queue ), buf_size ) ) return( false );
	if( ! Init_Word_Queue( &( queue->Item_Queue ), item_size ) ) return( false );
	return( true );
}

void	Clear_Message_Queue( Message_Queue_Struct *queue )
{
	Clear_Byte_Queue( &( queue->Message_Queue ) );
	Clear_Word_Queue( &( queue->Item_Queue ) );
}

bool	Is_Message_Queue_Empty( Message_Queue_Struct *queue )
{
	return( Is_Word_Queue_Empty( &( queue->Item_Queue ) ) );
}

bool	Put_Message_into_Queue( Message_Queue_Struct *queue, const uint8_t *msg, uint16_t length )
{
	uint16_t	i, rsz;

	// Check item space
	if( Is_Word_Queue_Full( &( queue->Item_Queue ) ) ) return( false );

	// Check buffer space
	rsz = Get_Available_Size_of_Byte_Queue( &( queue->Message_Queue ) );
	if( rsz < length ) return( false );

	// Insert message
	for( i = 0; i < length; i++ ) {
		Put_Byte_into_Queue( &( queue->Message_Queue ), msg[ i ] );
	}

	// Insert message length as item
	Put_Word_into_Queue( &( queue->Item_Queue ), length );
	return( true );
}


bool	Get_Message_from_Queue( uint8_t *buffer, uint16_t buf_sz, uint16_t *ret_length, Message_Queue_Struct *queue )
{
	uint16_t	i, dsz;

	// Get message length
	if( ! Peek_Word_from_Queue( &( queue->Item_Queue ) ) ) return( false );

	// Check buffer size
	dsz = queue->Item_Queue.Output;
	if( dsz > buf_sz ) {
		if( ret_length ) *ret_length = dsz;
		return( false );
	}

	// Get message
	for( i = 0; i < dsz; i++ ) {
		Get_Byte_from_Queue( &( queue->Message_Queue ) );
		buffer[ i ] = queue->Message_Queue.Output;
	}
	if( ret_length ) *ret_length = i;

	// Remove item from queue
	Get_Word_from_Queue( &( queue->Item_Queue ) );

	return( true );
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/

