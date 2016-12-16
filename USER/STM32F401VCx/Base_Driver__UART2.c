
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


/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>
#include "config.h"
#include "Base_Driver__UART2.h"
#include "stm32f4xx.h"

#ifdef USE_HARDWARE__USART2

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool			UART2__Enable = false;

Fn_UART2__RX_Callback	UART2__RX_Callback = NULL;

Byte_Queue_Struct	UART2__TX_Queue;
Byte_Queue_Struct *	UART2__RX_Queue = NULL;

/* Private function prototypes -----------------------------------------------*/
void	UART2__PinInitialization( void );
void	UART2__PinUnInitialization( void );
void	UART2__UartConfig( uint32_t BaudRate );
void	UART2__UartDeConfig( void );


/* Private functions ---------------------------------------------------------*/

void	UART2__Register_Callback( Fn_UART2__RX_Callback cb )
{
	UART2__RX_Callback = cb;
}
void	UART2__PinInitialization( void )
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	/* Pin configuration */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init( GPIOA, & GPIO_InitStructure );

	/* Alternate function conf */
	GPIO_PinAFConfig( GPIOA, GPIO_PinSource2, GPIO_AF_USART2 );
	GPIO_PinAFConfig( GPIOA, GPIO_PinSource3, GPIO_AF_USART2 );
}

void	UART2__PinUnInitialization( void )
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	/* Pin configuration */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOA, & GPIO_InitStructure );
}

void	UART2__UartConfig( uint32_t BaudRate )
{
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;

	/* Enable the USART2 clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );

	USART_DeInit( USART2 );

	/* USART conf */
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init( USART2, & USART_InitStructure );

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( & NVIC_InitStructure );

	/* Enable USART */
	USART_Cmd( USART2, ENABLE );
	USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );
	USART_ITConfig( USART2, USART_IT_TXE, ENABLE );
}

void	UART2__UartDeConfig( void )
{
	NVIC_InitTypeDef	NVIC_InitStructure;

	/* Disable USART */
	USART_Cmd( USART2, DISABLE );
	USART_ITConfig( USART2, USART_IT_RXNE, DISABLE );
	USART_ITConfig( USART2, USART_IT_TXE, DISABLE );

	/* Disable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init( & NVIC_InitStructure );

	USART_DeInit( USART2 );

	/* Disable the USART2 clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, DISABLE );
}

void	UART2__Init( Fn_UART2__RX_Callback cb, Byte_Queue_Struct *RX_Queue )
{
	Init_Byte_Queue( & UART2__TX_Queue, UART2__TX_Buffer_Size );
	UART2__RX_Queue = RX_Queue;
	UART2__RX_Callback = cb;
}

void	UART2__Open( uint32_t BaudRate )
{
	UART2__PinInitialization();
	UART2__UartConfig( BaudRate );
	UART2__Enable = true;
}

void	UART2__Close( void )
{
	UART2__UartDeConfig();
	UART2__PinUnInitialization();
	UART2__Enable = false;
}

void	UART2__UartWrite( const uint8_t * Str, uint32_t Length )
{
	uint32_t	count;

	if( UART2__Enable ) {
		for( count = 0 ; count < Length ; count++ ) {
			Put_Byte_into_Queue( & UART2__TX_Queue, Str[count] );
		}
	}

	USART_ITConfig( USART2, USART_IT_TXE, ENABLE );
}

void	USART2_IRQHandler( void )
{
	uint8_t		d;

	if( USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET ) {
		/* Read one byte from the receive data register */
		d = USART_ReceiveData( USART2 );
		if( UART2__RX_Callback ) {
			UART2__RX_Callback( d );
		} else {
			Put_Byte_into_Queue( UART2__RX_Queue, d );
		}

		/* Clear the USART2 Receive interrupt */
		USART_ClearITPendingBit( USART2, USART_IT_RXNE );
	}

	if( USART_GetITStatus( USART2, USART_IT_TXE ) == SET ) {
		if( ! Get_Byte_from_Queue( & UART2__TX_Queue ) ) {
			USART_ITConfig( USART2, USART_IT_TXE, DISABLE );
			return;
		}

		USART_SendData( USART2, UART2__TX_Queue.Output );
	}
}

#endif		// USE_HARDWARE__USART2

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
