
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
#include <stdlib.h>
#include "Word_Queue.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

bool	Init_Word_Queue( Word_Queue_Struct *queue, uint16_t buf_items )
{
	if( ! queue || ! buf_items ) return( false );
	if( queue->Buffer ) return( false );

	queue->Buffer_Size = 0;
	queue->Buffer = malloc( buf_items * sizeof( uint16_t ) );
	if( ! queue->Buffer ) return( false );

	queue->Buffer_Size = buf_items;
	return( true );
}

void	Clear_Word_Queue( Word_Queue_Struct *queue )
{
	queue->Buffer[ 0 ] = 0x00;
	queue->Head = queue->Tail;
}

bool	Is_Word_Queue_Empty( Word_Queue_Struct *queue )
{
	return( queue->Head == queue->Tail );
}

bool	Is_Word_Queue_Full( Word_Queue_Struct *queue )
{
	uint16_t	t;

	t = queue->Head + 1;
	t %= queue->Buffer_Size;
	return( t == queue->Tail );
}

bool	Get_Word_from_Queue( Word_Queue_Struct *queue )
{
	if( queue->Tail == queue->Head ) return( false );

	queue->Output = queue->Buffer[ queue->Tail ];
	queue->Tail++;
	queue->Tail %= queue->Buffer_Size;

	return( true );
}

bool	Peek_Word_from_Queue( Word_Queue_Struct *queue )
{
	if( queue->Tail == queue->Head ) return( false );

	queue->Output = queue->Buffer[ queue->Tail ];
	return( true );
}

bool	Put_Word_into_Queue( Word_Queue_Struct *queue, uint16_t d )
{
	if( Is_Word_Queue_Full( queue ) ) return( false );

	queue->Buffer[ queue->Head ] = d;
	queue->Head++;
	queue->Head %= queue->Buffer_Size;

	return( true );
}

uint16_t	Get_Data_Size_of_Word_Queue( Word_Queue_Struct *queue )
{
	uint16_t	t;

	if( queue->Head >= queue->Tail ) {
		t = queue->Head - queue->Tail;
	} else {
		t = queue->Buffer_Size - queue->Tail;
		t += queue->Head;
	}
	return( t );
}

uint16_t	Get_Available_Size_of_Word_Queue( Word_Queue_Struct *queue )
{
	uint16_t	t;

	t = Get_Data_Size_of_Word_Queue( queue );
	return( queue->Buffer_Size - t - 1 );
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
