/**
  ******************************************************************************
  * @file    Project/ARM-Lora/base64.h 
  * @author  JC
  * @version V1.0.0
  * @date    25-Feb-2016
  * @brief   base64 program body
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __base64_H
#define __base64_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>

#ifdef STM32F072
  #include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ERR_BASE64_BUFFER_TOO_SMALL               (-0x002A)
#define ERR_BASE64_INVALID_CHARACTER              (-0x002C)

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t Base64_encode( uint8_t *, size_t, size_t *, const uint8_t *, size_t);
int32_t Base64_decode( uint8_t *, size_t, size_t *, const uint8_t *, size_t);
int32_t Base64_self_test( int8_t );



#endif /* __base64_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


