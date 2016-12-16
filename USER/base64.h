
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_BASE64_H_
#define USER_BASE64_H_

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
#define ERR_BASE64_BUFFER_TOO_SMALL	(-0x002A)
#define ERR_BASE64_INVALID_CHARACTER	(-0x002C)

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t		Base64_encode( uint8_t *, size_t, size_t *, const uint8_t *, size_t);
int32_t		Base64_decode( uint8_t *, size_t, size_t *, const uint8_t *, size_t);
int32_t		Base64_self_test( int8_t );



#endif		// USER_BASE64_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
