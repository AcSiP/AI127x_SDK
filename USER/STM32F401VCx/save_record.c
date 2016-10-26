
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
extern __IO uint32_t			SystemOperMode;
extern bool				EnableMaster;		// 1=Master or 0=Slave selection
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
 *  Function Name: SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara(void)
{
	uint8_t *		node_saved_data = NULL;
	uint8_t			prov[4] = {0};
	uint16_t		count, size;
	uint32_t		temp, node_save_addr, interval;
	SaveRecordMyPara_t	MyLoraPara;
	SaveRecordNodePara_t	SaveNodePara;

	if(SystemOperMode == SystemInNormal) {
		FLASH_ReadWord( MyWorkParaAddr + 4, &temp, 1 );
		if((temp != 0xFFFFFFFF) && (EnableMaster == true)) {
			FLASH_ReadByte( LoraGateWayParaAddr, prov, 4 );
			FLASH_ReadWord( LoraGateWayParaInterval, &interval, 1 );
			FLASH_EraseRecordSector(WorkParaSector);
		} else {
			if((temp != 0xFFFFFFFF) && (EnableMaster == false)) {
				FLASH_ReadWord( LoraNodeParaAddr + 4, &temp, 1 );
				if(temp != 0xFFFFFFFF) {
					size = (sizeof(SaveRecordNodePara_t) * MAX_LoraNodeNum) + 4;
					node_saved_data = (uint8_t *) malloc(size);
					if(node_saved_data != NULL) FLASH_ReadByte( LoraNodeParaAddr, node_saved_data, size );
				}
				FLASH_EraseRecordSector(WorkParaSector);
			}
		}

		memset((void *)&MyLoraPara.MyPara, 0, sizeof(tLoRaSettings));
		memcpy(( void * )&MyLoraPara.MyPara, ( void * )&LoRaSettings, sizeof(tLoRaSettings));
		MyLoraPara.MyPara.RFFrequency = Lora_RFFrequency;
		MyLoraPara.MyPara.HopPeriod = Lora_RFHoppingPeriod;
		FLASH_WriteByte( MyWorkParaAddr + 4, MyLoraPara.Value, sizeof(MyLoraPara.Value));

		if(EnableMaster == true) {
			if((prov[0] != 0XFF) || (prov[1] != 0XFF) || (prov[2] != 0XFF)) {
				FLASH_WriteByte( LoraGateWayParaAddr, prov, 4 );
				FLASH_WriteWord( LoraGateWayParaInterval, &interval, 1 );
			}

			node_save_addr = LoraNodeParaAddr;
			FLASH_WriteHalfWord( node_save_addr, (uint16_t *)&LoraNodeCount, 1 );
			node_save_addr += 4;
			size = sizeof(SaveNodePara.Value);
			temp = WorkParaSector + WorkParaSectorSize - 1;

			for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
				if(LoraNodeDevice[count] != NULL) {
					memset((void *)SaveNodePara.Value, 0, size);
					memcpy(( void * )SaveNodePara.NodePara.NodeAddress, ( void * )LoraNodeDevice[count]->NodeAddress, 3);
					memcpy(( void * )SaveNodePara.NodePara.Aliases, ( void * )LoraNodeDevice[count]->Aliases, AliasesSize);
					SaveNodePara.NodePara.Interval = LoraNodeDevice[count]->Interval;

					FLASH_WriteByte( node_save_addr, SaveNodePara.Value, size);
					node_save_addr += size;
					if(node_save_addr >= temp) break;
				}
			}
		} else {
			if(LoraGateWay != NULL) {
				prov[0] = LoraGateWay->NodeAddress[0];
				prov[1] = LoraGateWay->NodeAddress[1];
				prov[2] = LoraGateWay->NodeAddress[2];
				prov[3] = 0;
				FLASH_WriteByte( LoraGateWayParaAddr, prov, 4 );
				interval = LoraGateWay->Interval;
				FLASH_WriteWord( LoraGateWayParaInterval, &interval, 1 );
			}

			if(node_saved_data != NULL) {
				FLASH_WriteByte( LoraNodeParaAddr, node_saved_data, size );
				free(node_saved_data);
				node_saved_data = NULL;
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
		MyLoraPara.MyPara.RFFrequency = Lora_RFFrequency;
		MyLoraPara.MyPara.HopPeriod = Lora_RFHoppingPeriod;
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
		FLASH_ReadWord( LoraNodeParaAddr + 4, &temp, 1 );
		if(temp == 0xFFFFFFFF) return;
	} else {
		return;
	}

	read_page = LoraNodeParaAddr;
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
					DeviceNodeSleepAndRandomHop[count]->DefineLoraRxFailureTimes = ceil(LoraNodeDevice[count]->Interval / SecondOfOneTimes);
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
	uint16_t			size;
	uint32_t			read_addr;
	SaveRecordNodeTraceData_t	temp;

	if((SystemOperMode == SystemInNormal) && (EnableMaster == false)) {
		TraceDataSaveTotal = (LoraNodeTraceDataEndAddr - LoraNodeTraceDataStarAddr + 1) / sizeof(tLoraNodeSensor);
		read_addr = LoraNodeTraceDataStarAddr;
		size = sizeof(temp.Value);
		memset((void *)temp.Value, 0xFF, size);
		do {
			FLASH_ReadByte( read_addr, temp.Value, size );
			read_addr += size;
			if(temp.TraceData.NodeSensorData.GPS_Latitude != 0xFFFFFFFF) {
				TraceDataSavedNum++;
			} else {
				break;
			}
		} while ( read_addr < LoraNodeTraceDataEndAddr );

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
	uint8_t				count, size;
	SaveRecordNodeTraceData_t	temp;

	if((SystemOperMode == SystemInNormal) && (EnableMaster == false)) {
		if(TraceDataSavedNum < TraceDataSaveTotal) {
			if(MySensor != NULL) {
				memcpy(( void * )&temp.TraceData.NodeSensorData, ( void * )MySensor, sizeof(tDeviceNodeSensor));
				size = sizeof(temp.TraceData.Identifier);
				for(count = 0 ; count < size ; count++) temp.TraceData.Identifier[count] = count;

				size = sizeof(temp.Value);
				FLASH_WriteByte( TraceDataCurrentSaveAddr, temp.Value, size );
				TraceDataCurrentSaveAddr += size;
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

			// 確認此處的輸出格式(與藍芽)
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
	if(SystemOperMode == SystemInNormal) {
		FLASH_EraseRecordSector(LoraNodeTraceDataSector);
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
		FLASH_EraseRecordSector(WorkParaSector);
		SaveRecord_ClearTraceDataSpace();
	}
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
