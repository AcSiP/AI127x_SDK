
//---------------------------------------------------------------------------
/*
//==========================================
// Author : Fox<shenfa@acsip.com.tw>
// Copyright 2017(C) AcSiP Technology Inc.
// 版權所有：群登科技股份有限公司
// http://www.acsip.com.tw
//==========================================
*/
//---------------------------------------------------------------------------



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_STM32F072RBX_BASE_DRIVER__ADC1_H_
#define USER_STM32F072RBX_BASE_DRIVER__ADC1_H_

#ifdef USE_HARDWARE__ADC1

/* Includes ------------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


/* Exported define -----------------------------------------------------------*/
#define		ADC_IDX___ADC0		(0x0001)		// PA0
#define		ADC_IDX___ADC1		(0x0002)		// PA1
#define		ADC_IDX___ADC4		(0x0004)		// PA4
#define		ADC_IDX___ADC5		(0x0008)		// PA5
#define		ADC_IDX___ADC6		(0x0010)		// PA6
#define		ADC_IDX___ADC7		(0x0020)		// PA7
#define		ADC_IDX___ADC8		(0x0040)		// PB0
#define		ADC_IDX___Vref		(0x0080)		// Vref
#define		ADC_IDX___VBat		(0x0100)		// VBattery
#define		ADC_IDX___VTemp		(0x0200)		// Temperature Sensor

// While VBat && VTemp configured together,  VBat > VTemp



/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern	uint16_t		ADC1_Converted_Value[16];


/* Exported functions ------------------------------------------------------- */
uint8_t		ADC1__Count_Items( uint16_t ch_sel );
int8_t		ADC1__Get_Item_Index( uint16_t ch_sel );
void		ADC1__Dump_Values( void );

void		ADC1__Configure_w_DMA( uint16_t ch_sel );
uint16_t	ADC1__Get_Converted_Value( uint16_t ch_sel );

#endif		// USE_HARDWARE__ADC1

#endif		// USER_STM32F072RBX_BASE_DRIVER__ADC1_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
