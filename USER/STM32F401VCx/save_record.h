
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
  * @file    Project/ARM-Lora/save_record.h 
  * @author  JC
  * @version V1.0.0
  * @date    29-Apr-2016
  * @brief   Header for save_record.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_STM32F401VCX_SAVE_RECORD_H_
#define USER_STM32F401VCX_SAVE_RECORD_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Private define ------------------------------------------------------------*/
#define  WorkParaSectorSize		((uint32_t)0x00004000)		// 16K
#define  WorkParaSector			(ADDR_FLASH_SECTOR_1)
#define  MyWorkParaAddr			(ADDR_FLASH_SECTOR_1)
#define  LoraGateWayParaAddr		(ADDR_FLASH_SECTOR_1 + 0x800)
#define  LoraGateWayParaInterval	(ADDR_FLASH_SECTOR_1 + 0x900)
#define  LoraNodeParaAddr		(ADDR_FLASH_SECTOR_1 + 0x1000)
#define  LoraNodeTraceDataSectorSize	((uint32_t)0x00004000)		// 16K
#define  LoraNodeTraceDataSector	(ADDR_FLASH_SECTOR_2)
#define  LoraNodeTraceDataStarAddr	(ADDR_FLASH_SECTOR_2)
#define  LoraNodeTraceDataEndAddr	(ADDR_FLASH_SECTOR_2 + LoraNodeTraceDataSectorSize - 1)

/* Exported types ------------------------------------------------------------*/
typedef union {
	tLoRaSettings		MyPara;
	uint8_t			Value[32];
} SaveRecordMyPara_t;

typedef struct sSaveRecordLoraDeviceNode {
	uint8_t			NodeAddress[3];
	uint8_t			Aliases[AliasesSize];
	uint16_t		Interval;
} tLoraNodePara;

typedef union {
	tLoraNodePara		NodePara;
	uint8_t			Value[38];
} SaveRecordNodePara_t;

typedef struct sSaveRecordLoraDeviceNodeData {
	tDeviceNodeSensor	NodeSensorData;
	uint8_t			Identifier[3];
} tLoraNodeSensor;

typedef union {
	tLoraNodeSensor		TraceData;
	uint8_t			Value[32];
} SaveRecordNodeTraceData_t;

/* Private define ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	SaveRecord_WriteInLoraMode(void);
void	SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara(void);
void	SaveRecord_ReadOutMyselfPara(void);
void	SaveRecord_ReadOutLoraGateWayPara(void);
void	SaveRecord_ReadOutLoraNodePara(void);
void	SaveRecord_TraceDataSpaceInitialization(void);
void	SaveRecord_WriteInTraceData(void);
void	SaveRecord_ReadOutTraceData(void);
bool	SaveRecord_isTraceDataSpaceFull(void);
void	SaveRecord_ClearTraceDataSpace(void);
void	SaveRecord_ClearAllSaveRecordSpace(void);

#endif		// USER_STM32F401VCX_SAVE_RECORD_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
