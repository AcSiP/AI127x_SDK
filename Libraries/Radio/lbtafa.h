
//---------------------------------------------------------------------------
/*
//==========================================
// Author : JC<jc@acsip.com.tw>
// Copyright 2017(C) AcSiP Technology Inc.
// 版權所有：群登科技股份有限公司
// http://www.acsip.com.tw
//==========================================
*/
//---------------------------------------------------------------------------


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef		USER_LBTAFA_H_
#define		USER_LBTAFA_H_

/* Includes ------------------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/
#define			LBT__MAXIMUM_AFA_CHANNELS	7


/*
#define		LBTandAFA_Channels_CE		Lora_RFFrequency
#define		LBTandAFA_Channels_JP		Lora_RFFrequency

// #define	LBTandAFA_Channels		LBTandAFA_Channels_CE
#define		LBTandAFA_Channels		LBTandAFA_Channels_JP

#define		DefaultToDoLBT		(true)
#define		CE_ToDoLBT		(false)
#define		CE_ToDoAFA		(false)
#define		JP_ToDoLBT		(true)

#if ( CE_ToDoLBT == true ) && ( JP_ToDoLBT == false )
	#define		CE_LBT_Function		(true)
	#define		JP_LBT_Function		(false)

#elif ( CE_ToDoLBT == false ) && ( JP_ToDoLBT == true )
	#define		CE_LBT_Function		(false)
	#define		JP_LBT_Function		(true)
#else
	#define		CE_LBT_Function		(false)
	#define		JP_LBT_Function		(false)
#endif

#if ( CE_LBT_Function == true ) && ( CE_ToDoAFA == true )
	#define		CE_AFA_Function		(true)
#else
	#define		CE_AFA_Function		(false)
#endif

#define		DefaultPreambleLength		(128)
#define		CADandAFA_TX_PreambleLength	DefaultPreambleLength
#define		CADandAFA_RX_PreambleLength	DefaultPreambleLength
*/


/* Exported types ------------------------------------------------------------*/
typedef enum	Enum__Option_LBT {
	LBT__Option__None = 0,
	LBT__Option__Japan,
	LBT__Option__Europe_w_AFA,

	LBT__Option__Enums
} tEnum__Option_LBT;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported variables ------------------------------------------------------------*/
extern	tEnum__Option_LBT	LBT__Option_Mode;

/* Exported functions --------------------------------------------------------*/
void	LBTandAFA_RandomWaittingTimeSetting(void);
void	LBTandAFA_ToDoCADandAFASetting(void);
bool	LBTandAFA_IsRunningLBT(void);
void	LBTandAFA_TxPreambleSetting(void);
void	LBTandAFA_RxPreambleSetting(void);
void	LBTandAFA_ParameterDeInit(void);
bool	LBTandAFA_TX(void);
//void	LBTandAFA_ClearTxParameter(void);
bool	LBTandAFA_RX(void);
void	LBTandAFA_ClearRxParameter(void);
bool	SX1276LoRaIsChannelFree(int16_t);
bool	SX1276LoRaIsChannelRealFreeAndCanTX(void);


void		LBT__Initialize( tEnum__Option_LBT mode );
void		LBT__Configure_AFA_Channels( const int32_t *ch_freq_Hz, uint16_t channels );

bool		LBT__Wait_RF_Channel_Clear( uint16_t timeout_ms );
void		LBT__Configure_Receving( void );


#endif		// USER_LBTAFA_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
