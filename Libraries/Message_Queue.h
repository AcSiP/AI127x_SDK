
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
#ifndef LIBRARIES_MESSAGE_QUEUE_H_
#define LIBRARIES_MESSAGE_QUEUE_H_
//---------------------------------------------------------------------------

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#include "Byte_Queue.h"
#include "Word_Queue.h"

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct	_Message_Queue_Struct_ {
	Byte_Queue_Struct	Message_Queue;
	Word_Queue_Struct	Item_Queue;
} Message_Queue_Struct;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
bool	Init_Message_Queue( Message_Queue_Struct *queue, uint16_t buf_size, uint16_t item_size );
void	Clear_Message_Queue( Message_Queue_Struct *queue );
bool	Is_Message_Queue_Empty( Message_Queue_Struct *queue );
bool	Put_Message_into_Queue( Message_Queue_Struct *queue, const uint8_t *msg, uint16_t length );
bool	Get_Message_from_Queue( uint8_t *buffer, uint16_t buf_sz, uint16_t *ret_length, Message_Queue_Struct *queue );

//---------------------------------------------------------------------------
#endif		// LIBRARIES_MESSAGE_QUEUE_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
