
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
//#include "Resource.h"
//#include "Common_Functions.h"

#include "board.h"

#include "i2c1.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t	I2C_Master__TimeoutTimeStamp;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void	I2C_Master__Reset_Timeout_Tick( void );
bool	I2C_Master__Assert_Timeout_w_STOP( void );

/**
 *	External interrupts example
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

//#ifdef I2C_interrupt_enable
/* Configure pins to be interrupts */
void Configure_IRQ_PA7(void) {
	/* Set variables used */
	Console_Output_String( "Configure_PA7 as interrupt!!\r\n" );
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/* Enable clock for GPIOA */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* Enable clock for SYSCFG */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Set pin as input */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* Tell system that you will use PA12 for EXTI_Line12 */
	/* Connect EXTI Line0 to PA0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);
	/* PA12 is connected to EXTI_Line7 */
	EXTI_InitStruct.EXTI_Line = EXTI_Line7;
	/* Enable interrupt */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	/* Interrupt mode */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	/* Triggers on rising and falling edge */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	/* Add to EXTI */
	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	/* PA7 is connected to EXTI_Line7, which has EXTI7_IRQn vector */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
	
		/* Clear the EXTI line 0 pending bit */
	EXTI_ClearITPendingBit(EXTI_Line7);
}
void Configure_PA0(void) {
	/* Set variables used */
	Console_Output_String( "Configure_PA0 as interrupt!!\r\n" );
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/* Enable clock for GPIOA */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* Enable clock for SYSCFG */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Set pin as input */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* Tell system that you will use PA12 for EXTI_Line12 */
	/* Connect EXTI Line0 to PA0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	/* PA12 is connected to EXTI_Line7 */
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	/* Enable interrupt */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	/* Interrupt mode */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	/* Triggers on rising and falling edge */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	/* Add to EXTI */
	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	/* PA7 is connected to EXTI_Line7, which has EXTI7_IRQn vector */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
	
		/* Clear the EXTI line 0 pending bit */
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void Configure_PB12(void) {
	/* Set variables used */
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/* Enable clock for GPIOB */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* Enable clock for SYSCFG */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* Set pin as input */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/* Tell system that you will use PB12 for EXTI_Line12 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);
	
	/* PB12 is connected to EXTI_Line12 */
	EXTI_InitStruct.EXTI_Line = EXTI_Line12;
	/* Enable interrupt */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	/* Interrupt mode */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	/* Triggers on rising and falling edge */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	/* Add to EXTI */
	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	/* PB12 is connected to EXTI_Line12, which has EXTI15_10_IRQn vector */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
}


void	I2C_Master__Init( uint8_t device_address )
{ 
	I2C_InitTypeDef		I2C_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd( MCU_I2C__SDA_CLK, ENABLE );
	RCC_AHB1PeriphClockCmd( MCU_I2C__SCL_CLK, ENABLE );

	/* Configure SCL */
	GPIO_InitStructure.GPIO_Pin = MCU_I2C__SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//GPIO_PuPd_NOPULL;
	GPIO_Init( MCU_I2C__SCL_PORT, & GPIO_InitStructure );
  
	/* Configure SDA */
	GPIO_InitStructure.GPIO_Pin = MCU_I2C__SDA_PIN;
	GPIO_Init( MCU_I2C__SDA_PORT, & GPIO_InitStructure );

	/* Connect PB8 and PB9 to I2C*/
	GPIO_PinAFConfig( MCU_I2C__SCL_PORT, MCU_I2C__SCL_SOURCE, MCU_I2C__SCL_AF );
	GPIO_PinAFConfig( MCU_I2C__SDA_PORT, MCU_I2C__SDA_SOURCE, MCU_I2C__SDA_AF );

	/* Periph clock enable */
	RCC_APB1PeriphClockCmd( MCU_I2C__CLK, ENABLE );
	I2C_Cmd( MCU_I2C__MASTER, DISABLE );
	I2C_DeInit( MCU_I2C__MASTER );

	/* Configure I2C for communication */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = device_address;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 10 * 1000;

	I2C_Init( MCU_I2C__MASTER, & I2C_InitStructure );
	I2C_Cmd( MCU_I2C__MASTER, ENABLE );
}

void	I2C_Master__Reset_Timeout_Tick( void )
{
	do{
		I2C_Master__TimeoutTimeStamp = TickCounter + MCU_I2C__TIMEOUT_MS;
	} while ( TickCounter >= I2C_Master__TimeoutTimeStamp );
}

bool	I2C_Master__Assert_Timeout_w_STOP( void )
{
	if( TickCounter < I2C_Master__TimeoutTimeStamp ) return( false );

	I2C_GenerateSTOP( MCU_I2C__MASTER, ENABLE );
	return( true );
}

bool	I2C_Master__Write( uint8_t Dev_Addr, uint8_t Reg_Addr, uint16_t NumByteToWrite, const uint8_t * pBuffer )
{
//	I2C_Master__Init( Dev_Addr );
	I2C_Master__Reset_Timeout_Tick();

	/* Send START condition */
	I2C_GenerateSTART( MCU_I2C__MASTER, ENABLE );
	do{	// Test on EV5 and clear it
		if( I2C_Master__Assert_Timeout_w_STOP() ){
//			Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_MODE_SELECT ) );		//BUSY, MSL, SB flag


	/* Send slave address for write */
	I2C_Send7bitAddress( MCU_I2C__MASTER, Dev_Addr, I2C_Direction_Transmitter );
//	I2C_Master__Reset_Timeout_Tick();
	do{	//Test on EV6 and clear it
		if( I2C_Master__Assert_Timeout_w_STOP() ){
			I2C_ClearFlag( MCU_I2C__MASTER, I2C_FLAG_BUSY | I2C_FLAG_AF );		//AF = ACK Failuare
//			Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );	//BUSY, MSL, ADDR, TXE and TRA



	/* Transmit the first address for r/w operations */
	I2C_Master__Reset_Timeout_Tick();
	I2C_SendData( MCU_I2C__MASTER, Reg_Addr );

	/* Test on EV8 and clear it */
	do{
		if( I2C_Master__Assert_Timeout_w_STOP() ){
//			Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );	//TRA(TxRx Activity), BUSY, MSL, TXE, BTF(Byte Transfer Finished)


	while( NumByteToWrite ){
		I2C_Master__Reset_Timeout_Tick();

		I2C_SendData( MCU_I2C__MASTER, *pBuffer );
		pBuffer++;
		NumByteToWrite--;

		/* Test on EV8 and clear it */
		do{
			if( I2C_Master__Assert_Timeout_w_STOP() ){
//				Console_Output_Item_and_Integer( __FILE__, __LINE__ );
				return( false );
			}
		} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
	}

	I2C_GenerateSTOP( MCU_I2C__MASTER, ENABLE );
//	Console_Output_Item_and_Integer( __FILE__, __LINE__ );
	return true;
}

bool	I2C_Master__Write_Raw( uint8_t Dev_Addr, uint16_t NumByteToWrite, const uint8_t * pBuffer )
{
	/* Send START condition */
	I2C_GenerateSTART( MCU_I2C__MASTER, ENABLE );
	I2C_Master__Reset_Timeout_Tick();

	do{	// Test on EV5 and clear it
		if( I2C_Master__Assert_Timeout_w_STOP() ){
			//Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_MODE_SELECT ) );		//BUSY, MSL, SB flag


	/* Send slave address for write */
	I2C_Send7bitAddress( MCU_I2C__MASTER, Dev_Addr, I2C_Direction_Transmitter );
//	I2C_Master__Reset_Timeout_Tick();
	do{	//Test on EV6 and clear it
		if( I2C_Master__Assert_Timeout_w_STOP() ){
			I2C_ClearFlag( MCU_I2C__MASTER, I2C_FLAG_BUSY | I2C_FLAG_AF );		//AF = ACK Failuare
			//Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );	//BUSY, MSL, ADDR, TXE and TRA


	while( NumByteToWrite ){
		I2C_Master__Reset_Timeout_Tick();

		I2C_SendData( MCU_I2C__MASTER, *pBuffer );
		pBuffer++;
		NumByteToWrite--;

		/* Test on EV8 and clear it */
		do{
			if( I2C_Master__Assert_Timeout_w_STOP() ){
				//Console_Output_Item_and_Integer( __FILE__, __LINE__ );
				return( false );
			}
		} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
	}

	I2C_GenerateSTOP( MCU_I2C__MASTER, ENABLE );
	//Console_Output_Item_and_Integer( __FILE__, __LINE__ );
	return true;
}

bool	I2C_Master__Read( uint8_t Dev_Addr, uint8_t Reg_Addr, uint16_t NumByteToRead, uint8_t * pBuffer, uint16_t Wait_Before_Read_mS )
{
	bool	flag_fire_RX = true;
//	I2C_Master__Init( Dev_Addr );

	//========================================
	//	Write Register Address Stage
	//========================================
	//Setup timeout tick
	I2C_Master__Reset_Timeout_Tick();


	/* Send START condition */
	I2C_GenerateSTART( MCU_I2C__MASTER, ENABLE );
	do{	// Test on EV5 and clear it
		if( I2C_Master__Assert_Timeout_w_STOP() ){
//			Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_MODE_SELECT ) );		//BUSY, MSL, SB flag


	/* Send slave address for read */
	I2C_Master__Reset_Timeout_Tick();
	I2C_Send7bitAddress( MCU_I2C__MASTER, Dev_Addr, I2C_Direction_Transmitter );
	do{	//Test on EV6 and clear it
		if( I2C_Master__Assert_Timeout_w_STOP() ){
			I2C_ClearFlag( MCU_I2C__MASTER, I2C_FLAG_BUSY | I2C_FLAG_AF );		//AF = ACK Failuare
			//Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );	//BUSY, MSL, ADDR, TXE and TRA

	/* Clear EV6 by setting again the PE bit */
	I2C_Master__Reset_Timeout_Tick();
	I2C_SendData( MCU_I2C__MASTER, Reg_Addr );
	do{	/* Test on EV8 and clear it */
		if( I2C_Master__Assert_Timeout_w_STOP() ){
//			Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );	//TRA(TxRx Activity), BUSY, MSL, TXE, BTF(Byte Transfer Finished)

	if( Wait_Before_Read_mS ) Delay( Wait_Before_Read_mS );
	//=============================
	//	Read Data Stage
	//=============================

	I2C_Master__Reset_Timeout_Tick();
	/* Send START condition */
	I2C_GenerateSTART( MCU_I2C__MASTER, ENABLE );
	do{	// Test on EV5 and clear it
		if( I2C_Master__Assert_Timeout_w_STOP() ){
//			Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_MODE_SELECT ) );		//BUSY, MSL, SB flag

	/* Send slave address for read */
	I2C_Send7bitAddress( MCU_I2C__MASTER, Dev_Addr, I2C_Direction_Receiver );
	do{	//Test on EV6 and clear it
		if( I2C_Master__Assert_Timeout_w_STOP() ){
			I2C_ClearFlag( MCU_I2C__MASTER, I2C_FLAG_BUSY | I2C_FLAG_AF );		//AF = ACK Failuare
//			Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_GetFlagStatus( MCU_I2C__MASTER, I2C_FLAG_ADDR ) );	//FLAG_ADDR = end of address transmission

	I2C_AcknowledgeConfig( MCU_I2C__MASTER, ENABLE );
	while( NumByteToRead ){
		if( NumByteToRead == 1 ){
			I2C_AcknowledgeConfig( MCU_I2C__MASTER, DISABLE );
		}

		if( flag_fire_RX ){
			flag_fire_RX = false;
			I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED );	//Clear ADDR flag to start RX
		}

		I2C_Master__Reset_Timeout_Tick();
		do{
			if( I2C_Master__Assert_Timeout_w_STOP() ){
//				Console_Output_Item_and_Integer( __FILE__, __LINE__ );
				return( false );
			}
		} while( ( I2C_GetLastEvent( MCU_I2C__MASTER ) & 0x0040 ) != 0x000040 );	/* Poll on RxNE */

		*pBuffer = I2C_ReceiveData( MCU_I2C__MASTER );
		pBuffer++;
		if( NumByteToRead == 1 ) I2C_GenerateSTOP( MCU_I2C__MASTER, ENABLE );
		NumByteToRead--;
	}

//	Console_Output_Item_and_Integer( __FILE__, __LINE__ );
	return( true );
}

bool	I2C_Master__Read_Raw( uint8_t Dev_Addr, uint16_t NumByteToRead, uint8_t * pBuffer )
{
	bool	flag_fire_RX = true;

	I2C_Master__Reset_Timeout_Tick();
	/* Send START condition */
	I2C_GenerateSTART( MCU_I2C__MASTER, ENABLE );
	do{	// Test on EV5 and clear it
		if( I2C_Master__Assert_Timeout_w_STOP() ){
//			Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_MODE_SELECT ) );		//BUSY, MSL, SB flag

	/* Send slave address for read */
	I2C_Send7bitAddress( MCU_I2C__MASTER, Dev_Addr, I2C_Direction_Receiver );
	do{	//Test on EV6 and clear it
		if( I2C_Master__Assert_Timeout_w_STOP() ){
			I2C_ClearFlag( MCU_I2C__MASTER, I2C_FLAG_BUSY | I2C_FLAG_AF );		//AF = ACK Failuare
//			Console_Output_Item_and_Integer( __FILE__, __LINE__ );
			return( false );
		}
	} while( ! I2C_GetFlagStatus( MCU_I2C__MASTER, I2C_FLAG_ADDR ) );	//FLAG_ADDR = end of address transmission

	I2C_AcknowledgeConfig( MCU_I2C__MASTER, ENABLE );
	while( NumByteToRead ){
		if( NumByteToRead == 1 ){
			I2C_AcknowledgeConfig( MCU_I2C__MASTER, DISABLE );
		}

		if( flag_fire_RX ){
			flag_fire_RX = false;
			I2C_CheckEvent( MCU_I2C__MASTER, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED );	//Clear ADDR flag to start RX
		}

		I2C_Master__Reset_Timeout_Tick();
		do{
			if( I2C_Master__Assert_Timeout_w_STOP() ){
//				Console_Output_Item_and_Integer( __FILE__, __LINE__ );
				return( false );
			}
		} while( ( I2C_GetLastEvent( MCU_I2C__MASTER ) & 0x0040 ) != 0x000040 );	/* Poll on RxNE */

		*pBuffer = I2C_ReceiveData( MCU_I2C__MASTER );
		pBuffer++;
		if( NumByteToRead == 1 ) I2C_GenerateSTOP( MCU_I2C__MASTER, ENABLE );
		NumByteToRead--;
	}

//	Console_Output_Item_and_Integer( __FILE__, __LINE__ );
	return( true );
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/

