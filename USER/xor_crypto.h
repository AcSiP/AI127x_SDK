/**
  ******************************************************************************
  * @file    Project/ARM-Lora/xor_crypto.h 
  * @author  JC
  * @version V1.0.0
  * @date    21-Mar-2016
  * @brief   XOR crypto program body
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XOR_Crypto_H
#define __XOR_Crypto_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
  #include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void XOR_Encryption(uint8_t, __IO uint8_t *, uint16_t);



#endif /* __XOR_Crypto_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


