/**
  ******************************************************************************
  * @file    Project/ARM-Lora/console_function.h
  * @author  JC
  * @version V1.0.0
  * @date    24-May-2016
  * @brief   Header for console_function.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Console_Function_H
#define __Console_Function_H

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
void ConsoleFunction_CmdOutput(uint8_t *, uint32_t);



#endif /* __Console_Function_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


