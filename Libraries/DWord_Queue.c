
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
#include "DWord_Queue.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

bool	Init_DWord_Queue( DWord_Queue_Struct *queue, uint16_t buf_items )
{
	if( ! queue || ! buf_items ) return( false );
	if( queue->Buffer ) return( false );

	queue->Buffer_Size = 0;
	queue->Buffer = malloc( buf_items * sizeof( uint32_t ) );
	if( ! queue->Buffer ) return( false );

	queue->Buffer_Size = buf_items;
	return( true );
}

void	Clear_DWord_Queue( DWord_Queue_Struct *queue )
{
	queue->Buffer[ 0 ] = 0x00;
	queue->Head = queue->Tail;
}

bool	Is_DWord_Queue_Empty( DWord_Queue_Struct *queue )
{
	return( queue->Head == queue->Tail );
}

bool	Is_DWord_Queue_Full( DWord_Queue_Struct *queue )
{
	uint16_t	t;

	t = queue->Head + 1;
	t %= queue->Buffer_Size;
	return( t == queue->Tail );
}

bool	Get_DWord_from_Queue( DWord_Queue_Struct *queue )
{
	if( queue->Tail == queue->Head ) return( false );

	queue->Output = queue->Buffer[ queue->Tail ];
	queue->Tail++;
	queue->Tail %= queue->Buffer_Size;

	return( true );
}

bool	Peek_DWord_from_Queue( DWord_Queue_Struct *queue )
{
	if( queue->Tail == queue->Head ) return( false );

	queue->Output = queue->Buffer[ queue->Tail ];
	return( true );
}

bool	Put_DWord_into_Queue( DWord_Queue_Struct *queue, uint32_t d )
{
	if( Is_DWord_Queue_Full( queue ) ) return( false );

	queue->Buffer[ queue->Head ] = d;
	queue->Head++;
	queue->Head %= queue->Buffer_Size;

	return( true );
}

uint16_t	Get_Data_Size_of_DWord_Queue( DWord_Queue_Struct *queue )
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

uint16_t	Get_Available_Size_of_DWord_Queue( DWord_Queue_Struct *queue )
{
	uint16_t	t;

	t = Get_Data_Size_of_DWord_Queue( queue );
	return( queue->Buffer_Size - t - 1 );
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
