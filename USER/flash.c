
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

/**
  ******************************************************************************
  * @file    Project/ARM-Lora/flash.c 
  * @author  JC
  * @version V1.0.0
  * @date    15-Mar-2016
  * @brief   FLASH program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "flash.h"
#include "usart1.h"
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
/* Private functions ---------------------------------------------------------*/

#ifdef STM32F401xx
uint32_t	GetSector( uint32_t );

uint32_t	GetSector( uint32_t Address )
{
	if( (Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0) ) return FLASH_Sector_0;
	if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1)) return FLASH_Sector_1;
	if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2)) return FLASH_Sector_2;
	if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3)) return FLASH_Sector_3;
	if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4)) return FLASH_Sector_4;

	/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_5))*/
	return FLASH_Sector_5;
}
#endif


/*
#ifdef STM32F401xx
void		FLASH_EraseAllRecordSector( void )
{
	uint32_t		StartSector, EndSector, i;
	__IO FLASH_Status	FLASHStatus = FLASH_COMPLETE;

	FLASH_Unlock();
	FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR );

	StartSector = GetSector( Record_WRITE_START_ADDR );
	EndSector = GetSector( Record_WRITE_END_ADDR );

	for(i = StartSector ; (i <= EndSector) && (FLASHStatus == FLASH_COMPLETE) ; i += 8 ){
		// Device voltage range supposed to be [2.7V to 3.6V], the operation will be done by word
		FLASHStatus = FLASH_EraseSector(i, VoltageRange_3);
	}

	FLASH_Lock();
 }
#endif
*/



#ifdef STM32F401xx
void	FLASH_EraseRecordSector( uint32_t FLASH_Sector )
{
	uint32_t		StartSector, EndSector, i;
	__IO FLASH_Status	FLASHStatus = FLASH_COMPLETE;

	disableGlobalInterrupts();
	#ifdef Board__A22_Tracker
	USART1_UartEnableOrDisable(DISABLE);
	#endif

	FLASH_Unlock();
	FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR );

	StartSector = GetSector( FLASH_Sector );
	EndSector = GetSector( FLASH_Sector );

	for( i = StartSector ; (i <= EndSector) && (FLASHStatus == FLASH_COMPLETE) ; i += 8 ) {
		// Device voltage range supposed to be [2.7V to 3.6V], the operation will be done by word
		FLASHStatus = FLASH_EraseSector(i, VoltageRange_3);
	}

	FLASH_Lock();
	#ifdef Board__A22_Tracker
	USART1_UartEnableOrDisable(ENABLE);
	#endif
	enableGlobalInterrupts();
}
#endif



/*
#ifdef STM32F072
void		FLASH_EraseAllRecordPage( void )
{
	uint32_t		NbrOfPage, EraseCounter;
	__IO FLASH_Status	FLASHStatus = FLASH_COMPLETE;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

	NbrOfPage = (Record_WRITE_END_ADDR - Record_WRITE_START_ADDR + 1) / FLASH_PAGE_SIZE;
	for(EraseCounter = 0 ; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE) ; EraseCounter++) {  
		FLASHStatus = FLASH_ErasePage(Record_WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
	}

	FLASH_Lock();
}
#endif
*/


#ifdef STM32F072
void	FLASH_EraseRecordPage( uint32_t PageStartAddr )
{
	__IO FLASH_Status	FLASHStatus;

	disableGlobalInterrupts();
	#ifdef Board__A22_Tracker
	USART1_UartEnableOrDisable(DISABLE);
	#endif

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

	do {
		FLASHStatus = FLASH_ErasePage(PageStartAddr);
	} while ( FLASHStatus != FLASH_COMPLETE );

	FLASH_Lock();
	#ifdef Board__A22_Tracker
	USART1_UartEnableOrDisable(ENABLE);
	#endif
	enableGlobalInterrupts();
}
#endif


void		FLASH_WriteByte( uint32_t StartAddr, uint8_t *Data, uint16_t Num )
{
	uint16_t	count;

	FLASH_Unlock();

#ifdef STM32F072
	uint16_t	value;

	Num /= 2;
	FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
	for( count = 0 ; count < Num ; count++ ) {
		value = *(Data++);
		value |= *(Data++) << 8;
		FLASH_ProgramHalfWord(StartAddr, value);
		StartAddr += 2;
	}

#elif STM32F401xx

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR );
	for( count = 0 ; count < Num ; count++ ) {
		FLASH_ProgramByte(StartAddr, Data[count]);
		StartAddr++;
	}

