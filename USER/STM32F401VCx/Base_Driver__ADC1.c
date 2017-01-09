
//---------------------------------------------------------------------------
/*
//==========================================
// Author : Fox<shenfa@acsip.com.tw>
// Copyright 2017(C) AcSiP Technology Inc.
// 版權所有：群登科技股份有限公司
// http://www.acsip.com.tw
//==========================================
*/
//---------------------------------------------------------------------------


/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

#include "config.h"
#include "Base_Driver__ADC1.h"
#include "stm32f4xx.h"

#ifdef USE_HARDWARE__ADC1

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_ADDRESS		( (uint32_t) 0x4001204C )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t		ADC1_Converted_Value[16];
uint16_t		ADC1_Current_Configuration;

bool			ADC1__Enable = false;



/* Private function prototypes -----------------------------------------------*/
uint8_t		ADC1__Count_Items( uint16_t ch_sel );
int8_t		ADC1__Get_Item_Index( uint16_t ch_sel );
void		ADC1__Enable_CLK( uint16_t ch_sel );
void		ADC1__Configure_GPIO( uint16_t ch_sel );
void		ADC1__DMA_Configure( uint16_t ch_sel );
void		ADC1__ADC_Configure( uint16_t ch_sel );

/* Private functions ---------------------------------------------------------*/
uint8_t		ADC1__Count_Items( uint16_t ch_sel )
{
	uint8_t		n = 0;

	if( ch_sel & ADC_IDX___ADC0 ) n++;
	if( ch_sel & ADC_IDX___ADC1 ) n++;
	if( ch_sel & ADC_IDX___ADC4 ) n++;
	if( ch_sel & ADC_IDX___ADC5 ) n++;
	if( ch_sel & ADC_IDX___ADC6 ) n++;
	if( ch_sel & ADC_IDX___ADC7 ) n++;

	if( ch_sel & ADC_IDX___ADC8 ) n++;

	if( ch_sel & ADC_IDX___Vref ) n++;
	if( ch_sel & ADC_IDX___VBat ) n++;
	if( ch_sel & ADC_IDX___VTemp ) n++;

	return( n );
}


int8_t		ADC1__Get_Item_Index( uint16_t ch_sel )
{
	int8_t		n = 0;

	if( ADC1_Current_Configuration & ADC_IDX___ADC0 ){
		if( ch_sel == ADC_IDX___ADC0 ) return( n );
		n++;
	}

	if( ADC1_Current_Configuration & ADC_IDX___ADC1 ){
		if( ch_sel == ADC_IDX___ADC1 ) return( n );
		n++;
	}

	if( ADC1_Current_Configuration & ADC_IDX___ADC4 ){
		if( ch_sel == ADC_IDX___ADC4 ) return( n );
		n++;
	}

	if( ADC1_Current_Configuration & ADC_IDX___ADC5 ){
		if( ch_sel == ADC_IDX___ADC5 ) return( n );
		n++;
	}

	if( ADC1_Current_Configuration & ADC_IDX___ADC6 ){
		if( ch_sel == ADC_IDX___ADC6 ) return( n );
		n++;
	}

	if( ADC1_Current_Configuration & ADC_IDX___ADC7 ){
		if( ch_sel == ADC_IDX___ADC7 ) return( n );
		n++;
	}

	if( ADC1_Current_Configuration & ADC_IDX___ADC8 ){
		if( ch_sel == ADC_IDX___ADC8 ) return( n );
		n++;
	}

	if( ADC1_Current_Configuration & ADC_IDX___Vref ){
		if( ch_sel == ADC_IDX___Vref ) return( n );
		n++;
	}

	if( ADC1_Current_Configuration & ADC_IDX___VBat ){
		if( ch_sel == ADC_IDX___VBat ) return( n );
		n++;
	}

	if( ADC1_Current_Configuration & ADC_IDX___VTemp ){
		if( ch_sel == ADC_IDX___VTemp ) return( n );
		n++;
	}

	return( -1 );
}


uint16_t	ADC1__Get_Converted_Value( uint16_t ch_sel )
{
	int8_t		i;
	char		cs[64];

	i = ADC1__Get_Item_Index( ch_sel );
	snprintf( cs, sizeof( cs ), "%d, i = %d \r\n", __LINE__, i );
	Console_Output_String( cs );
	if( 0 <= i && i < 16 ) return( ADC1_Converted_Value[ i ] & 0x0FFF );

	return( 0 );
}


void		ADC1__Enable_CLK( uint16_t ch_sel )
{
	bool	flag_port_A = false;
	bool	flag_port_B = false;

	if( ch_sel & ADC_IDX___ADC0 ) flag_port_A = true;
	if( ch_sel & ADC_IDX___ADC1 ) flag_port_A = true;
	if( ch_sel & ADC_IDX___ADC4 ) flag_port_A = true;
	if( ch_sel & ADC_IDX___ADC5 ) flag_port_A = true;
	if( ch_sel & ADC_IDX___ADC6 ) flag_port_A = true;
	if( ch_sel & ADC_IDX___ADC7 ) flag_port_A = true;

	if( ch_sel & ADC_IDX___ADC8 ) flag_port_B = true;

	if( flag_port_A ) RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
	if( flag_port_B ) RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
}

