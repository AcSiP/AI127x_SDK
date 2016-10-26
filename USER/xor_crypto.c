
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
void	XOR_Encryption(uint8_t key, __IO uint8_t *data, uint16_t Length)
{
	uint16_t	count;

	for(count = 0 ; count < Length ; count++) data[count] ^= key;
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
