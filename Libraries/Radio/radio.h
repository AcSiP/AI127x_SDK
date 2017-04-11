

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


/*! 
 * \file       radio.h
 * \brief      Generic radio driver ( radio abstraction )
 *
 * \version    2.0.B2 
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Gregory Cristian on Apr 25 2013
 */
#ifndef __RADIO_H__
#define __RADIO_H__

#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/*!
 * SX1272 and SX1276 General parameters definition
 */
#define LORA				(1)         // [0: OFF, 1: ON]

#ifdef  USE_LORA_SX1276
	#define Target_Lora_Device	"SX1276"
	//#define Lora_Freq915		((uint32_t)915000000)
	//#define Lora_Freq868		((uint32_t)868000000)
	//#define MIN_RF_Freq		((uint32_t)137000000)
	//#define MAX_RF_Freq		((uint32_t)1020000000)
	#define Lora_Freq915		(915000000)
	#define Lora_Freq868		(868000000)
	#define Lora_Freq780		(780000000)
	#define MIN_RF_Freq		(137000000)
	#define MAX_RF_Freq		(1020000000)
	#define RUN_RF_Freq		(Lora_Freq915)
#elif  USE_LORA_SX1278
	#define Target_Lora_Device	"SX1278"
	//#define Lora_Freq470		((uint32_t)470000000)
	//#define Lora_Freq433		((uint32_t)433000000)
	//#define MIN_RF_Freq		((uint32_t)137000000)
	//#define MAX_RF_Freq		((uint32_t)525000000)
	#define Lora_Freq470		(470000000)
	#define MIN_RF_Freq		(137000000)
	#define MAX_RF_Freq		(525000000)
	#define RUN_RF_Freq		(Lora_Freq470)
#endif

#define Lora_RFFrequency		(RUN_RF_Freq)
//#define Lora_RFFrequency		(905000000)

//#define LoraReceptionFailureTimes	(5)	// Unit:Times
#define LoraReceptionFailureTimes	(3)	// Unit:Times

#define Lora_RFHoppingPeriod		(5)

#define MAX_LoraOutputPower		(20)
#define MIN_LoraOutputPower		(5)
#define MAX_LoraPayloadLength		(255)
#define MIN_LoraPayloadLength		(1)
#define LoraBufferLength		(256)
#define Lora_TxSuccess			(0x10)
#define Lora_TxFailed			(0x11)

#define LoraPV_DefaultTxLength		(9)
#define LoraPV_DefaultTxString		("AcsipWin")

/*!
 * RF process function return codes
 */
typedef enum
{
	RF_IDLE,
	RF_BUSY,
	RF_RX_DONE,
	RF_RX_TIMEOUT,
	RF_TX_DONE,
	RF_TX_TIMEOUT,
	RF_LEN_ERROR,
	RF_CHANNEL_EMPTY,
	RF_CHANNEL_ACTIVITY_DETECTED,
} tRFProcessReturnCodes;

/*!
 * Radio driver structure defining the different function pointers
 */
typedef struct sRadioDriver
{
	void		( *Init )( void );
	void		( *Reset )( void );
	void		( *StartRx )( void );
	void		( *GetRxPacket )( void *buffer, uint16_t *size );
	void		( *SetTxPacket )( const void *buffer, uint16_t size );
  // 20170123 add by JC
  void		( *StartCAD )( void );
  // 20170123 add finish by JC
	uint32_t	( *Process )( void );
} tRadioDriver;

/*!
 * \brief Initializes the RadioDriver structure with specific radio
 *        functions.
 *
 * \retval radioDriver Pointer to the radio driver variable
 */
tRadioDriver*	RadioDriverInit( void );

#endif // __RADIO_H__
