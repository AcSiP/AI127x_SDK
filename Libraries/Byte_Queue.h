
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LIBRARIES_BYTE_QUEUE_H_
#define LIBRARIES_BYTE_QUEUE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct	_Byte_Queue_Struct_ {
	uint8_t		Output;
	uint8_t		*Buffer;
	uint16_t	Buffer_Size;
	uint16_t	Head;		// Head == Tail   means there is no data
	uint16_t	Tail;		// Head+1 == Tail means the queue is full
} Byte_Queue_Struct;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
bool		Init_Byte_Queue( Byte_Queue_Struct *queue, uint16_t buf_sz );
void		Clear_Byte_Queue( Byte_Queue_Struct *queue );
bool		Is_Byte_Queue_Empty( Byte_Queue_Struct *queue );
bool		Is_Byte_Queue_Full( Byte_Queue_Struct *queue );
bool		Get_Byte_from_Queue( Byte_Queue_Struct *queue );
bool		Peek_Byte_from_Queue( Byte_Queue_Struct *queue );
bool		Put_Byte_into_Queue( Byte_Queue_Struct *queue, uint8_t d );
uint16_t	Get_Data_Size_of_Byte_Queue( Byte_Queue_Struct *queue );
uint16_t	Get_Available_Size_of_Byte_Queue( Byte_Queue_Struct *queue );

#endif		// LIBRARIES_BYTE_QUEUE_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
