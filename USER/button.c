/**
  ******************************************************************************
  * @file    Project/ARM-Lora/button.c
  * @author  JC
  * @version V1.0.0
  * @date    18-May-2016
  * @brief   button program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "sx1276-Hal.h"
#include "button.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Const variables -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern bool		EnableMaster;		// Master/Slave selection
extern __IO uint32_t	SystemOperMode;
__IO bool PowerButton = false;
__IO bool FactoryResetButton = false;
__IO bool GPS_HandmadeOff = false;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/***************************************************************************************************
 *  Function Name: Button_ButtonInitialization
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void Button_ButtonInitialization(void) {
  
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  
#ifdef STM32F401xx
  /* Enable GPIOA & GPIOB clock */
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE );
#elif STM32F072
  /* Enable GPIOA & GPIOB clock */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE );
#endif
  
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Connect EXTI Line0 to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
  
  /* Configure PA1 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Connect EXTI Line1 to PA1 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
  
  /* Configure EXTI Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Configure EXTI Line1 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
#ifdef STM32F401xx
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
#elif STM32F072
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
#endif
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable and set EXTI Line1 Interrupt to the lowest priority */
#ifdef STM32F401xx
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_Init(&NVIC_InitStructure);
#endif
  
  
  /* Clear the EXTI line 0 pending bit */
  EXTI_ClearITPendingBit(EXTI_Line0);
  /* Clear the EXTI line 1 pending bit */
  EXTI_ClearITPendingBit(EXTI_Line1);
  
  /* Configure PB4 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Connect EXTI Line4 to PB4 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
  
  /* Configure EXTI Line4 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set EXTI Line4 Interrupt to the lowest priority */
#ifdef STM32F401xx
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
#elif STM32F072
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
#endif
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Clear the EXTI line 4 pending bit */
  EXTI_ClearITPendingBit(EXTI_Line4);
  
}



/***************************************************************************************************
 *  Function Name: Button_Delay
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void Button_Delay(void) {
  
  uint32_t delay = 0;
  
  do{
    asm("NOP");
    delay++;
  }while(delay < 6000000);
  
}



/***************************************************************************************************
 *  Function Name: Button_PowerButtonAction
 *
 *  Description: Power Key
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void Button_PowerButtonAction(void) {
  
  if(( SystemOperMode == SystemInNormal ) && (EnableMaster == true)) {
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1) {   // PA4 = BLE_LDO_EN,
      BlueTooth_DA14580Stop();                            // if BLE_LDO_EN = 1, GPS running, so stop it.
    } else {
      BlueTooth_DA14580Run(ComPortBaudRate);              // if BLE_LDO_EN = 0, GPS stopping, so run it.
    }
  } else if(( SystemOperMode == SystemInNormal ) && (EnableMaster == false)) {
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 1) {   // PB9 = GPS_LDO_EN,
      GPS_MT3333Stop();                                   // if GPS_LDO_EN = 1, GPS running, so stop it.
      GPS_HandmadeOff = true;
    } else {
      GPS_MT3333Run();                                    // if GPS_LDO_EN = 0, GPS stopping, so run it.
      GPS_HandmadeOff = false;
    }
  }
  
}



/***************************************************************************************************
 *  Function Name: EXTI0_IRQHandler(F401)
 *
 *  Description: Power Key
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
#ifdef STM32F401xx
void EXTI0_IRQHandler(void) {
  
  Button_Delay();
  
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
    
    PowerButton = true;
  
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
  
}
#endif



/***************************************************************************************************
 *  Function Name: EXTI1_IRQHandler(F401)  or  EXTI0_1_IRQHandler(F072/71)
 *
 *  Description: Power Key
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
#ifdef STM32F401xx
void EXTI1_IRQHandler(void) {
#elif STM32F072
void EXTI0_1_IRQHandler(void) {
#endif
  
  Button_Delay();
  
#ifdef STM32F072
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
    
    PowerButton = true;
    
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
    
  }
#endif
  
  if(EXTI_GetITStatus(EXTI_Line1) != RESET) {
    
    PowerButton = true;
    
    /* Clear the EXTI line 1 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line1);
    
  }
  
}



/***************************************************************************************************
 *  Function Name: EXTI4_IRQHandler(F401)  or  EXTI4_15_IRQHandler(F072/71)
 *
 *  Description: Factory Reset Key , If keep press button > 5s, then Factory Reset, or nothing to do.
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
#ifdef STM32F401xx
void EXTI4_IRQHandler(void) {
#elif STM32F072
void EXTI4_15_IRQHandler(void) {
#endif
  
  Button_Delay();
  
  if(EXTI_GetITStatus(EXTI_Line4) != RESET) {
    
    if( SystemOperMode == SystemInNormal ) {
      FactoryResetButton = true;
    } else {
      FactoryResetButton = false;
    }
    
    /* Clear the EXTI line 4 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line4);
    
  }
  
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


