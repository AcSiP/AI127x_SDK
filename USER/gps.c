
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
  * @file    Project/ARM-Lora/gps.c
  * @author  JC
  * @version V1.0.0
  * @date    12-Apr-2016
  * @brief   gps program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "config.h"
#include "acsip_protocol.h"
#include "usart1.h"
#include "save_record.h"
#include "gps.h"

#ifdef Board__A22_Tracker


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// #define  MaxNorthPosition	(8388607)		// 2^23 - 1
// #define  MaxSouthPosition	(8388608)		// -2^23
// #define  MaxEastPosition	(8388607)		// 2^23 - 1
// #define  MaxWestPosition	(8388608)		// -2^23
/* Private macro -------------------------------------------------------------*/

/* Const variables -----------------------------------------------------------*/
const int32_t	MaxNorthPosition = 8388607;		// 2^23 - 1
const int32_t	MaxSouthPosition = 8388608;		// -2^23
const int32_t	MaxEastPosition = 8388607;		// 2^23 - 1
const int32_t	MaxWestPosition = 8388608;		// -2^23

/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t	GPS_USART_RX_Buf1[USART1_RX_BufLength];
extern __IO uint8_t	GPS_USART_RX_Buf2[USART1_RX_BufLength];
extern __IO bool	isGPS_USART_RX_Buf1Full;
extern __IO bool	isGPS_USART_RX_Buf2Full;
// __IO bool		GPS_Located = false;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***************************************************************************************************
 *  Function Name: GPS_PinInitialization
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	GPS_PinInitialization(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

#ifdef STM32F401xx
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
#elif STM32F071 || STM32F072
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
#endif

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_WriteBit( GPIOB, GPIO_Pin_8, Bit_SET );
}


