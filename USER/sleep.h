/**
  ******************************************************************************
  * @file    Project/ARM-Lora/sleep.h 
  * @author  JC
  * @version V1.0.0
  * @date    07-Jun-2016
  * @brief   Header for sleep.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sleep_H
#define __sleep_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

#include "LinkListEvent.h"

/* Exported types ------------------------------------------------------------*/
typedef struct sDeviceNodeSleepAndRandomHop {
  //for sleep
  bool      isNowSleeping;
  uint16_t  WakeUpTimePoint;
  //for Lora disconnect
  uint16_t  DefineLoraRxFailureTimes;
  bool      isLoraDisconnecting;
  uint16_t  LoraRxFailureTimes;
  //for Lora Event
  tLoraNodeEvent *EventHeadPriority0;
  tLoraNodeEvent *EventTailPriority0;
  uint8_t EventCountPriority0;
  tLoraNodeEvent *EventHeadPriority1;
  tLoraNodeEvent *EventTailPriority1;
  uint8_t EventCountPriority1;
  tLoraNodeEvent *EventHeadPriority2;
  tLoraNodeEvent *EventTailPriority2;
  uint8_t EventCountPriority2;
}tDeviceNodeSleepAndRandomHop;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SLEEP_SYSCLKConfigFromSTOPMode(void);
void SLEEP_SlaveSleepVariableSet(void);
void SLEEP_SlaveSleep(uint16_t *);
void SLEEP_SlaveSleepProcedure(uint16_t *);
void SLEEP_MasterSleepProcedure(void);



#endif /* __sleep_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