void		ADC1__Configure_GPIO( uint16_t ch_sel )
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	if( ch_sel & ADC_IDX___ADC0 ){	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	GPIO_Init( GPIOA, & GPIO_InitStructure );	}
	if( ch_sel & ADC_IDX___ADC1 ){	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	GPIO_Init( GPIOA, & GPIO_InitStructure );	}
	if( ch_sel & ADC_IDX___ADC4 ){	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	GPIO_Init( GPIOA, & GPIO_InitStructure );	}
	if( ch_sel & ADC_IDX___ADC5 ){	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	GPIO_Init( GPIOA, & GPIO_InitStructure );	}
	if( ch_sel & ADC_IDX___ADC6 ){	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	GPIO_Init( GPIOA, & GPIO_InitStructure );	}
	if( ch_sel & ADC_IDX___ADC7 ){	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	GPIO_Init( GPIOA, & GPIO_InitStructure );	}

	if( ch_sel & ADC_IDX___ADC8 ){	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	GPIO_Init( GPIOB, & GPIO_InitStructure );	}
}


void		ADC1__DMA_Configure( uint16_t ch_sel )
{
	DMA_Cmd( DMA2_Stream0, DISABLE );
	DMA_DeInit( DMA2_Stream0 );
	if( ! ch_sel ) return;

	DMA_InitTypeDef		DMA_InitStructure;

	// DMA2 Stream 0 channel 0 configuration
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) ADC1_Converted_Value;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = ADC1__Count_Items( ch_sel );
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init( DMA2_Stream0, & DMA_InitStructure );
	DMA_Cmd( DMA2_Stream0, ENABLE );
}


void		ADC1__ADC_Configure( uint16_t ch_sel )
{
	ADC_DMACmd( ADC1, DISABLE );
	ADC_Cmd( ADC1, DISABLE );
	if( ! ch_sel ) return;

	ADC_InitTypeDef		ADC_InitStructure;
	ADC_CommonInitTypeDef	ADC_CommonInitStructure;

	// ADC Common Init
	// SYSCLK = 72M,	APB2_CLK = 36M,		ADC_CLK = 18M
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit( & ADC_CommonInitStructure );

	// ADC1 Init
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init( ADC1, & ADC_InitStructure );


	// ADC1 regular channel configuration
	int8_t		n = 1;
	if( ch_sel & ADC_IDX___ADC0 ) ADC_RegularChannelConfig( ADC1, ADC_Channel_0, n, ADC_SampleTime_3Cycles );

	n++;
	if( ch_sel & ADC_IDX___ADC1 ) ADC_RegularChannelConfig( ADC1, ADC_Channel_1, n, ADC_SampleTime_3Cycles );

	n++;
	if( ch_sel & ADC_IDX___ADC4 ) ADC_RegularChannelConfig( ADC1, ADC_Channel_4, n, ADC_SampleTime_3Cycles );

	n++;
	if( ch_sel & ADC_IDX___ADC5 ) ADC_RegularChannelConfig( ADC1, ADC_Channel_5, n, ADC_SampleTime_3Cycles );

	n++;
	if( ch_sel & ADC_IDX___ADC6 ) ADC_RegularChannelConfig( ADC1, ADC_Channel_6, n, ADC_SampleTime_3Cycles );

	n++;
	if( ch_sel & ADC_IDX___ADC7 ) ADC_RegularChannelConfig( ADC1, ADC_Channel_7, n, ADC_SampleTime_3Cycles );

	n++;
	if( ch_sel & ADC_IDX___ADC8 ) ADC_RegularChannelConfig( ADC1, ADC_Channel_8, n, ADC_SampleTime_3Cycles );

	n++;
	if( ch_sel & ADC_IDX___Vref ){
		ADC_RegularChannelConfig( ADC1, ADC_Channel_17, n, ADC_SampleTime_3Cycles );
		ADC_TempSensorVrefintCmd( ENABLE );
	}

	n++;
	if( ch_sel & ADC_IDX___VBat ){
		ADC_RegularChannelConfig( ADC1, ADC_Channel_18, n, ADC_SampleTime_3Cycles );
		ADC_VBATCmd( ENABLE );
	}

	n++;
	if( ch_sel & ADC_IDX___VTemp ){
		ADC_RegularChannelConfig( ADC1, ADC_Channel_17, n, ADC_SampleTime_3Cycles );
		ADC_TempSensorVrefintCmd( ENABLE );
		ADC_VBATCmd( DISABLE );
	}
	ADC1_Current_Configuration = ch_sel;
}


void		ADC1__Configure_w_DMA( uint16_t ch_sel )
{
	ADC1__Enable_CLK( ch_sel );
	ADC1__Configure_GPIO( ch_sel );
	ADC1__DMA_Configure( ch_sel );
	ADC1__ADC_Configure( ch_sel );

	if( ! ch_sel ){
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, DISABLE );
		return;
	}

	// Enable DMA request after last transfer (Single-ADC mode)
	ADC_DMARequestAfterLastTransferCmd( ADC1, ENABLE );

	// Enable ADC1 DMA
	ADC_DMACmd( ADC1, ENABLE );

	// Enable ADC1
	ADC_Cmd( ADC1, ENABLE );
}

#endif		// USE_HARDWARE__ADC1

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