/***************************************************************************************************
 *  Function Name: GPS_MT3333Reset
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	GPS_MT3333Reset(void)
{
	uint32_t	count = 0;

	GPIO_WriteBit( GPIOB, GPIO_Pin_8, Bit_RESET );
	do {
		asm("NOP");
		count++;
	} while ( count < 200000 );

	GPIO_WriteBit( GPIOB, GPIO_Pin_8, Bit_SET );
}


/***************************************************************************************************
 *  Function Name: GPS_MT3333Enable
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	GPS_MT3333Enable(void)
{
	uint32_t	count = 0;

	GPIO_WriteBit( GPIOB, GPIO_Pin_9, Bit_SET );

	do {
		asm("NOP");
		count++;
	} while (count < 200000);
}


/***************************************************************************************************
 *  Function Name: GPS_MT3333Disable
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	GPS_MT3333Disable(void)
{
	GPIO_WriteBit( GPIOB, GPIO_Pin_9, Bit_RESET );
}


/***************************************************************************************************
 *  Function Name: GPS_MT3333Run
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	GPS_MT3333Run(void)
{
	USART1_UartInit(9600);
	GPS_PinInitialization();
	GPS_MT3333Enable();
	GPS_MT3333Reset();
}


/***************************************************************************************************
 *  Function Name: GPS_MT3333Stop
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	GPS_MT3333Stop(void)
{
	USART1_UartEnableOrDisable(DISABLE);
	USART_DeInit(USART1);
	GPS_MT3333Disable();

	Led_GreenLedLightOff();
}


/***************************************************************************************************
 *  Function Name: GPS_ReadIn
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	GPS_ReadIn(tDeviceNodeSensor *Sensor)
{
	uint8_t		count, comma;
	uint8_t *	temp;
	int8_t *	gps_str = NULL;
	int8_t		TIME[11] = {0}, DATE[7] = {0}, LATITUDE[10] = {0}, LONGITUDE[11] = {0};
	int8_t		STATUS = 0, NorS = 0, EorW = 0;
	struct tm	tm1;
	int32_t		gps_dd, gps_mm, gps_mmmm;
	double		lat, lon;
	long double	cal;

	if((isGPS_USART_RX_Buf1Full == false) && (isGPS_USART_RX_Buf2Full == false)) return;

	// PB9 = GPS_LDO_EN, PB9 = 0, GPS_LDO off
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0) goto Return_Out;

	Led_GreenLedLightOn();

	if(Sensor == NULL) goto Return_Out;

	if(isGPS_USART_RX_Buf1Full == true) {
		temp = (uint8_t *)GPS_USART_RX_Buf1;
	} else {
		if(isGPS_USART_RX_Buf2Full == true) temp = (uint8_t *)GPS_USART_RX_Buf2;
	}

	gps_str = (int8_t *)strstr((const char*)temp, "$GNRMC");
	if(gps_str == NULL) {
		gps_str = (int8_t *)strstr((const char*)temp, "$GPRMC");
		if(gps_str == NULL) goto Return_Out;
	}

	// Console_Output_String( "GPS_ReadIn\r\n" );		// for test

	comma = count = 0;
	while(*gps_str != 0) {
		if(*gps_str != ',') {
			switch(comma) {
			case 1:
				TIME[count] = *gps_str;
				count++;
				break;

			case 2:
				STATUS = *gps_str;
				break;

			case 3:
				LATITUDE[count] = *gps_str;
				count++;
				break;

			case 4:
				NorS = *gps_str;
				count++;
				break;

			case 5:
				LONGITUDE[count] = *gps_str;
				count++;
				break;

			case 6:
				EorW = *gps_str;
				break;

			case 9:
				DATE[count] = *gps_str;
				count++;
				break;

			default:
				break;
			}
		} else {
			comma++;
			count = 0;
		}

		gps_str++;
		if(comma > 9) break;
	}

	if(STATUS == 'A') {
		sscanf((const char*)TIME, "%2d%2d%2d.", &tm1.tm_hour, &tm1.tm_min, &tm1.tm_sec);
		sscanf((const char*)DATE, "%2d%2d%2d", &tm1.tm_mday, &tm1.tm_mon, &tm1.tm_year);
		tm1.tm_mon--;
		tm1.tm_year = tm1.tm_year + 2000 - 1900;
		Sensor->UTC = mktime(&tm1);

		sscanf((const char*)LATITUDE, "%2d%2d.%4d", &gps_dd, &gps_mm, &gps_mmmm);
		lat = (double)gps_dd + (((double)gps_mm + ((double)gps_mmmm * 0.0001)) / 60.0);
		if(NorS == 'S') lat *= -1;
		if( lat >= 0 ) {		// North
			cal = lat * MaxNorthPosition;
		} else {			// South
			cal = lat * MaxSouthPosition;
		}
		Sensor->GPS_Latitude = cal / 90;

		sscanf((const char*)LONGITUDE, "%3d%2d.%4d", &gps_dd, &gps_mm, &gps_mmmm);
		lon = (double)gps_dd + (((double)gps_mm + ((double)gps_mmmm * 0.0001)) / 60.0);
		if(EorW == 'W') lon *= -1;
		if( lon >= 0 ) {		// East
			cal = lon * MaxEastPosition;
		} else {			// West
			cal = lon * MaxWestPosition;
		}
		Sensor->GPS_Longitude = cal / 180;
		SaveRecord_WriteInTraceData();
		Led_GreenLedLightOff();
	} else {
		Sensor->UTC = 0;
		Sensor->GPS_Latitude = 0;
		Sensor->GPS_Longitude = 0;
	}

	/*
	if(Sensor->UTC != 0) {
		GPS_Located = true;
	} else {
		GPS_Located = false;
	}
	*/

Return_Out:
	if(isGPS_USART_RX_Buf1Full == true) {
		isGPS_USART_RX_Buf1Full = false;
		memset((void *)GPS_USART_RX_Buf1, 0, USART1_RX_BufLength);
	} else {
		if(isGPS_USART_RX_Buf2Full == true) {
			isGPS_USART_RX_Buf2Full = false;
			memset((void *)GPS_USART_RX_Buf2, 0, USART1_RX_BufLength);
		}
	}
}

#endif

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
