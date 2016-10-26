
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
#ifndef USER_UUID_H_
#define USER_UUID_H_

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Private define ------------------------------------------------------------*/
#ifdef STM32F072
	#define UUID__FLASH_ADDR		( (uint32_t) 0x1FFFF7AC )
	#define MCU_DevID__FLASH_ADDR		( (uint32_t) 0x40015800 )
	#define MCU_RDP__FLASH_ADDR		( (uint32_t) 0x1FFFF800 )
	#define MCU_WRP__FLASH_ADDR		( (uint32_t) 0x1FFFF808 )
#endif
#ifdef STM32F401xx
	#define UUID__FLASH_ADDR		( (uint32_t) 0x1FFF7A10 )
	#define MCU_DevID__FLASH_ADDR		( (uint32_t) 0xE0042000 )
	#define MCU_RDP__FLASH_ADDR		( (uint32_t) 0x1FFFC000 )
	#define MCU_WRP__FLASH_ADDR		( (uint32_t) 0x1FFFC008 )
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	UUID_Read(uint32_t *);
void	UUID_ReadLeast3Bytes(uint8_t *);

uint32_t	MCU__Read_Device_ID( void );
uint32_t	MCU__Read_Device_RDP( void );
uint32_t	MCU__Read_Device_WRP( void );

#endif		// USER_UUID_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
