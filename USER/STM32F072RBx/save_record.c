
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
  * @file    Project/ARM-Lora/save_record.c 
  * @author  JC
  * @version V1.0.0
  * @date    29-Apr-2016
  * @brief   Save program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "config.h"
#include "flash.h"
#include "main.h"
#include "save_record.h"

/*
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t			SystemOperMode;
extern bool				EnableMaster;						// 1=Master or 0=Slave selection
extern tLoRaSettings			LoRaSettings;
extern tLoraDeviceNode *		LoraNodeDevice[MAX_LoraNodeNum];			// for MASTER
extern tDeviceNodeSleepAndRandomHop *	DeviceNodeSleepAndRandomHop[MAX_LoraNodeNum];		// for MASTER
extern tDeviceNodeSensor *		DeviceNodeSensor[MAX_LoraNodeNum];			// for MASTER
extern uint8_t				LoraNodeCount;						// for MASTER
extern tLoraDeviceNode *		LoraGateWay;						// for SLAVE
extern tDeviceNodeSensor *		MySensor;						// for SLAVE
static uint16_t				TraceDataSaveTotal;
static uint16_t				TraceDataSavedNum;
static uint32_t				TraceDataCurrentSaveAddr;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/***************************************************************************************************
 *  Function Name: SaveRecord_WriteInLoraMode
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SaveRecord_WriteInLoraMode(void)
{
	uint16_t	val;

	if(SystemOperMode != SystemInNormal) return;

	if(EnableMaster == true) {
		val = 0x1234;
		FLASH_WriteHalfWord( MyWorkParaAddr, &val, 1 );
	}
}


/***************************************************************************************************
 *  Function Name: SaveRecord_WriteInMyselfParaAndLoraGateWayPara
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SaveRecord_WriteInMyselfParaAndLoraGateWayPara(void)
{
	uint8_t			prov[4] = {0};
	uint32_t		temp = 0, interval;
	SaveRecordMyPara_t	MyLoraPara;

	if(SystemOperMode == SystemInNormal) {
		memset((void *)&MyLoraPara.MyPara, 0, sizeof(tLoRaSettings));
		memcpy(( void * )&MyLoraPara.MyPara, ( void * )&LoRaSettings, sizeof(tLoRaSettings));
		if( MyLoraPara.MyPara.FreqHopOn ) {
			MyLoraPara.MyPara.RFFrequency = Lora_RFFrequency;
			MyLoraPara.MyPara.HopPeriod = Lora_RFHoppingPeriod;
		}

		// 起始位址附近取值,因存值都從起始開始存,有值就表示被使用了
		FLASH_ReadWord( MyWorkParaAddr + 4, &temp, 1 );
		FLASH_ReadByte( LoraGateWayParaAddr, prov, 4 );
		FLASH_ReadWord( LoraGateWayParaInterval, &interval, 1 );

		if((temp == 0xFFFFFFFF) && (prov[0] == 0xFF) && (prov[1] == 0xFF) && (prov[2] == 0xFF) && (prov[3] == 0xFF)) {
			FLASH_WriteByte( MyWorkParaAddr + 4, MyLoraPara.Value, sizeof(MyLoraPara.Value));
			if(LoraGateWay != NULL) {
				prov[0] = LoraGateWay->NodeAddress[0];
				prov[1] = LoraGateWay->NodeAddress[1];
				prov[2] = LoraGateWay->NodeAddress[2];
				prov[3] = 0;
				FLASH_WriteByte( LoraGateWayParaAddr, prov, 4 );
				interval = LoraGateWay->Interval;
				FLASH_WriteWord( LoraGateWayParaInterval, &interval, 1 );
			}
		} else {
			FLASH_EraseRecordPage( MyWorkParaAddr );
			FLASH_WriteByte( MyWorkParaAddr + 4, MyLoraPara.Value, sizeof(MyLoraPara.Value));
			if(EnableMaster == true) {
				FLASH_WriteByte( LoraGateWayParaAddr, prov, 4 );
				FLASH_WriteWord( LoraGateWayParaInterval, &interval, 1 );
			} else {
				if(LoraGateWay != NULL) {
					prov[0] = LoraGateWay->NodeAddress[0];
					prov[1] = LoraGateWay->NodeAddress[1];
					prov[2] = LoraGateWay->NodeAddress[2];
					prov[3] = 0;
					FLASH_WriteByte( LoraGateWayParaAddr, prov, 4 );
					interval = LoraGateWay->Interval;
					FLASH_WriteWord( LoraGateWayParaInterval, &interval, 1 );
					// Console_Output_String( "WriteInMyselfPara\r\n" );
				}
			}
		}
	}
}


/***************************************************************************************************
 *  Function Name: SaveRecord_ReadOutMyselfPara
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SaveRecord_ReadOutMyselfPara(void)
{
	SaveRecordMyPara_t	MyLoraPara;
	uint16_t		val;
	uint32_t		temp;

	if(SystemOperMode == SystemInNormal) {
		// 起始位址附近取值,因存值都從起始開始存,有值就表示被使用了
		FLASH_ReadWord( MyWorkParaAddr + 4, &temp, 1 );
		if(temp == 0xFFFFFFFF) return;

		FLASH_ReadHalfWord( MyWorkParaAddr, &val, 1 );
		if(val == 0x1234) {
			EnableMaster = true;
		} else {
			EnableMaster = false;
		}

		memset((void *)&MyLoraPara.MyPara, 0, sizeof(tLoRaSettings));
		FLASH_ReadByte( MyWorkParaAddr + 4, MyLoraPara.Value, sizeof(MyLoraPara.Value));
		if( MyLoraPara.MyPara.FreqHopOn ) {
			MyLoraPara.MyPara.RFFrequency = Lora_RFFrequency;
			MyLoraPara.MyPara.HopPeriod = Lora_RFHoppingPeriod;
		}
		memcpy(( void * )&LoRaSettings, ( void * )&MyLoraPara.MyPara, sizeof(tLoRaSettings));
	}
}


/***************************************************************************************************
 *  Function Name: SaveRecord_ReadOutLoraGateWayPara
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Readme : 內部有一串判斷 "== 0xFF" 就直接返回,須注意 LoraGateWay 的位址不能是 0xFFFFFF
 **************************************************************************************************/
