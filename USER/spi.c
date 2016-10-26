
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
 * \file       spi.c
 * \brief      SPI hardware driver
 *
 * \version    1.0
 * \date       Feb 12 2010
 * \author     Miguel Luis
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "spi.h"
#include "console_function.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


#define SPI_INTERFACE			SPI2
#define SPI_CLK				RCC_APB1Periph_SPI2

#if defined( STM32F401xx )
// 	#define SPI_PIN_NSS_PORT		GPIOB
// 	#define SPI_PIN_NSS_PORT_CLK		RCC_AHB1Periph_GPIOB
// 	#define SPI_PIN_NSS			GPIO_Pin_12
// 	#define SPI_PIN_NSS_AF_SOURCE		GPIO_PinSource12
// 	#define SPI_PIN_NSS_AF			GPIO_AF_SPI2

	#define SPI_PIN_SCK_PORT		GPIOB
	#define SPI_PIN_SCK_PORT_CLK		RCC_AHB1Periph_GPIOB
	#define SPI_PIN_SCK			GPIO_Pin_13
	#define SPI_PIN_SCK_AF_SOURCE		GPIO_PinSource13
	#define SPI_PIN_SCK_AF			GPIO_AF_SPI2

	#define SPI_PIN_MISO_PORT		GPIOB
	#define SPI_PIN_MISO_PORT_CLK		RCC_AHB1Periph_GPIOB
	#define SPI_PIN_MISO			GPIO_Pin_14
	#define SPI_PIN_MISO_AF_SOURCE		GPIO_PinSource14
	#define SPI_PIN_MISO_AF			GPIO_AF_SPI2

	#define SPI_PIN_MOSI_PORT		GPIOB
	#define SPI_PIN_MOSI_PORT_CLK		RCC_AHB1Periph_GPIOB
	#define SPI_PIN_MOSI			GPIO_Pin_15
	#define SPI_PIN_MOSI_AF_SOURCE		GPIO_PinSource15
	#define SPI_PIN_MOSI_AF			GPIO_AF_SPI2
#elif defined( STM32F072 )
	#define SPI_PIN_SCK_PORT		GPIOB
	#define SPI_PIN_SCK_PORT_CLK		RCC_AHBPeriph_GPIOB
	#define SPI_PIN_SCK			GPIO_Pin_13
	#define SPI_PIN_SCK_AF_SOURCE		GPIO_PinSource13
	#define SPI_PIN_SCK_AF			GPIO_AF_0

	#define SPI_PIN_MISO_PORT		GPIOB
	#define SPI_PIN_MISO_PORT_CLK		RCC_AHBPeriph_GPIOB
	#define SPI_PIN_MISO			GPIO_Pin_14
	#define SPI_PIN_MISO_AF_SOURCE		GPIO_PinSource14
	#define SPI_PIN_MISO_AF			GPIO_AF_0

	#define SPI_PIN_MOSI_PORT		GPIOB
	#define SPI_PIN_MOSI_PORT_CLK		RCC_AHBPeriph_GPIOB
	#define SPI_PIN_MOSI			GPIO_Pin_15
	#define SPI_PIN_MOSI_AF_SOURCE		GPIO_PinSource15
	#define SPI_PIN_MOSI_AF			GPIO_AF_0
#else
	#error "Missing define MCU type (STM32F072 or STM32F401xx)"
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


void	SpiInit( void )
{
	SPI_InitTypeDef		SPI_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;

	SPI_Cmd( SPI_INTERFACE, DISABLE );
	RCC_APB1PeriphClockCmd( SPI_CLK, DISABLE );
	SPI_I2S_DeInit( SPI_INTERFACE );

	/* Enable peripheral clocks --------------------------------------------------*/

#if defined( STM32F401xx )
	RCC_AHB1PeriphClockCmd( SPI_PIN_MISO_PORT_CLK | SPI_PIN_MOSI_PORT_CLK | SPI_PIN_SCK_PORT_CLK, ENABLE );
#elif defined( STM32F072 )
	RCC_AHBPeriphClockCmd( SPI_PIN_MISO_PORT_CLK | SPI_PIN_MOSI_PORT_CLK | SPI_PIN_SCK_PORT_CLK, ENABLE );
#endif

	RCC_APB1PeriphClockCmd( SPI_CLK, ENABLE );


	/* GPIO configuration ------------------------------------------------------*/
// 	GPIO_PinAFConfig( SPI_PIN_NSS_PORT, SPI_PIN_NSS_AF_SOURCE, SPI_PIN_NSS_AF );
	GPIO_PinAFConfig( SPI_PIN_SCK_PORT, SPI_PIN_SCK_AF_SOURCE, SPI_PIN_SCK_AF );
	GPIO_PinAFConfig( SPI_PIN_MOSI_PORT, SPI_PIN_MOSI_AF_SOURCE, SPI_PIN_MISO_AF );
	GPIO_PinAFConfig( SPI_PIN_MISO_PORT, SPI_PIN_MISO_AF_SOURCE, SPI_PIN_MOSI_AF );

#if defined( STM32F401xx )
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#elif defined( STM32F072 )
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
#endif

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

	GPIO_InitStructure.GPIO_Pin = SPI_PIN_SCK;
	GPIO_Init( SPI_PIN_SCK_PORT, & GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = SPI_PIN_MOSI;
	GPIO_Init( SPI_PIN_MOSI_PORT, & GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = SPI_PIN_MISO;
	GPIO_Init( SPI_PIN_MISO_PORT, & GPIO_InitStructure );


// 	GPIO_InitStructure.GPIO_Pin = SPI_PIN_NSS;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init( SPI_PIN_NSS_PORT, & GPIO_InitStructure );
// 	GPIO_SetBits( SPI_PIN_NSS_PORT, SPI_PIN_NSS );


	/* SPI_INTERFACE Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;

	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
// 	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	// SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		// 10 MHz
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init( SPI_INTERFACE, &SPI_InitStructure );
#if defined( STM32F072 )
	SPI_RxFIFOThresholdConfig( SPI_INTERFACE, SPI_RxFIFOThreshold_QF );
#endif
// 	SPI_SSOutputCmd( SPI_INTERFACE, ENABLE );
	SPI_Cmd( SPI_INTERFACE, ENABLE );
}

uint8_t		SpiInOut( uint8_t outData )
{
	uint8_t		d;

	while( SPI_I2S_GetFlagStatus( SPI_INTERFACE, SPI_I2S_FLAG_TXE ) == RESET ) {
	}

// 	GPIO_ResetBits( SPI_PIN_NSS_PORT, SPI_PIN_NSS );

#if defined( STM32F401xx )
	SPI_I2S_SendData( SPI_INTERFACE, (uint16_t)outData );
#elif defined( STM32F072 )
	SPI_SendData8( SPI_INTERFACE, outData );
#endif


	while( SPI_I2S_GetFlagStatus( SPI_INTERFACE, SPI_I2S_FLAG_RXNE ) == RESET ) {
	}

#if defined( STM32F401xx )
	d = SPI_I2S_ReceiveData( SPI_INTERFACE );
#elif defined( STM32F072 )
	d = SPI_ReceiveData8( SPI_INTERFACE );
#endif

// 	GPIO_SetBits( SPI_PIN_NSS_PORT, SPI_PIN_NSS );
	return( d );
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/

