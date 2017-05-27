
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
#ifndef USER_FLASH_H_
#define USER_FLASH_H_

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef STM32F072
	#define FLASH_PAGE_SIZE			((uint32_t)0x00000800)		/* 2K bytes */
	#define FLASH_SECTOR_SIZE		((uint32_t)0x00001000)		/* 4K bytes */
	#define ADDR_FLASH_PAGE_0		((uint32_t)0x08000000)		/* Base @ of Page 0, 2 Kbytes */
	#define ADDR_FLASH_PAGE_1		((uint32_t)0x08000800)		/* Base @ of Page 1, 2 Kbytes */
	#define ADDR_FLASH_PAGE_2		((uint32_t)0x08001000)		/* Base @ of Page 2, 2 Kbytes */
	#define ADDR_FLASH_PAGE_3		((uint32_t)0x08001800)		/* Base @ of Page 3, 2 Kbytes */
	#define ADDR_FLASH_PAGE_4		((uint32_t)0x08002000)		/* Base @ of Page 4, 2 Kbytes */
	#define ADDR_FLASH_PAGE_5		((uint32_t)0x08002800)		/* Base @ of Page 5, 2 Kbytes */
	#define ADDR_FLASH_PAGE_6		((uint32_t)0x08003000)		/* Base @ of Page 6, 2 Kbytes */
	#define ADDR_FLASH_PAGE_7		((uint32_t)0x08003800)		/* Base @ of Page 7, 2 Kbytes */
	#define ADDR_FLASH_PAGE_8		((uint32_t)0x08004000)		/* Base @ of Page 8, 2 Kbytes */
	#define ADDR_FLASH_PAGE_9		((uint32_t)0x08004800)		/* Base @ of Page 9, 2 Kbytes */
	#define ADDR_FLASH_PAGE_10		((uint32_t)0x08005000)		/* Base @ of Page 10, 2 Kbytes */
	#define ADDR_FLASH_PAGE_11		((uint32_t)0x08005800)		/* Base @ of Page 11, 2 Kbytes */
	#define ADDR_FLASH_PAGE_12		((uint32_t)0x08006000)		/* Base @ of Page 12, 2 Kbytes */
	#define ADDR_FLASH_PAGE_13		((uint32_t)0x08006800)		/* Base @ of Page 13, 2 Kbytes */
	#define ADDR_FLASH_PAGE_14		((uint32_t)0x08007000)		/* Base @ of Page 14, 2 Kbytes */
	#define ADDR_FLASH_PAGE_15		((uint32_t)0x08007800)		/* Base @ of Page 15, 2 Kbytes */
	#define ADDR_FLASH_PAGE_16		((uint32_t)0x08008000)		/* Base @ of Page 16, 2 Kbytes */
	#define ADDR_FLASH_PAGE_17		((uint32_t)0x08008800)		/* Base @ of Page 17, 2 Kbytes */
	#define ADDR_FLASH_PAGE_18		((uint32_t)0x08009000)		/* Base @ of Page 18, 2 Kbytes */
	#define ADDR_FLASH_PAGE_19		((uint32_t)0x08009800)		/* Base @ of Page 19, 2 Kbytes */
	#define ADDR_FLASH_PAGE_20		((uint32_t)0x0800A000)		/* Base @ of Page 20, 2 Kbytes */
	#define ADDR_FLASH_PAGE_21		((uint32_t)0x0800A800)		/* Base @ of Page 21, 2 Kbytes */
	#define ADDR_FLASH_PAGE_22		((uint32_t)0x0800B000)		/* Base @ of Page 22, 2 Kbytes */
	#define ADDR_FLASH_PAGE_23		((uint32_t)0x0800B800)		/* Base @ of Page 23, 2 Kbytes */
	#define ADDR_FLASH_PAGE_24		((uint32_t)0x0800C000)		/* Base @ of Page 24, 2 Kbytes */
	#define ADDR_FLASH_PAGE_25		((uint32_t)0x0800C800)		/* Base @ of Page 25, 2 Kbytes */
	#define ADDR_FLASH_PAGE_26		((uint32_t)0x0800D000)		/* Base @ of Page 26, 2 Kbytes */
	#define ADDR_FLASH_PAGE_27		((uint32_t)0x0800D800)		/* Base @ of Page 27, 2 Kbytes */
	#define ADDR_FLASH_PAGE_28		((uint32_t)0x0800E000)		/* Base @ of Page 28, 2 Kbytes */
	#define ADDR_FLASH_PAGE_29		((uint32_t)0x0800E800)		/* Base @ of Page 29, 2 Kbytes */
	#define ADDR_FLASH_PAGE_30		((uint32_t)0x0800F000)		/* Base @ of Page 30, 2 Kbytes */
	#define ADDR_FLASH_PAGE_31		((uint32_t)0x0800F800)		/* Base @ of Page 31, 2 Kbytes */
	#define ADDR_FLASH_PAGE_32		((uint32_t)0x08010000)		/* Base @ of Page 32, 2 Kbytes */
	#define ADDR_FLASH_PAGE_33		((uint32_t)0x08010800)		/* Base @ of Page 33, 2 Kbytes */
	#define ADDR_FLASH_PAGE_34		((uint32_t)0x08011000)		/* Base @ of Page 34, 2 Kbytes */
	#define ADDR_FLASH_PAGE_35		((uint32_t)0x08011800)		/* Base @ of Page 35, 2 Kbytes */
	#define ADDR_FLASH_PAGE_36		((uint32_t)0x08012000)		/* Base @ of Page 36, 2 Kbytes */
	#define ADDR_FLASH_PAGE_37		((uint32_t)0x08012800)		/* Base @ of Page 37, 2 Kbytes */
	#define ADDR_FLASH_PAGE_38		((uint32_t)0x08013000)		/* Base @ of Page 38, 2 Kbytes */
	#define ADDR_FLASH_PAGE_39		((uint32_t)0x08013800)		/* Base @ of Page 39, 2 Kbytes */
	#define ADDR_FLASH_PAGE_40		((uint32_t)0x08014000)		/* Base @ of Page 40, 2 Kbytes */
	#define ADDR_FLASH_PAGE_41		((uint32_t)0x08014800)		/* Base @ of Page 41, 2 Kbytes */
	#define ADDR_FLASH_PAGE_42		((uint32_t)0x08015000)		/* Base @ of Page 42, 2 Kbytes */
	#define ADDR_FLASH_PAGE_43		((uint32_t)0x08015800)		/* Base @ of Page 43, 2 Kbytes */
	#define ADDR_FLASH_PAGE_44		((uint32_t)0x08016000)		/* Base @ of Page 44, 2 Kbytes */
	#define ADDR_FLASH_PAGE_45		((uint32_t)0x08016800)		/* Base @ of Page 45, 2 Kbytes */
	#define ADDR_FLASH_PAGE_46		((uint32_t)0x08017000)		/* Base @ of Page 46, 2 Kbytes */
	#define ADDR_FLASH_PAGE_47		((uint32_t)0x08017800)		/* Base @ of Page 47, 2 Kbytes */
	#define ADDR_FLASH_PAGE_48		((uint32_t)0x08018000)		/* Base @ of Page 48, 2 Kbytes */
	#define ADDR_FLASH_PAGE_49		((uint32_t)0x08018800)		/* Base @ of Page 49, 2 Kbytes */
	#define ADDR_FLASH_PAGE_50		((uint32_t)0x08019000)		/* Base @ of Page 50, 2 Kbytes */
	#define ADDR_FLASH_PAGE_51		((uint32_t)0x08019800)		/* Base @ of Page 51, 2 Kbytes */
	#define ADDR_FLASH_PAGE_52		((uint32_t)0x0801A000)		/* Base @ of Page 52, 2 Kbytes */
	#define ADDR_FLASH_PAGE_53		((uint32_t)0x0801A800)		/* Base @ of Page 53, 2 Kbytes */
	#define ADDR_FLASH_PAGE_54		((uint32_t)0x0801B000)		/* Base @ of Page 54, 2 Kbytes */
	#define ADDR_FLASH_PAGE_55		((uint32_t)0x0801B800)		/* Base @ of Page 55, 2 Kbytes */
	#define ADDR_FLASH_PAGE_56		((uint32_t)0x0801C000)		/* Base @ of Page 56, 2 Kbytes */
	#define ADDR_FLASH_PAGE_57		((uint32_t)0x0801C800)		/* Base @ of Page 57, 2 Kbytes */
	#define ADDR_FLASH_PAGE_58		((uint32_t)0x0801D000)		/* Base @ of Page 58, 2 Kbytes */
	#define ADDR_FLASH_PAGE_59		((uint32_t)0x0801D800)		/* Base @ of Page 59, 2 Kbytes */
	#define ADDR_FLASH_PAGE_60		((uint32_t)0x0801E000)		/* Base @ of Page 60, 2 Kbytes */
	#define ADDR_FLASH_PAGE_61		((uint32_t)0x0801E800)		/* Base @ of Page 61, 2 Kbytes */
	#define ADDR_FLASH_PAGE_62		((uint32_t)0x0801F000)		/* Base @ of Page 62, 2 Kbytes */
	#define ADDR_FLASH_PAGE_63		((uint32_t)0x0801F800)		/* Base @ of Page 63, 2 Kbytes */
	#define ADDR_FLASH_SECTOR_0		((uint32_t)0x08000000)		/* Base @ of Sector 0, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_1		((uint32_t)0x08001000)		/* Base @ of Sector 1, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_2		((uint32_t)0x08002000)		/* Base @ of Sector 2, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_3		((uint32_t)0x08003000)		/* Base @ of Sector 3, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_4		((uint32_t)0x08004000)		/* Base @ of Sector 4, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_5		((uint32_t)0x08005000)		/* Base @ of Sector 5, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_6		((uint32_t)0x08006000)		/* Base @ of Sector 6, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_7		((uint32_t)0x08007000)		/* Base @ of Sector 7, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_8		((uint32_t)0x08008000)		/* Base @ of Sector 8, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_9		((uint32_t)0x08009000)		/* Base @ of Sector 9, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_10		((uint32_t)0x0800A000)		/* Base @ of Sector 10, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_11		((uint32_t)0x0800B000)		/* Base @ of Sector 11, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_12		((uint32_t)0x0800C000)		/* Base @ of Sector 12, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_13		((uint32_t)0x0800D000)		/* Base @ of Sector 13, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_14		((uint32_t)0x0800E000)		/* Base @ of Sector 14, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_15		((uint32_t)0x0800F000)		/* Base @ of Sector 15, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_16		((uint32_t)0x08010000)		/* Base @ of Sector 16, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_17		((uint32_t)0x08011000)		/* Base @ of Sector 17, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_18		((uint32_t)0x08012000)		/* Base @ of Sector 18, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_19		((uint32_t)0x08013000)		/* Base @ of Sector 19, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_20		((uint32_t)0x08014000)		/* Base @ of Sector 20, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_21		((uint32_t)0x08015000)		/* Base @ of Sector 21, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_22		((uint32_t)0x08016000)		/* Base @ of Sector 22, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_23		((uint32_t)0x08017000)		/* Base @ of Sector 23, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_24		((uint32_t)0x08018000)		/* Base @ of Sector 24, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_25		((uint32_t)0x08019000)		/* Base @ of Sector 25, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_26		((uint32_t)0x0801A000)		/* Base @ of Sector 26, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_27		((uint32_t)0x0801B000)		/* Base @ of Sector 27, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_28		((uint32_t)0x0801C000)		/* Base @ of Sector 28, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_29		((uint32_t)0x0801D000)		/* Base @ of Sector 29, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_30		((uint32_t)0x0801E000)		/* Base @ of Sector 30, 4 Kbytes */
	#define ADDR_FLASH_SECTOR_31		((uint32_t)0x0801F000)		/* Base @ of Sector 31, 4 Kbytes */
	#define Record_WRITE_START_ADDR		(ADDR_FLASH_PAGE_62)
	#define Record_WRITE_END_ADDR		((uint32_t)0x0801FFFF)
