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
#ifndef __SAVE_RECORD_H
#define __SAVE_RECORD_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  MyWorkParaAddr                 (ADDR_FLASH_PAGE_48)
#define  LoraGateWayParaAddr            (ADDR_FLASH_PAGE_48 + 0x400)
#define  LoraGateWayParaInterval        (ADDR_FLASH_PAGE_48 + 0x500)
#define  LoraNodeParaPage1Addr          (ADDR_FLASH_PAGE_49)
#define  LoraNodeParaPage2Addr          (ADDR_FLASH_PAGE_50)
#define  LoraNodeTraceTableAddr         (ADDR_FLASH_PAGE_51)
#define  LoraNodeTraceDataPageSize      (FLASH_PAGE_SIZE)
#define  LoraNodeTraceDataStarPage      (ADDR_FLASH_PAGE_52)
#define  LoraNodeTraceDataEndPage       (ADDR_FLASH_PAGE_63)
#define  LoraNodeTraceDataStarAddr      (ADDR_FLASH_PAGE_52)
#define  LoraNodeTraceDataEndAddr       (LoraNodeTraceDataEndPage + FLASH_PAGE_SIZE - 1)

/* Exported types ------------------------------------------------------------*/
typedef union {
  tLoRaSettings MyPara;
  uint8_t Value[32];
}SaveRecordMyPara_t;

typedef struct sSaveRecordLoraDeviceNode {
	uint8_t NodeAddress[3];
  uint8_t Aliases[AliasesSize];
  uint16_t Interval;
}tLoraNodePara;

typedef union {
  tLoraNodePara NodePara;
  uint8_t Value[38];
}SaveRecordNodePara_t;

typedef struct sSaveRecordLoraDeviceNodeData {
	tDeviceNodeSensor NodeSensorData;
  uint8_t Identifier[3];
}tLoraNodeSensor;

typedef union {
  tLoraNodeSensor TraceData;
  uint8_t Value[32];
}SaveRecordNodeTraceData_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SaveRecord_WriteInLoraMode(void);
void SaveRecord_WriteInMyselfParaAndLoraGateWayPara(void);
void SaveRecord_ReadOutMyselfPara(void);
void SaveRecord_ReadOutLoraGateWayPara(void);
void SaveRecord_WriteInLoraNodePara(void);
void SaveRecord_ReadOutLoraNodePara(void);
void SaveRecord_TraceDataSpaceInitialization(void);
void SaveRecord_WriteInTraceData(void);
void SaveRecord_ReadOutTraceData(void);
bool SaveRecord_isTraceDataSpaceFull(void);
void SaveRecord_ClearTraceDataSpace(void);
void SaveRecord_ClearAllSaveRecordSpace(void);



#endif /* __SAVE_RECORD_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


