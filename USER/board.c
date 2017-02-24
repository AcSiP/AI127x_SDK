
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

#include "config.h"

#include "Base_Driver__ADC1.h"


// System tick (1ms)
volatile uint32_t	TickCounter = 0;

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


	ADC1__Configure_w_DMA(	ADC_IDX___ADC0 |
				ADC_IDX___ADC1 |
				ADC_IDX___ADC4 |
				ADC_IDX___VBat );

#ifdef STM32F401xx
	// Start ADC1 Software Conversion
	ADC_SoftwareStartConv( ADC1 );
#endif

#ifdef STM32F072
	ADC_StartOfConversion( ADC1 );
#endif
}

void	Board_DeInit( void )
{
	if( SysTick_Config( SystemCoreClock / 20 ) ) {		// SystemCoreClock / 20 = 50ms
		/* Capture error */
		while (1) {
		}
	}

	ADC1__DeInit();
	SPI_De_Init();
}

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
