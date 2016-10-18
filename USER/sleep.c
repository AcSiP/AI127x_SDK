/**
  ******************************************************************************
  * @file    Project/ARM-Lora/sleep.c 
  * @author  JC
  * @version V1.0.0
  * @date    07-Jun-2016
  * @brief   SLEEP program body
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
#include "main.h"
#include "sleep.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO bool isInSleep = false;                    //for SLAVE
extern __IO uint16_t Running_TimeCount;         //for MASTER & SLAVE
extern tDeviceNodeSleepAndRandomHop *DeviceNodeSleepAndRandomHop[MAX_LoraNodeNum];  //for MASTER
extern uint8_t EventCountPriority0, EventCountPriority1, EventCountPriority2;  //for MASTER
extern tLoraDeviceNode *LoraGateWay;            //for SLAVE
extern tDeviceNodeSensor *MySensor;             //for SLAVE
extern __IO uint16_t Sleep_TimeCount;           //for SLAVE
//extern __IO bool GPS_Located;                 //for SLAVE
extern __IO bool Slave_PollEventAccomplish;     //for SLAVE
extern __IO bool Slave_PollEvent;               //for SLAVE
extern __IO bool Slave_PollEvent_UTCnotZero;    //for SLAVE
extern bool EnableMaster;                       // 1=Master or 0=Slave selection
extern tRadioDriver *Radio;
extern __IO tLoraRunningEvent LoraRunningEvent;
extern __IO bool GPS_HandmadeOff;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/***************************************************************************************************
 *  Function Name: SLEEP_SYSCLKConfigFromSTOPMode
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void SLEEP_SYSCLKConfigFromSTOPMode(void) {
  
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* Wait till HSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
  
  /* Enable PLL */
  RCC_PLLCmd(ENABLE);
  
  /* Wait till PLL is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  
  /* Select PLL as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* Wait till PLL is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x08);
  
}



/***************************************************************************************************
 *  Function Name: SLEEP_SlaveSleepVariableSet
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void SLEEP_SlaveSleepVariableSet(void) {
  
  Running_TimeCount = 0;
  //GPS_Located = false;
  Slave_PollEvent = false;
  Slave_PollEvent_UTCnotZero = false;
  Slave_PollEventAccomplish = false;
  isInSleep = false;
  
}



/***************************************************************************************************
 *  Function Name: SLEEP_SlaveSleep
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void SLEEP_SlaveSleep(uint16_t *SleepTime) {
  
  if(*SleepTime == 0) {
    isInSleep = false;
    return;
  }
  
  SX1276LoRaSetOpMode(RFLR_OPMODE_SLEEP);
  
  GPS_MT3333Stop();
  if(MySensor != NULL) {
    memset((void *)MySensor, 0, sizeof(tDeviceNodeSensor));
  }
  
  Sleep_TimeCount = 0;
  
  isInSleep = true;
  
  while(true) {
    if(Sleep_TimeCount < *SleepTime) {
      /* Enter Stop Mode */
      PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    } else {
      break;
    }
  }
  
  SLEEP_SYSCLKConfigFromSTOPMode();
  
  if(GPS_HandmadeOff == false) {
    GPS_MT3333Run();
  }
  
  SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
  Radio->StartRx();
  
  isInSleep = false;
  
}



/***************************************************************************************************
 *  Function Name: SLEEP_SlaveSleepProcedure
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void SLEEP_SlaveSleepProcedure(uint16_t *SleepTime) {
  
  if((EnableMaster != false) || (LoraGateWay == NULL) || (*SleepTime == 0)) {
    Running_TimeCount = 0;
    return;
  }
  
  if((Running_TimeCount >= GPSnoLocated_RunningTime) /*&& (GPS_Located == false)*/ && (Slave_PollEventAccomplish == false)) {
    SLEEP_SlaveSleep(SleepTime);
    Running_TimeCount = 0;
    //GPS_Located = false;
  } else if(Slave_PollEventAccomplish == true /*&& (GPS_Located == true)*/) {
    SLEEP_SlaveSleep(SleepTime);
    Running_TimeCount = 0;
    //GPS_Located = false;
    Slave_PollEventAccomplish = false;
  }
  
}



/***************************************************************************************************
 *  Function Name: SLEEP_MasterSleepProcedure
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void SLEEP_MasterSleepProcedure(void) {
  
  uint8_t count;
  bool sleep = false;
  
  if(LoraRunningEvent.RunNodeEvent != 0)        //表目前有事件正在執行中
    return;
  
  if(EventCountPriority0 != 0)                  //表共同事件排程裡有事件準備執行
    return;
  if(EventCountPriority1 != 0)                  //表共同事件排程裡有事件準備執行
    return;
  if(EventCountPriority2 != 0)                  //表共同事件排程裡有事件準備執行
    return;
  
  for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
    //此 SLAVE Node 裝置是否存在,不存在就跳到下一值去判斷
    if(DeviceNodeSleepAndRandomHop[count] == NULL) {
      continue;
    }
    //判斷個別事件排程裡是否有事件準備執行
    if(DeviceNodeSleepAndRandomHop[count]->isNowSleeping == false) {
      if(DeviceNodeSleepAndRandomHop[count]->EventCountPriority0 != 0)
        return;
      if(DeviceNodeSleepAndRandomHop[count]->EventCountPriority1 != 0)
        return;
      if(DeviceNodeSleepAndRandomHop[count]->EventCountPriority2 != 0)
        return;
    }
  }
  
  sleep = true;
  while(sleep) {
    sleep = false;
    /* Lora sx1276/78 into sleep mode */
    SX1276LoRaSetOpMode(RFLR_OPMODE_SLEEP);
    /* Disable SysTick ISR */
    SysTick->CTRL &= (~SysTick_CTRL_TICKINT_Msk);
    /* Enter SLEEP mode */
    NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,DISABLE);
    NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT,DISABLE);
    __WFI();
  }
  
  SLEEP_SYSCLKConfigFromSTOPMode();
  
  /* Enable SysTick ISR */
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  
  /* Lora sx1276/78 into running mode */
  SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
  Radio->StartRx();
  
  /* 進入 SLEEP Mode 必須先將 SYS_TICK 中斷關閉,
     不然一睡下去之後,立刻 SYS_TICK 中斷產生進入
     執行,如此根本不用睡了,因為 SYS_TICK=1ms */
  
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


