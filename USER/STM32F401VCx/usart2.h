/**
  ******************************************************************************
  * @file    Project/ARM-Lora/usart2.h 
  * @author  JC
  * @version V1.0.0
  * @date    10-Mar-2016
  * @brief   Header for usart2.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART2_H
#define __USART2_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define		USART2_RX_BufLength		(256)

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USART2_PinInitialization(void);
void USART2_UartConfig(uint32_t);
void USART2_UartEnableOrDisable(FunctionalState);
void USART2_UartInit(uint32_t);
void USART2_UartWrite(uint8_t *, uint32_t);
void USART2_IRQHandler(void);
void USART2_EnvironmentVariableSet(void);
void USART2_VariableSwap(void);
void USART2_DigitalClear(void);



#endif /* __USART2_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


