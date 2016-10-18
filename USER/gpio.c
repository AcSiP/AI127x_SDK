/**
  ******************************************************************************
  * @file    Project/ARM-Lora/gpio.c 
  * @author  JC
  * @version V1.0.0
  * @date    20-Jan-2016
  * @brief   GPIO program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>

#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

#include "gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/***************************************************************************************************
 *  Function Name: GPIO_PinInitialization
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	GPIO_PinInitialization( void )
{
	GPIO_InitTypeDef	GPIO_InitStructure;
//	EXTI_InitTypeDef	EXTI_InitStructure;
//	NVIC_InitTypeDef	NVIC_InitStructure;

#ifdef STM32F072
	/* Pin configuration */
	//PB2 for SysTick in pin output
	//PB3 for TIM4 rate in pin output
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	/* Alternate function conf */
	// PA8 output SYSCLK
	GPIO_PinAFConfig( GPIOA, GPIO_PinSource8, GPIO_AF_0 );
	RCC_MCOConfig( RCC_MCOSource_SYSCLK, RCC_MCOPrescaler_1 );
#endif

#ifdef STM32F401xx
	/* Pin configuration */
	//PB2 for SysTick in pin output
	//PB3 for TIM4 rate in pin output
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init( GPIOA, &GPIO_InitStructure );
	/* Alternate function conf */
	// PA8 output PLLCLK(in Lora system set the SYSCLK=PLLCLK)
	GPIO_PinAFConfig( GPIOA, GPIO_PinSource8, GPIO_AF_MCO );
	RCC_MCO1Config( RCC_MCO1Source_PLLCLK, RCC_MCO1Div_1 );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init( GPIOC, &GPIO_InitStructure );
	/* Alternate function conf */
	// PC9 output SYSCLK
	GPIO_PinAFConfig( GPIOC, GPIO_PinSource9, GPIO_AF_MCO );
	RCC_MCO2Config( RCC_MCO2Source_SYSCLK, RCC_MCO1Div_1 );
#endif

//	RCC_MCOConfig(RCC_MCOSource_HSE, RCC_MCOPrescaler_1);
//	RCC_MCOConfig(RCC_MCOSource_PLLCLK, RCC_MCOPrescaler_1);

	/* Configure PB4 pin as input floating */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Enable SYSCFG clock */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Connect EXTI4 Line to PB4 pin */
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);

	/* Configure EXTI04 line */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI0 Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}



/***************************************************************************************************
 *  Function Name: GPIO_UM402toNormalMode
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void		GPIO_UM402toNormalMode( void )
{
	GPIO_ResetBits( GPIOB, GPIO_Pin_2 );
	GPIO_ResetBits( GPIOB, GPIO_Pin_3 );
}



/***************************************************************************************************
 *  Function Name: GPIO_UM402toWakeupMode
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	GPIO_UM402toWakeupMode( void )
{
	GPIO_ResetBits( GPIOB, GPIO_Pin_2 );
	GPIO_SetBits( GPIOB, GPIO_Pin_3 );
}



/***************************************************************************************************
 *  Function Name: GPIO_UM402toLowPowerMode
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	GPIO_UM402toLowPowerMode( void )
{
	GPIO_SetBits( GPIOB, GPIO_Pin_2 );
	GPIO_ResetBits( GPIOB, GPIO_Pin_3 );
}



/***************************************************************************************************
 *  Function Name: GPIO_UM402toConfigMode
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	GPIO_UM402toConfigMode( void )
{
	GPIO_SetBits( GPIOB, GPIO_Pin_2 );
	GPIO_SetBits( GPIOB, GPIO_Pin_3 );
}



/***************************************************************************************************
 *  Function Name: EXTI4_15_IRQHandler
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	EXTI4_15_IRQHandler( void )
{
	if( EXTI_GetITStatus(EXTI_Line4) != RESET ){
		EXTI_ClearITPendingBit( EXTI_Line4 );
	}
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


