/**
  ******************************************************************************
  * @file    Project/ARM-Lora/button.h  
  * @author  JC
  * @version V1.0.0
  * @date    18-May-2016
  * @brief   Header for button.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Button_H
#define __Button_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
  #include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Button_ButtonInitialization(void);
void Button_Delay(void);
void Button_PowerButtonAction(void);
#ifdef STM32F401xx
  void EXTI0_IRQHandler(void);
  void EXTI1_IRQHandler(void);
  void EXTI4_IRQHandler(void);
#elif STM32F072
  void EXTI0_1_IRQHandler(void);
  void EXTI4_15_IRQHandler(void);
#endif



#endif /* __Button_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