void	SaveRecord_ReadOutLoraGateWayPara(void)
{
	uint8_t		para[4] = {0};
	uint32_t	interval;

	if((SystemOperMode == SystemInNormal) && (EnableMaster == false)) {
		FLASH_ReadByte( LoraGateWayParaAddr, para, 4 );
		if((para[0] == 0xFF) && (para[1] == 0xFF) && (para[2] == 0xFF) && (para[3] == 0xFF)) return;

		FLASH_ReadWord( LoraGateWayParaInterval, &interval, 1 );
		if(LoraGateWay == NULL) LoraGateWay = (tLoraDeviceNode *) malloc(sizeof(tLoraDeviceNode));
		if(MySensor == NULL) MySensor = (tDeviceNodeSensor *) malloc(sizeof(tDeviceNodeSensor));

		if((LoraGateWay != NULL) && (MySensor != NULL)) {
			memset((void *)LoraGateWay, 0, sizeof(tLoraDeviceNode));
			memset((void *)MySensor, 0, sizeof(tDeviceNodeSensor));

			LoraGateWay->NodeAddress[0] = para[0];
			LoraGateWay->NodeAddress[1] = para[1];
			LoraGateWay->NodeAddress[2] = para[2];

			if((interval & 0xFFFF0000) == 0) {
				LoraGateWay->Interval = (uint16_t)(interval & 0x0000FFFF);
			} else {
				LoraGateWay->Interval = 0;
			}
		}
	}
}