#endif

#ifdef STM32F401xx
	#define ADDR_FLASH_SECTOR_0		((uint32_t)0x08000000)		/* Base @ of Sector 0, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_1		((uint32_t)0x08004000)		/* Base @ of Sector 1, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_2		((uint32_t)0x08008000)		/* Base @ of Sector 2, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_3		((uint32_t)0x0800C000)		/* Base @ of Sector 3, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_4		((uint32_t)0x08010000)		/* Base @ of Sector 4, 64 Kbytes */
	#define ADDR_FLASH_SECTOR_5		((uint32_t)0x08020000)		/* Base @ of Sector 5, 128 Kbytes */
	#define Record_WRITE_START_ADDR		((uint32_t)ADDR_FLASH_SECTOR_4)
	#define Record_WRITE_END_ADDR		((uint32_t)0x0801FFFF)

	#define Record__CW_Offset__Addr		((uint32_t)0x1FFF7800)		// OTP Block 0
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef STM32F072
	// void	FLASH_EraseAllRecordPage(void);
	void	FLASH_EraseRecordPage( uint32_t );
#endif

#ifdef STM32F401xx
	// void	FLASH_EraseAllRecordSector(void);
	void	FLASH_EraseRecordSector( uint32_t );
#endif

void		FLASH_WriteByte( uint32_t, uint8_t *, uint16_t );
void		FLASH_WriteHalfWord( uint32_t StartAddr, const uint16_t * Data, uint16_t Num );
void		FLASH_WriteWord( uint32_t, uint32_t *, uint16_t );
void		FLASH_WriteDoubleWord( uint32_t, uint64_t *, uint16_t );
void		FLASH_ReadByte( uint32_t, uint8_t *, uint16_t );
void		FLASH_ReadHalfWord( uint32_t, uint16_t *, uint16_t );
void		FLASH_ReadWord( uint32_t, uint32_t *, uint16_t );
void		FLASH_ReadDoubleWord( uint32_t, uint64_t *, uint16_t );
void		FLASH_WriteString( uint32_t, uint8_t *, uint16_t );
void		FLASH_ReadString( uint32_t, uint8_t *, uint16_t );

int16_t		Flash_Read__CW_Offset( void );
bool		Flash_Write__CW_Offset( int16_t offset_hz );

#endif		// USER_FLASH_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
