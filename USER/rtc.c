/**
  ******************************************************************************
  * @file    Project/ARM-Lora/rtc.c 
  * @author  JC
  * @version V1.0.0
  * @date    06-Jun-2016
  * @brief   RTC program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdbool.h>
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif
#include "rtc.h"
#include "sleep.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t Running_TimeCount = 0;              //for MASTER & SLAVE
__IO uint16_t Sleep_TimeCount = 0;                //for SLAVE
extern __IO bool isInSleep;                       //for SLAVE
extern tLoraDeviceNode *LoraGateWay;              //for SLAVE
extern bool EnableMaster;        // 1=Master or 0=Slave selection
extern tLoraDeviceNode *LoraNodeDevice[MAX_LoraNodeNum];      //for MASTER
extern tDeviceNodeSleepAndRandomHop *DeviceNodeSleepAndRandomHop[MAX_LoraNodeNum];  //for MASTER 
extern tDeviceNodeSensor *DeviceNodeSensor[MAX_LoraNodeNum];  //for MASTER
extern uint8_t LoraNodeCount;                                 //for MASTER

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/***************************************************************************************************
 *  Function Name: RTC_TimerConfig
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void RTC_TimerConfig(void) {
  
  RTC_InitTypeDef  RTC_InitStructure;
  RTC_TimeTypeDef  RTC_TimeStruct;
	
	/* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
  
  /* Reset RTC Domain */
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
	
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);
  
  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
  
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  /* Configure the RTC data register and RTC prescaler */
  /* ck_spre(1Hz) = RTCCLK(LSIorLSE) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
  RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
  RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
  RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);
  
  /* Set the time to 00h 00mn 00s AM */
  RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
  RTC_TimeStruct.RTC_Hours   = 0;
  RTC_TimeStruct.RTC_Minutes = 0;
  RTC_TimeStruct.RTC_Seconds = 0;  
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
  
}



/***************************************************************************************************
 *  Function Name: RTC_AlarmConfig
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void RTC_AlarmConfig(void) {
  
  EXTI_InitTypeDef EXTI_InitStructure;
  RTC_AlarmTypeDef RTC_AlarmStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* EXTI configuration */
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Alarm Interrupt */
#ifdef STM32F401xx
  NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
#endif
#ifdef STM32F072
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
#endif
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
  /* Set the alarm A Masks */
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All;
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0x0;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes =0x0;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds =0x0;
  RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
  
  /* Set alarm A sub seconds and enable SubSec Alarm : generate 1 interrupts per Second */
  RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0xFF, RTC_AlarmSubSecondMask_SS14_8);

  /* Enable alarm A interrupt */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	
}



/***************************************************************************************************
 *  Function Name: RTC_AlarmRun
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void RTC_AlarmRun(void) {
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
  
  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
  
  /* Enable the alarmA */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
  
}



/***************************************************************************************************
 *  Function Name: RTC_AlarmStop
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void RTC_AlarmStop(void) {
	
	/* Disable the RTC Clock */
  RCC_RTCCLKCmd(DISABLE);
  
  /* Disable the alarm */
  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
}



/***************************************************************************************************
 *  Function Name: RTC_Alarm_IRQHandler
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
#ifdef STM32F401xx
void RTC_Alarm_IRQHandler(void) {
#elif STM32F072
void RTC_IRQHandler(void) {
#endif
  
  static uint8_t LoraNum;
  uint8_t status;
  
  /* Check on the Alarm A flag and on the number of interrupts per Second (60*8) */
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET) {
    
    /* Clear RTC AlarmA Flags */
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    
    if((EnableMaster == false) && (LoraGateWay != NULL)) {     // SLAVE & Joined the Acsip-LoraNet
      
      if(isInSleep == false) {                                 // MCU running
        Running_TimeCount++;
      } else {                                                 // MCU in STOP mode
        Sleep_TimeCount++;
      }
      
    } else if(EnableMaster == true) {                          // MASTER
      
      Running_TimeCount++;
      status = (uint8_t)(Running_TimeCount % SecondOfOneTimes); 
      //status = (uint8_t)(Running_TimeCount & 0x0001);          //every 2 seconds
      
      for(LoraNum = 0 ; LoraNum < MAX_LoraNodeNum ; LoraNum++) {
        if((LoraNodeDevice[LoraNum] != NULL) && (DeviceNodeSleepAndRandomHop[LoraNum] != NULL)) {
          if((DeviceNodeSleepAndRandomHop[LoraNum]->isNowSleeping == false) && (status == 0)) {
            if(DeviceNodeSleepAndRandomHop[LoraNum]->EventCountPriority2 < 3) {
              LoraLinkListEvent_BuildLoraEvent(LoraEventPriority2, LoraNum, Master_AcsipProtocol_Poll, LoraNodeDevice[LoraNum]->NodeAddress, NULL, NULL);
            }
          } else if(Running_TimeCount >= DeviceNodeSleepAndRandomHop[LoraNum]->WakeUpTimePoint) {
            DeviceNodeSleepAndRandomHop[LoraNum]->isNowSleeping = false;
            DeviceNodeSleepAndRandomHop[LoraNum]->WakeUpTimePoint = 0;
          }
        }
      }
      
    }
    
	}
  
  /* Clear the EXTI line 17 */
  EXTI_ClearITPendingBit(EXTI_Line17);
  
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


