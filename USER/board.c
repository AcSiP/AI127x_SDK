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
//Program_Sample
#include "config.h"
//Program_Sample end

// System tick (1ms)
volatile uint32_t	TickCounter = 0;

//Program_Sample
uint32_t	LEDBlinkStartTime = 0;
uint32_t	LEDLoop = 1;
uint32_t	gOnTime = 0;
uint32_t	gOffTime = 0;
uint32_t	gDuration = 0;
bool		isLEDOn = false;
//Program_Sample end

//void InitUnusedGPIO( void ) ;

void	BoardInit( void )
{
	/* Setup SysTick Timer for 1 ms interrupts ( not too often to save power ) */
	if( SysTick_Config( SystemCoreClock / 1000 ) ){    // SystemCoreClock / 1000 = 1ms

		/* Capture error */ 
		while (1);
	}

	// Initialize unused GPIO to optimize power consumption
	//InitUnusedGPIO( );

	// Initialize SPI
	SpiInit( );
}

void	Delay( uint32_t delay )
{
	// Wait delay ms
	uint32_t startTick = TickCounter;
	while( ( TickCounter - startTick ) < delay );   
}

void	LongDelay( uint8_t delay )
{
	uint32_t	longDelay;
	uint32_t	startTick;

	longDelay = delay * 1000;

	// Wait delay s
	startTick = TickCounter;
	while( ( TickCounter - startTick ) < longDelay );   
}

//Program_Sample
void	LEDBlink( uint32_t onTime, uint32_t offTime, uint32_t duration )
{
	if((LEDBlinkStartTime == 0) || (TickCounter > LEDBlinkStartTime + gDuration)) {
		LEDBlinkStartTime = TickCounter;
		gOnTime = onTime;
		gOffTime = offTime;
		gDuration = duration;
		CmdUART_UartWrite((uint8_t *)"New LED action start", strlen("New LED action start"));
		CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
		}
	else {
		if(TickCounter < LEDBlinkStartTime + gDuration) {
			if(TickCounter < (LEDBlinkStartTime + ((gOffTime+gOnTime)*LEDLoop))) {
				if(TickCounter < (LEDBlinkStartTime + ((gOnTime*LEDLoop)+(gOffTime*(LEDLoop-1))))) {
					//LED GPIO on
					if(!isLEDOn) {
						CmdUART_UartWrite((uint8_t *)"LED GPIO on", strlen("LED GPIO on"));
						CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
						//Add LED GPIO control here
						isLEDOn = true;
						}
					}
				else {
					//LED GPIO off
					if(isLEDOn) {
						CmdUART_UartWrite((uint8_t *)"LED GPIO off", strlen("LED GPIO off"));
						CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
						//Add LED GPIO control here
						isLEDOn = false;
						}
					}
				}
			else {
				LEDLoop++;
				}
			}
		else {
			LEDBlinkStartTime = 0;
			LEDLoop = 1;
			gOnTime = 0;
			gOffTime = 0;
			gDuration = 0;
			isLEDOn = false;
			}
		}
}
//Program_Sample end

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
	// -> save 10 to 50 µA

	// activate pullups on B10 and B11 (I²C)
	//GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	//GPIO_Init(GPIOB, &GPIO_InitStructure);
	// -> dubious influence (might increase power a little bit)

	// activate pulldowns on B6, B7 and B12 to B15 (J2 and J5)
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// very little influence (<<5 µA)

	// activate pulldowns on A11 and A12 (USB)
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// very little influence (<<5 µA)

	// activate pulldowns on A10, B0, B1, C3 and C13 (unused pins)
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// little influence? (~10 µA)
}
*/


