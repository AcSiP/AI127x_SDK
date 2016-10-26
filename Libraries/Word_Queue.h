
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
#ifndef LIBRARIES_WORD_QUEUE_H_
#define LIBRARIES_WORD_QUEUE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct	_Word_Queue_Struct_ {
	uint16_t	Output;
	uint16_t	*Buffer;
	uint16_t	Buffer_Size;
	uint16_t	Head;		// Head == Tail   means there is no data
	uint16_t	Tail;		// Head+1 == Tail means the queue is full
} Word_Queue_Struct;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
bool		Init_Word_Queue( Word_Queue_Struct *queue, uint16_t buf_items );
void		Clear_Word_Queue( Word_Queue_Struct *queue );
bool		Is_Word_Queue_Empty( Word_Queue_Struct *queue );
bool		Is_Word_Queue_Full( Word_Queue_Struct *queue );
bool		Get_Word_from_Queue( Word_Queue_Struct *queue );
bool		Peek_Word_from_Queue( Word_Queue_Struct *queue );
bool		Put_Word_into_Queue( Word_Queue_Struct *queue, uint16_t d );
uint16_t	Get_Data_Size_of_Word_Queue( Word_Queue_Struct *queue );
uint16_t	Get_Available_Size_of_Word_Queue( Word_Queue_Struct *queue );

#endif		// LIBRARIES_WORD_QUEUE_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