/***************************************************************************************************
 *  Function Name: SaveRecord_WriteInLoraNodePara
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SaveRecord_WriteInLoraNodePara(void)
{
	uint16_t		count, size;
	uint32_t		temp, save_page, erase_page;
	SaveRecordNodePara_t	prov;

	if((SystemOperMode == SystemInNormal) && (EnableMaster == true)) {
		// 起始位址附近取值,因存值都從起始開始存,有值就表示被使用了
		FLASH_ReadWord( LoraNodeParaPage1Addr + 4, &temp, 1 );
		if(temp != 0xFFFFFFFF) {
			save_page = LoraNodeParaPage2Addr;
			erase_page = LoraNodeParaPage1Addr;
		} else {
			save_page = LoraNodeParaPage1Addr;
			erase_page = LoraNodeParaPage2Addr;
		}
	} else {
		return;
	}

	temp = save_page + FLASH_PAGE_SIZE - 1;
	FLASH_WriteHalfWord( save_page, (uint16_t *)&LoraNodeCount, 1 );
	save_page += 4;
	size = sizeof(prov.Value);

	for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
		if(LoraNodeDevice[count] != NULL) {
			memset((void *)prov.Value, 0, size);
			memcpy(( void * )prov.NodePara.NodeAddress, ( void * )LoraNodeDevice[count]->NodeAddress, 3);
			memcpy(( void * )prov.NodePara.Aliases, ( void * )LoraNodeDevice[count]->Aliases, AliasesSize);
			prov.NodePara.Interval = LoraNodeDevice[count]->Interval;

			FLASH_WriteByte( save_page, prov.Value, size);
			save_page += size;

			if(save_page >= temp) break;
		}
	}
	FLASH_EraseRecordPage( erase_page );
}


/***************************************************************************************************
 *  Function Name: SaveRecord_ReadOutLoraNodePara
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SaveRecord_ReadOutLoraNodePara(void)
{
	uint16_t		count, size;
	uint32_t		temp, read_page;
	SaveRecordNodePara_t	prov;

	if((SystemOperMode == SystemInNormal) && (EnableMaster == true)) {
		// 起始位址附近取值,因存值都從起始開始存,有值就表示被使用了
		FLASH_ReadWord( LoraNodeParaPage1Addr + 4, &temp, 1 );
		if(temp != 0xFFFFFFFF) {
			read_page = LoraNodeParaPage1Addr;
		} else {
			// 起始位址附近取值,因存值都從起始開始存,有值就表示被使用了
			FLASH_ReadWord( LoraNodeParaPage2Addr + 4, &temp, 1 );
			if(temp != 0xFFFFFFFF) {
				read_page = LoraNodeParaPage2Addr;
			} else {
				return;
			}
		}
	} else {
		return;
	}

	FLASH_ReadHalfWord( read_page, (uint16_t *)&LoraNodeCount, 1 );
	read_page += 4;
	size = sizeof(prov.Value);

	for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
		memset((void *)prov.Value, 0xFF, size);
		FLASH_ReadByte( read_page, prov.Value, size );
		read_page += size;
		if((prov.Value[0] != 0xFF) || (prov.Value[1] != 0xFF) || (prov.Value[2] != 0xFF)) {
			if((LoraNodeDevice[count] == NULL) && (DeviceNodeSleepAndRandomHop[count] == NULL) && (DeviceNodeSensor[count] == NULL)) {
				LoraNodeDevice[count] = (tLoraDeviceNode *) malloc(sizeof(tLoraDeviceNode));
				DeviceNodeSleepAndRandomHop[count] = (tDeviceNodeSleepAndRandomHop *) malloc(sizeof(tDeviceNodeSleepAndRandomHop));
				DeviceNodeSensor[count] = (tDeviceNodeSensor *) malloc(sizeof(tDeviceNodeSensor));
			}

			if((LoraNodeDevice[count] != NULL) && (DeviceNodeSleepAndRandomHop[count] != NULL) && (DeviceNodeSensor[count] != NULL)) {
				memset((void *)LoraNodeDevice[count], 0, sizeof(tLoraDeviceNode));
				memset((void *)DeviceNodeSleepAndRandomHop[count], 0, sizeof(tDeviceNodeSleepAndRandomHop));
				memset((void *)DeviceNodeSensor[count], 0, sizeof(tDeviceNodeSensor));
				memcpy(( void * )LoraNodeDevice[count]->NodeAddress, ( void * )prov.NodePara.NodeAddress, 3);
				memcpy(( void * )LoraNodeDevice[count]->Aliases, ( void * )prov.NodePara.Aliases,  AliasesSize);
				LoraNodeDevice[count]->Interval = prov.NodePara.Interval;
				if(LoraNodeDevice[count]->Interval != 0) {
					DeviceNodeSleepAndRandomHop[count]->DefineLoraRxFailureTimes = DEF_Allowed_LoRa_Rx_Failure_Times_Times;
				} else {
					DeviceNodeSleepAndRandomHop[count]->DefineLoraRxFailureTimes = 0;
				}
			}
		} else {
			break;
		}
	}
}


/***************************************************************************************************
 *  Function Name: SaveRecord_TraceDataSpaceInitialization
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SaveRecord_TraceDataSpaceInitialization(void)
{
	uint8_t		temp[64];
	uint16_t	count;
	uint32_t	read_addr;
	uint32_t	over_addr;

	if((SystemOperMode == SystemInNormal) && (EnableMaster == false)) {
		TraceDataSaveTotal = (LoraNodeTraceDataEndAddr - LoraNodeTraceDataStarAddr + 1) / sizeof(tLoraNodeSensor);
		read_addr = LoraNodeTraceTableAddr;
		over_addr = LoraNodeTraceTableAddr + (TraceDataSaveTotal * 2) - 1;
		do {
			FLASH_ReadByte( read_addr, temp, 64 );
			read_addr += 64;
			if((temp[0] != 0xFF) && (temp[1] != 0xFF)) {
				for(count = 0 ; count < 64 ; count+=2) {
					if((temp[count] == 0) && (temp[count + 1] == 0)) TraceDataSavedNum++;
				}
			} else {
				break;
			}
		} while ( read_addr < over_addr );

		TraceDataCurrentSaveAddr = LoraNodeTraceDataStarAddr + (TraceDataSavedNum * sizeof(tLoraNodeSensor));
	}
}


/***************************************************************************************************
 *  Function Name: SaveRecord_WriteInTraceData
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SaveRecord_WriteInTraceData(void)
{
	uint16_t			count, size;
	SaveRecordNodeTraceData_t	temp;

	if((SystemOperMode == SystemInNormal) && (EnableMaster == false)) {
		if(TraceDataSavedNum < TraceDataSaveTotal) {
			if(MySensor != NULL) {
				// Console_Output_String( "WriteInTraceData\r\n" );

				memcpy(( void * )&temp.TraceData.NodeSensorData, ( void * )MySensor, sizeof(tDeviceNodeSensor));
				size = sizeof(temp.TraceData.Identifier);
				for(count = 0 ; count < size ; count++) temp.TraceData.Identifier[count] = count;

				size = sizeof(temp.Value);
				FLASH_WriteByte( TraceDataCurrentSaveAddr, temp.Value, size );
				TraceDataCurrentSaveAddr += size;

				size = 0x0000;
				FLASH_WriteHalfWord( (LoraNodeTraceTableAddr + (TraceDataSavedNum * 2)), &size, 1 );

				TraceDataSavedNum++;
			}
		}
	}
}


/***************************************************************************************************
 *  Function Name: SaveRecord_ReadOutTraceData
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SaveRecord_ReadOutTraceData(void)
{
	uint16_t			count = 0, size;
	SaveRecordNodeTraceData_t	temp;
	uint32_t			addr;
	int8_t				str[10];

	if((SystemOperMode == SystemInNormal) && (EnableMaster == false)) {
		if(TraceDataSavedNum == 0) {
			Console_Output_String( "No Data!\r\n" );
			return;
		}

		size = sizeof(temp.Value);
		addr = LoraNodeTraceDataStarAddr;
		do {
			FLASH_ReadByte( addr, temp.Value, size );
			addr += size;
			count++;

			// com port 輸出
			// 因 RSSI 值全交給 MASTER 端判斷,目前 SLAVE 端沒有將 RSSI 值讀入存下,故不做輸出
			// sprintf((char *)str, "%3.2f", temp.TraceData.NodeSensorData.RSSI);
			// Console_Output_String( "RSSI=" );
			// Console_Output_String( (const char *)str );
			// Console_Output_String( " " );

			Console_Output_String( "GPS-Latitude=" );
			snprintf( (char *)str, sizeof(str), "%d", temp.TraceData.NodeSensorData.GPS_Latitude );
			Console_Output_String( (const char *)str );
			Console_Output_String( " " );
			Console_Output_String( "GPS-Longitude=" );
			snprintf( (char *)str, sizeof(str), "%d", temp.TraceData.NodeSensorData.GPS_Longitude );
			Console_Output_String( (const char *)str );
			Console_Output_String( " " );
			Console_Output_String( "UTC-Time=" );
			snprintf( (char *)str, sizeof(str), "%u", temp.TraceData.NodeSensorData.UTC );
			Console_Output_String( (const char *)str );
			Console_Output_String( " " );
			Console_Output_String( "Battery=" );
			snprintf( (char *)str, sizeof(str), "%u", temp.TraceData.NodeSensorData.Battery );
			Console_Output_String( (const char *)str );
			Console_Output_String( "\r\n" );

			// 詢問確認此處的輸出格式(與藍芽)
		} while ( count < TraceDataSavedNum );

		if(count != 0) {
			snprintf( (char *)str, sizeof(str), "%u", count );
			Console_Output_String( "Total=" );
			Console_Output_String( (const char *)str );
			Console_Output_String( "\r\n" );
		}
	}
}


/***************************************************************************************************
 *  Function Name: SaveRecord_isTraceDataSpaceFull
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
bool	SaveRecord_isTraceDataSpaceFull(void)
{
	if((SystemOperMode == SystemInNormal) && (EnableMaster == false)) {
		if(TraceDataSavedNum >= TraceDataSaveTotal) return true;
	}
	return false;
}


/***************************************************************************************************
 *  Function Name: SaveRecord_ClearTraceDataSpace
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SaveRecord_ClearTraceDataSpace(void)
{
	uint32_t	addr;

	if(SystemOperMode == SystemInNormal) {
		// Console_Output_String( "ClearTraceData\r\n" );		// for test
		addr = LoraNodeTraceDataStarPage;
		do {
			FLASH_EraseRecordPage( addr );
			addr += LoraNodeTraceDataPageSize;
		} while ( addr <= LoraNodeTraceDataEndPage );

		FLASH_EraseRecordPage( LoraNodeTraceTableAddr );
		TraceDataSavedNum = 0;
		TraceDataCurrentSaveAddr = LoraNodeTraceDataStarAddr;
		TraceDataSaveTotal = (LoraNodeTraceDataEndAddr - LoraNodeTraceDataStarAddr + 1) / sizeof(tLoraNodeSensor);
	}
}


/***************************************************************************************************
 *  Function Name: SaveRecord_ClearAllSaveRecordSpace
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SaveRecord_ClearAllSaveRecordSpace(void)
{
	if(SystemOperMode == SystemInNormal) {
		// Console_Output_String( "ClearAllStart\r\n" );
		FLASH_EraseRecordPage( MyWorkParaAddr );
		FLASH_EraseRecordPage( LoraNodeParaPage1Addr );
		FLASH_EraseRecordPage( LoraNodeParaPage2Addr );
		SaveRecord_ClearTraceDataSpace();
		// Console_Output_String( "ClearAllDown\r\n" );
	}
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
