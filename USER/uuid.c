
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
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

#include "flash.h"
#include "uuid.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/***************************************************************************************************
 *  Function Name: UUID_Read
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	UUID_Read(uint32_t *UUID)
{
	FLASH_ReadWord(UUID__FLASH_ADDR, UUID, 3);
}

/***************************************************************************************************
 *  Function Name: UUID_ReadLeast3Bytes
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	UUID_ReadLeast3Bytes(uint8_t *Data)
{
	FLASH_ReadByte(UUID__FLASH_ADDR, Data, 3);
}

uint32_t	MCU__Read_Device_ID( void )
{
	uint32_t	id = 0;

	FLASH_ReadWord( MCU_DevID__FLASH_ADDR, & id, 1 );
	return( id );
}


uint16_t	MCU__Merge_Option( uint32_t n )
{
	uint32_t	t;

	t = n;
	t >>= 8;
	t &= 0xFF00;
	n &= 0x00FF;
	t |= n;
	return( t );
}

uint32_t	MCU__Read_Device_RDP( void )
{
	uint32_t	rdp = 0;
	uint32_t	wrp = 0;

	FLASH_ReadWord( MCU_RDP__FLASH_ADDR, & rdp, 1 );
#ifdef STM32F072
	wrp = rdp;
	rdp = MCU__Merge_Option( wrp );
#endif


#ifdef STM32F401xx
	FLASH_ReadWord( MCU_WRP__FLASH_ADDR, & wrp, 1 );
	if( wrp & 0x8000 ) {
		rdp |= 0x0001;
	} else {
		rdp &= 0xFFFE;
	}
#endif

	return( rdp );
}

uint32_t	MCU__Read_Device_WRP( void )
{
	uint32_t	wrp[2] = { 0 };

#ifdef STM32F401xx
	FLASH_ReadWord( MCU_WRP__FLASH_ADDR, wrp, 1 );
// 	wrp[0] &= 0x7FFF;
	return( wrp[0] );
#endif

#ifdef STM32F072
	uint32_t	r, t;

	FLASH_ReadWord( MCU_WRP__FLASH_ADDR, wrp, 2 );
	t = MCU__Merge_Option( wrp[0] );
	r = MCU__Merge_Option( wrp[1] );
	r <<= 16;
	r |= t;
	return( r );
#endif
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
