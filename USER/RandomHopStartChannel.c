
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

/**
  ******************************************************************************
  * @file    Project/ARM-Lora/RandomHopStartChannel.c 
  * @author  JC
  * @version V1.0.0
  * @date    05-Jul-2016
  * @brief   Random Hopping Start Channel program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#if ((Lora_RFFrequency >= 902000000) && (Lora_RFFrequency <= 928000000))		// for US 902~928MHz
const uint32_t	Default_Hopping_Frequency_Hz[] = {
	915000000,		// 0
	923000000,		// 1
	911500000,		// 2
	920500000,
	927500000,
	927000000,		// 5
	905500000,
	903500000,
	906000000,
	903000000,
	925000000,		// 10
	917500000,
	924000000,
	904000000,
	923500000,
	907500000,		// 15
	919500000,
	918500000,
	914500000,
	905000000,
	922000000,		// 20
	921000000,
	914000000,
	904500000,
	908000000,
	916000000,		// 25
	908500000,
	920000000,
	911000000,
	912000000,
	924500000,		// 30
	922500000,
	912500000,
	916500000,
	909000000,
	921500000,		// 35
	919000000,
	915500000,
	909500000,
	910000000,
	910500000,		// 40
	913500000,
	926500000,
	918000000,
	907000000,
	917000000,		// 45
	913000000,
	926000000,
	906500000,
	902500000,
	925500000		// 50
};

#elif ((Lora_RFFrequency >= 470000000) && (Lora_RFFrequency <= 510000000))		// for china 470~510MHz or 470~517MHz
const uint32_t	Default_Hopping_Frequency_Hz[] = {
	470000000,		// 0
	473000000,		// 1
	492000000,
	488000000,
	501000000,
	498000000,		// 5
	483000000,
	475000000,
	499000000,
	505000000,
	500000000,		// 10
	471000000,
	485000000,
	490000000,
	479000000,
	477000000,		// 15
	486000000,
	481000000,
	493000000,
	508000000,
	506000000,		// 20
	472500000,
	478000000,
	488500000,
	484000000,
	502000000,		// 25
	509500000,
	504000000,
	480000000,
	485500000,
	478500000,		// 30
	491000000,
	495500000,
	497500000,
	491500000,
	489500000,		// 35
	471500000,
	475500000,
	503000000,
	507000000,
	509000000		// 40
};

#elif ((Lora_RFFrequency >= 863000000) && (Lora_RFFrequency <= 870000000))		// for EU 863~870MHz
const uint32_t	Default_Hopping_Frequency_Hz[] = {
	870000000,		// 0
	865000000,		// 1
	866500000,
	864000000,
	868000000,
	869000000,		// 5
	868500000,
	864500000,
	867000000,
	865500000,
	863800000,		// 10
	865200000,
	869500000,
	864800000,
	866000000,
	867500000,		// 15
	868200000,
	863700000,
	869200000,
	863500000,
	865700000,		// 20
	866600000,
	864300000,
	867700000		// 23
};

#elif ((Lora_RFFrequency >= 779000000) && (Lora_RFFrequency <= 787000000))		// for china 779~787MHz
const uint32_t	Default_Hopping_Frequency_Hz[] = {
	775000000,		// 0
	779500000,		// 1
	781000000,
	786500000,
	785000000,
	783000000,		// 5
	781500000,
	782000000,
	786000000,
	780500000,
	783500000,		// 10
	785500000,
	782500000,
	782700000,
	781300000,
	786600000,		// 15
	785200000,
	779800000		// 17
};
#else
	#error		"Not Default_Hopping_Frequency_Hz[] in this Lora_RFFrequency"
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern bool		EnableMaster;		// 1=Master or 0=Slave selection
extern tLoRaSettings	LoRaSettings;
extern __IO uint8_t	SLAVE_LoraHoppingStartChannel;		// for SLAVE
__IO uint8_t		Running_HoppingStartChannel = 0;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void		RandomHopStartChannel_SetHoppingStartChannelFreq(uint8_t channel)
{
	if( ! LoRaSettings.FreqHopOn ) return;
	if( channel >= LoRaSettings.Channel_Lists ) return;

	if( Running_HoppingStartChannel != channel ) {
		Running_HoppingStartChannel = channel;
		SX1276LoRaSetRFFrequency( LoRaSettings.Channel_List[Running_HoppingStartChannel] );

		if( ! EnableMaster ) {
			char		str[24];
			snprintf( str, sizeof(str), "Freq %d KHz\r\n", LoRaSettings.Channel_List[Running_HoppingStartChannel] / 1000 );
			Console_Output_String( str );
		}
	}
}


void		RandomHopStartChannel_SlaveDefaultHoppingChannel(void)
{
	SLAVE_LoraHoppingStartChannel = 0;
	RandomHopStartChannel_SetHoppingStartChannelFreq( SLAVE_LoraHoppingStartChannel );
}

void	Clear_FHSS_Channel_List( void )
{
	LoRaSettings.Channel_Lists = 0;
	memset( LoRaSettings.Channel_List, 0x00, sizeof( LoRaSettings.Channel_List ) );
}


void	Load_Default_FHSS_Channel_List( void )
{
	uint8_t		i;

	Clear_FHSS_Channel_List();
	for( i = 0 ; i < sizeof( Default_Hopping_Frequency_Hz ) / sizeof( uint32_t ) ; i++ ) {
		if( LoRaSettings.Channel_Lists >= MAX_HOPPING_CHANNELS ) return;

		LoRaSettings.Channel_List[ LoRaSettings.Channel_Lists ] = Default_Hopping_Frequency_Hz[ i ];
		LoRaSettings.Channel_Lists++;
	}
}

void	List_FHSS_Channel_List( void )
{
	char		cs[64];
	uint8_t		i;

	Console_Output_String( "\r\nFHSS Channel List\r\n" );

	for( i = 0 ; i < LoRaSettings.Channel_Lists ; i++ ) {
		snprintf( cs, sizeof( cs ), "Channel[%d] = %d KHz\r\n", i, LoRaSettings.Channel_List[ i ] / 1000 );
		Console_Output_String( cs );
		Delay( 10 );
	}

	snprintf( cs, sizeof( cs ), "Total FHSS Channels = %d \r\n", i );
	Console_Output_String( cs );
}

bool	Append_FHSS_Channel_into_List( uint32_t freq_Hz )
{
	if( LoRaSettings.Channel_Lists + 1 < MAX_HOPPING_CHANNELS ) {
		LoRaSettings.Channel_List[ LoRaSettings.Channel_Lists ] = freq_Hz;
		LoRaSettings.Channel_Lists++;

		return( true );
	}

	Console_Output_String( "FHSS Channel List is full.\r\n" );
	return( false );
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
