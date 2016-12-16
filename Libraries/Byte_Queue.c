
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
#include "Byte_Queue.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

bool	Init_Byte_Queue( Byte_Queue_Struct *queue, uint16_t buf_sz )
{
	if( ! queue || ! buf_sz ) return( false );
	if( queue->Buffer ) return( false );

	queue->Buffer_Size = 0;
	queue->Buffer = malloc( buf_sz );
	if( ! queue->Buffer ) return( false );

	queue->Buffer_Size = buf_sz;
	return( true );
}

void	Clear_Byte_Queue( Byte_Queue_Struct *queue )
{
	queue->Buffer[ 0 ] = 0x00;
	queue->Head = queue->Tail = 0;
}

bool	Is_Byte_Queue_Empty( Byte_Queue_Struct *queue )
{
	return( queue->Head == queue->Tail );
}

bool	Is_Byte_Queue_Full( Byte_Queue_Struct *queue )
{
	uint16_t	t;

	t = queue->Head + 1;
	t %= queue->Buffer_Size;
	return( t == queue->Tail );
}

bool	Get_Byte_from_Queue( Byte_Queue_Struct *queue )
{
	if( queue->Tail == queue->Head ) return( false );

	queue->Output = queue->Buffer[ queue->Tail ];
	queue->Tail++;
	queue->Tail %= queue->Buffer_Size;

	return( true );
}

bool	Peek_Byte_from_Queue( Byte_Queue_Struct *queue )
{
	if( queue->Tail == queue->Head ) return( false );

	queue->Output = queue->Buffer[ queue->Tail ];
	return( true );
}

bool	Put_Byte_into_Queue( Byte_Queue_Struct *queue, uint8_t d )
{
	if( Is_Byte_Queue_Full( queue ) ) return( false );

	queue->Buffer[ queue->Head ] = d;
	queue->Head++;
	queue->Head %= queue->Buffer_Size;

	return( true );
}

uint16_t	Get_Data_Size_of_Byte_Queue( Byte_Queue_Struct *queue )
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

uint16_t	Get_Available_Size_of_Byte_Queue( Byte_Queue_Struct *queue )
{
	uint16_t	t;

	t = Get_Data_Size_of_Byte_Queue( queue );
	return( queue->Buffer_Size - t - 1 );
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
