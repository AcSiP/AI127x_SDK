
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
#ifndef LIBRARIES_DWORD_QUEUE_H_
#define LIBRARIES_DWORD_QUEUE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct	_DWord_Queue_Struct_ {
	uint32_t	Output;
	uint32_t	*Buffer;
	uint16_t	Buffer_Size;
	uint16_t	Head;		// Head == Tail   means there is no data
	uint16_t	Tail;		// Head+1 == Tail means the queue is full
} DWord_Queue_Struct;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
bool		Init_DWord_Queue( DWord_Queue_Struct *queue, uint16_t buf_items );
void		Clear_DWord_Queue( DWord_Queue_Struct *queue );
bool		Is_DWord_Queue_Empty( DWord_Queue_Struct *queue );
bool		Is_DWord_Queue_Full( DWord_Queue_Struct *queue );
bool		Get_DWord_from_Queue( DWord_Queue_Struct *queue );
bool		Peek_DWord_from_Queue( DWord_Queue_Struct *queue );
bool		Put_DWord_into_Queue( DWord_Queue_Struct *queue, uint32_t d );
uint16_t	Get_Data_Size_of_DWord_Queue( DWord_Queue_Struct *queue );
uint16_t	Get_Available_Size_of_DWord_Queue( DWord_Queue_Struct *queue );

#endif		// LIBRARIES_DWORD_QUEUE_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
