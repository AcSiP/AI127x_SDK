
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
#ifndef USER_RANDOMHOPSTARTCHANNEL_H_
#define USER_RANDOMHOPSTARTCHANNEL_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
void	RandomHopStartChannel_SetHoppingStartChannelFreq(uint8_t);
void	RandomHopStartChannel_SlaveDefaultHoppingChannel(void);

void	Clear_FHSS_Channel_List( void );
void	Load_Default_FHSS_Channel_List( void );
void	List_FHSS_Channel_List( void );
bool	Append_FHSS_Channel_into_List( uint32_t freq_Hz );

#endif		// USER_RANDOMHOPSTARTCHANNEL_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
