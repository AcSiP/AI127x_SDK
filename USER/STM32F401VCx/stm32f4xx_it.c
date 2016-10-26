
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
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    21-October-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"
#include "board.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef STM32F401xx
#ifdef	USBD_VCP_Console
extern	USB_OTG_CORE_HANDLE	USB_OTG_dev;
extern	uint32_t		USBD_OTG_ISR_Handler( USB_OTG_CORE_HANDLE *pdev );
#endif
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1) {
		Console_Output_String( "HardFault\r\n" );
	}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1) {
		Console_Output_String( "MemManage\r\n" );
	}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1) {
		Console_Output_String( "BusFault\r\n" );
	}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1) {
		Console_Output_String( "UsageFault\r\n" );
	}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	// char		str[11];

	TickCounter++;
	// snprintf( str, sizeof(str), "%d", TickCounter );
	// Console_Output_String( "Tick=" );
	// Console_Output_String( str );
	// Console_Output_String( "\r\n" );

	// test, PB2 for SysTick in pin output
	// GPIO_WriteBit(GPIOB, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_2)));
	// test end
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/



/**
  * @}
  */ 
// void EXTI0_IRQHandler(void) {
//
//  if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
//    /* Clear the EXTI line 0 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line0);
//  }
//
// }



#ifdef STM32F401xx
#ifdef	USBD_VCP_Console
/**
  * @brief  This function handles EXTI15_10_IRQ Handler.
  * @param  None
  * @retval None
  */
void	OTG_FS_WKUP_IRQHandler( void )
{
	if( USB_OTG_dev.cfg.low_power ) {
		/* Reset SLEEPDEEP and SLEEPONEXIT bits */
		SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));

		/* After wake-up from sleep mode, reconfigure the system clock */
		SystemInit();
		USB_OTG_UngateClock( & USB_OTG_dev );
	}
	EXTI_ClearITPendingBit( EXTI_Line18 );
}



/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
void	OTG_FS_IRQHandler( void )
{
	USBD_OTG_ISR_Handler( & USB_OTG_dev );
}
#endif
#endif

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
