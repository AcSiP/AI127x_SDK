
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
#include "Base_Driver__UART1.h"
#include "stm32f4xx.h"

#ifdef USE_HARDWARE__USART1

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool			UART1__Enable = false;

Fn_UART1__RX_Callback	UART1__RX_Callback = NULL;

Byte_Queue_Struct	UART1__TX_Queue;
Byte_Queue_Struct *	UART1__RX_Queue = NULL;

/* Private function prototypes -----------------------------------------------*/
void	UART1__PinInitialization( void );
void	UART1__PinUnInitialization( void );
void	UART1__UartConfig( uint32_t BaudRate );
void	UART1__UartDeConfig( void );


/* Private functions ---------------------------------------------------------*/

void	UART1__Register_Callback( Fn_UART1__RX_Callback cb )
{
	UART1__RX_Callback = cb;
}

void	UART1__PinInitialization( void )
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	/* Pin configuration */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init( GPIOB, & GPIO_InitStructure );

	/* Alternate function conf */
	GPIO_PinAFConfig( GPIOB, GPIO_PinSource6, GPIO_AF_USART1 );
	GPIO_PinAFConfig( GPIOB, GPIO_PinSource7, GPIO_AF_USART1 );
}

void	UART1__PinUnInitialization( void )
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	/* Pin configuration */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOB, & GPIO_InitStructure );
}

void	UART1__UartConfig( uint32_t BaudRate )
{
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;

	/* Enable the USART1 clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );

	USART_DeInit( USART1 );

	/* USART conf */
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init( USART1, & USART_InitStructure );

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( & NVIC_InitStructure );

	/* Enable USART */
	USART_Cmd( USART1, ENABLE );
	USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
	USART_ITConfig( USART1, USART_IT_TXE, ENABLE );
}

void	UART1__UartDeConfig( void )
{
	NVIC_InitTypeDef	NVIC_InitStructure;

	/* Disable USART */
	USART_Cmd( USART1, DISABLE );
	USART_ITConfig( USART1, USART_IT_RXNE, DISABLE );
	USART_ITConfig( USART1, USART_IT_TXE, DISABLE );

	/* Disable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init( & NVIC_InitStructure );

	USART_DeInit( USART1 );

	/* Disable the USART2 clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, DISABLE );
}

void	UART1__Init( Fn_UART1__RX_Callback cb, Byte_Queue_Struct *RX_Queue )
{
	Init_Byte_Queue( & UART1__TX_Queue, UART1__TX_Buffer_Size );
	UART1__RX_Queue = RX_Queue;
	UART1__RX_Callback = cb;
}

void	UART1__Open( uint32_t BaudRate )
{
	UART1__PinInitialization();
	UART1__UartConfig( BaudRate );
	UART1__Enable = true;
}

void	UART1__Close( void )
{
	UART1__UartDeConfig();
	UART1__PinUnInitialization();
	UART1__Enable = false;
}

void	UART1__UartWrite( const uint8_t * Str, uint32_t Length )
{
	uint32_t	count;

	if( UART1__Enable ) {
		for( count = 0 ; count < Length ; count++ ) {
			Put_Byte_into_Queue( & UART1__TX_Queue, Str[count] );
		}
	}

	USART_ITConfig( USART1, USART_IT_TXE, ENABLE );
}

void	USART1_IRQHandler( void )
{
	uint8_t		d;

	if( USART_GetITStatus( USART1, USART_IT_RXNE ) != RESET ) {
		/* Read one byte from the receive data register */
		d = USART_ReceiveData( USART1 );
		if( UART1__RX_Callback ) {
			UART1__RX_Callback( d );
		} else {
			Put_Byte_into_Queue( UART1__RX_Queue, d );
		}

		/* Clear the USART1 Receive interrupt */
		USART_ClearITPendingBit( USART1, USART_IT_RXNE );
	}

	if( USART_GetITStatus( USART1, USART_IT_TXE ) == SET ) {
		if( ! Get_Byte_from_Queue( & UART1__TX_Queue ) ) {
			USART_ITConfig( USART1, USART_IT_TXE, DISABLE );
			return;
		}

		USART_SendData( USART1, UART1__TX_Queue.Output );
	}
}

#endif		// USE_HARDWARE__USART1

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
