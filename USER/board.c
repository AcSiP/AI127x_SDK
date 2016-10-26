
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

/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       board.c
 * \brief        
 *
 * \version    1.0
 * \date       Nov 21 2012
 * \author     Miguel Luis
 */
#include <stdint.h>
#include "spi.h"
#include "board.h"

// System tick (1ms)
volatile uint32_t	TickCounter = 0;


#ifdef STM32F401xx
#define ADC1_DR_ADDRESS		((uint32_t)0x4001204C)

__IO uint16_t		ADC1ConvertedValue = 0;

static void		ADC1_CH8_DMA_Config(void);
#endif


void	BoardInit( void )
{
	/* Setup SysTick Timer for 1 ms interrupts ( not too often to save power ) */
	if( SysTick_Config( SystemCoreClock / 1000 ) ) {		// SystemCoreClock / 1000 = 1ms
		/* Capture error */
		while (1) {
		}
	}

	// Initialize unused GPIO to optimize power consumption
	// InitUnusedGPIO( );

	// Initialize SPI
	SpiInit( );

#ifdef STM32F401xx
	// Init. ADC1
	ADC1_CH8_DMA_Config();

	// Start ADC1 Software Conversion
	ADC_SoftwareStartConv(ADC1);
#endif
}

#ifdef STM32F401xx
static void	ADC1_CH8_DMA_Config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;
	ADC_CommonInitTypeDef	ADC_CommonInitStructure;
	DMA_InitTypeDef		DMA_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;

	/* Enable ADC3, DMA2 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOB, ENABLE);		// 0x00400000 | 0x00000004, AHB1 up to 84MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);		// 0x00000100, APB2 up to 84Mhz

	/* DMA2 Stream0 channel0 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC1ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* Configure ADC1 Channel12 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;		// ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel12 configuration *************************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_144Cycles);

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}
#endif


void	Delay( uint32_t delay )
{
	// Wait delay ms
	uint32_t startTick = TickCounter;
	while( ( TickCounter - startTick ) < delay ) {
	}
}

void	LongDelay( uint8_t delay )
{
	uint32_t	longDelay;
	uint32_t	startTick;

	longDelay = delay * 1000;

	// Wait delay s
	startTick = TickCounter;
	while( ( TickCounter - startTick ) < longDelay ) {
	}
}

/*
void	InitUnusedGPIO( void ) 
{   
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); // enable clock of the IO bank A
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); // enable clock of the IO bank B
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); // enable clock of the IO bank C

	GPIO_StructInit( & GPIO_InitStructure );
	// activate pullups on C8 to C12 (SDIO)
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// -> save 10 to 50 渙

	// activate pullups on B10 and B11 (I涎)
	//GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	//GPIO_Init(GPIOB, &GPIO_InitStructure);
	// -> dubious influence (might increase power a little bit)

	// activate pulldowns on B6, B7 and B12 to B15 (J2 and J5)
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// very little influence (<<5 渙)

	// activate pulldowns on A11 and A12 (USB)
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// very little influence (<<5 渙)

	// activate pulldowns on A10, B0, B1, C3 and C13 (unused pins)
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// little influence? (~10 渙)
}
*/

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
