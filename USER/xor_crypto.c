/**
  ******************************************************************************
  * @file    Project/ARM-Lora/xor_crypto.c
  * @author  JC
  * @version V1.0.0
  * @date    21-Mar-2016
  * @brief   XOR crypto program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "xor_crypto.h"

#ifdef STM32F072
  #include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/



/***************************************************************************************************
 *  Function Name: XOR_Encryption
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void XOR_Encryption(uint8_t key, __IO uint8_t *data, uint16_t Length) {
	
	uint16_t count;
	
	for(count = 0 ; count < Length ; count++) {
		data[count] ^= key;
	}
	
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