#endif

	FLASH_Lock();
}


void		FLASH_WriteHalfWord( uint32_t StartAddr, const uint16_t * Data, uint16_t Num )
{
	uint16_t	count;

	FLASH_Unlock();

#ifdef STM32F072
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
#elif STM32F401xx
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR );
#endif

	for( count = 0 ; count < Num ; count++ ) {
		FLASH_ProgramHalfWord(StartAddr, Data[count]);
		StartAddr += 2;
	}

	FLASH_Lock();
}


void		FLASH_WriteWord( uint32_t StartAddr, uint32_t *Data, uint16_t Num )
{
	uint16_t	count;

	FLASH_Unlock();

#ifdef STM32F072
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
#elif STM32F401xx
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR );
#endif

	for( count = 0 ; count < Num ; count++ ) {
		FLASH_ProgramWord( StartAddr, Data[count] );
		StartAddr += 4;
	}

	FLASH_Lock();
}


void		FLASH_WriteDoubleWord( uint32_t StartAddr, uint64_t *Data, uint16_t Num)
{
	uint16_t	count;
	uint32_t	word;

	FLASH_Unlock();

#ifdef STM32F072
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
#elif STM32F401xx
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
#endif

	for( count = 0 ; count < Num ; count++ ) {
		word = (uint32_t)((*Data) & 0x00000000FFFFFFFF);
		FLASH_ProgramWord(StartAddr, word);
		StartAddr += 4;
		word = (uint32_t)(((*Data) >> 32) & 0x00000000FFFFFFFF);
		FLASH_ProgramWord(StartAddr, word);
		StartAddr += 4;
		Data++;
	}

	FLASH_Lock();
}


void		FLASH_ReadByte( uint32_t StartAddr, uint8_t *Data, uint16_t Num )
{
	while( Num-- ) {
		*( Data++ ) = *( (__IO uint8_t *) StartAddr++ );
	}
}


void		FLASH_ReadHalfWord( uint32_t StartAddr, uint16_t *Data, uint16_t Num )
{
	while( Num-- ) {
		*(Data++) = *( (__IO uint16_t *)StartAddr );
		StartAddr += 2;
	}
}


void		FLASH_ReadWord( uint32_t StartAddr, uint32_t *Data, uint16_t Num )
{
	while( Num-- ) {
		*(Data++) = *( (__IO uint32_t *)StartAddr );
		StartAddr += 4;
	}
}


void		FLASH_ReadDoubleWord( uint32_t StartAddr, uint64_t *Data, uint16_t Num )
{
	while( Num-- ) {
		*(Data++) = *( (__IO uint64_t *) StartAddr );
		StartAddr += 8;
	}
}


void		FLASH_WriteString( uint32_t StartAddr, uint8_t *Data, uint16_t Num )
{
	FLASH_WriteByte( StartAddr, Data, Num );
}


void		FLASH_ReadString( uint32_t StartAddr, uint8_t *Data, uint16_t Num )
{
	FLASH_ReadByte(StartAddr, Data, Num);
}

#ifdef STM32F401xx
int16_t		Flash_Read__CW_Offset( void )
{
	int		i;
	int16_t		last_val = 0;
	uint16_t	val;

	for( i = 0; i < 16; i += 2 ) {
		FLASH_ReadHalfWord( Record__CW_Offset__Addr + i, & val, 1 );
		if( val == 0xffff ) break;

		val += 1;
		last_val = (int) val;
	}
	return( last_val );
}

bool		Flash_Write__CW_Offset( int16_t offset_hz )
{
	int		i;
	uint16_t	val;

	if( ! offset_hz ) return( true );

	offset_hz -= 1;
	for( i = 0; i < 16; i += 2 ) {
		FLASH_ReadHalfWord( Record__CW_Offset__Addr + i, & val, 1 );
		if( val != 0xffff ) continue;

		val = (uint16_t) offset_hz;
		FLASH_WriteHalfWord( Record__CW_Offset__Addr + i, & val , 1 );
		return( true );
	}
	return( false );
}
#endif

#ifdef STM32F072
int16_t		Flash_Read__CW_Offset( void )
{
//	FLASH_WriteByte( 0x1FFF7800 + addr, & val, 1 );
	return( 0 );
}

bool		Flash_Write__CW_Offset( int16_t offset_hz )
{
	return( false );
}
#endif


/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
