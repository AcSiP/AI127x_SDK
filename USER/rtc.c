
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
#define		LOCAL__DEBUG_OUTPUT	(true)
// #undef		LOCAL__DEBUG_OUTPUT
#define		LOCAL_USE_LSE		(true)
// #undef		LOCAL_USE_LSE
// #define		LOCAL_USE_LSI		(true)
#undef		LOCAL_USE_LSI
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t				Running_TimeCount = 0;					// for MASTER & SLAVE
__IO uint16_t				SLAVE_LoraPollEventInterval = 0;			// for SLAVE
__IO uint16_t				Sleep_TimeCount = 0;					// for SLAVE
extern __IO bool			isInSleep;						// for SLAVE
extern tLoraDeviceNode *		LoraGateWay;						// for SLAVE
extern __IO uint32_t			SystemOperMode;
extern bool				EnableMaster;						// 1=Master or 0=Slave selection
extern tLoraDeviceNode *		LoraNodeDevice[MAX_LoraNodeNum];			// for MASTER
extern tDeviceNodeSleepAndRandomHop *	DeviceNodeSleepAndRandomHop[MAX_LoraNodeNum];		// for MASTER
extern tDeviceNodeSensor *		DeviceNodeSensor[MAX_LoraNodeNum];			// for MASTER
extern uint8_t				LoraNodeCount;						// for MASTER

static	bool				RTC__Flag_IRQ_Configured = false;
static	bool				RTC__Flag_Configured = false;

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
void		RTC_TimerConfig(void)
{
	RTC_InitTypeDef		RTC_InitStructure;
	RTC_TimeTypeDef		RTC_TimeStruct;

	if( RTC__Flag_Configured ) return;

	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* Reset RTC Domain */
	RCC_BackupResetCmd(ENABLE);
	RCC_BackupResetCmd(DISABLE);

#if LOCAL_USE_LSE
	/* Enable the LSE OSC */
	RCC_LSEConfig(RCC_LSE_ON);

	/* Wait till LSE is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {
	}

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
#endif

#if LOCAL_USE_LSI
	/* LSI used as RTC source clock */
	/* The RTC Clock may varies due to LSI frequency dispersion. */   
	/* Enable the LSI OSC */ 
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) {
	}

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#endif

	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();

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
	RTC_SetTime( RTC_Format_BCD, &RTC_TimeStruct );

	RTC__Flag_Configured = true;
}

void		RTC_IRQ_Config( void )
{
	EXTI_InitTypeDef	EXTI_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;

	if( RTC__Flag_IRQ_Configured ) return;

	/* EXTI configuration */
	EXTI_ClearITPendingBit( EXTI_Line17 );
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init( & EXTI_InitStructure );

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
	NVIC_Init( & NVIC_InitStructure );

	RTC__Flag_IRQ_Configured = true;
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
void		RTC_AlarmConfig(void)
{
	RTC_AlarmTypeDef	RTC_AlarmStructure;

	RTC_IRQ_Config();

	/* Set the alarm A Masks */
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0x0;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x0;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x0;
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

	/* Set alarm A sub seconds and enable SubSec Alarm : generate 1 interrupts per Second */
	RTC_AlarmSubSecondConfig( RTC_Alarm_A, 0xFF, RTC_AlarmSubSecondMask_SS14_8 );

	/* Enable alarm A interrupt */
	RTC_ITConfig( RTC_IT_ALRA, ENABLE );
}

void		RTC_AlarmConfig_Timeout( uint32_t timeout_sec )
{
// 	RTC_DateTypeDef		RTC_DateStructure;
	RTC_AlarmTypeDef	RTC_AlarmStructure;
	RTC_TimeTypeDef		RTC_TimeStruct;
	uint32_t		hh, mm, ss, tt;
	char			buf[64];

	RTC_IRQ_Config();

	RTC_GetTime( RTC_Format_BIN, & RTC_TimeStruct );
// 	RTC_GetDate( RTC_Format_BIN, & RTC_DateStructure );

#ifdef	LOCAL__DEBUG_OUTPUT
	Console_Output_String( "RTC_TimeStruct=" );
// 	snprintf( buf, sizeof(buf), "20%02d-%02d-%02d  ", RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date );
// 	Console_Output_String( buf );
	snprintf( buf, sizeof(buf), "%02d:%02d:%02d\r\n", RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds );
	Console_Output_String( buf );
#endif

	hh = RTC_TimeStruct.RTC_Hours;
	mm = RTC_TimeStruct.RTC_Minutes;
	ss = RTC_TimeStruct.RTC_Seconds + timeout_sec;

	// process second part
	tt = ss % 60;
	ss -= tt;
	mm += ss / 60;
	ss = tt;

	// process minutes part
	tt = mm % 60;
	mm -= tt;
	hh += mm / 60;
	mm = tt;

	// process hour part
	hh %= 24;

#ifdef	LOCAL__DEBUG_OUTPUT
	Console_Output_String( "RTC_AlarmTime=" );
	snprintf( buf, sizeof(buf), "%02d:%02d:%02d\r\n", hh, mm, ss );
	Console_Output_String( buf );
#endif

	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 =	RTC_H12_AM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours =	hh;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes =	mm;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds =	ss;

	/* Set the alarm A Masks */
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;

	RTC_SetAlarm( RTC_Format_BIN, RTC_Alarm_A, & RTC_AlarmStructure );

	// Disable SubSec Alarm
	RTC_AlarmSubSecondConfig( RTC_Alarm_A, 0x00, RTC_AlarmSubSecondMask_None );

	/* Enable alarm A interrupt */
	RTC_ITConfig( RTC_IT_ALRA, ENABLE );
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
void		RTC_AlarmRun(void)
{
	/* Enable the RTC Clock */
// 	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronisation */
// 	RTC_WaitForSynchro();

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
void		RTC_AlarmStop(void)
{
	/* Disable the RTC Clock */
// 	RCC_RTCCLKCmd(DISABLE);

	/* Disable the alarm */
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
}

static void	RTC__Process_ISR( void )
{
//	static uint8_t	LoraNum;
//	uint8_t		status;

	if( SystemOperMode != SystemInNormal ) return;

	if ( ! EnableMaster ) {
		if ( isInSleep ) Sleep_TimeCount++;
	}

	if( ! EnableMaster && LoraGateWay ) {		// SLAVE & Joined the Acsip-LoraNet
		if( isInSleep == false ) {		// MCU running
			Running_TimeCount++;
			SLAVE_LoraPollEventInterval++;
		}
		return;
	}

	if( ! EnableMaster ) return;	// SLAVE

	// Check all events are done
	uint16_t	i;
	bool		flag_need_build_pool = true;

	Running_TimeCount++;
	for( i = 0; i < MAX_LoraNodeNum; i++ ){
		if( LoraNodeDevice[i] && DeviceNodeSleepAndRandomHop[i] ){
			if( DeviceNodeSleepAndRandomHop[i]->Event_Count[2] ){
				flag_need_build_pool = false;
				break;
			}
		}
	}

//	char	cb[128];
//	if( flag_need_build_pool ){
//		snprintf( cb, sizeof( cb ), "%d, flag_need_build_pool = %d \r\n", __LINE__, (int) flag_need_build_pool );
//		Console_Output_String( cb );
//	}

	for( i = 0; i < MAX_LoraNodeNum; i++ ){
		if( LoraNodeDevice[i] && DeviceNodeSleepAndRandomHop[i] ){
			if( Running_TimeCount >= DeviceNodeSleepAndRandomHop[i]->WakeUpTimePoint ){
				DeviceNodeSleepAndRandomHop[i]->isNowSleeping = false;
				DeviceNodeSleepAndRandomHop[i]->WakeUpTimePoint = 0;
			}

			if( ! DeviceNodeSleepAndRandomHop[i]->isNowSleeping && flag_need_build_pool ){
//				snprintf( cb, sizeof( cb ), "%d, RHop[%d]->P2 = %d \r\n", __LINE__, i, DeviceNodeSleepAndRandomHop[i]->Event_Head[ LoraEventPriority2 ] );
//				Console_Output_String( cb );

				LoraLinkListEvent_BuildLoraEvent( LoraEventPriority2, i, Master_AcsipProtocol_Poll, LoraNodeDevice[i]->NodeAddress, NULL, NULL );
			}
		}
	}

/*
	if( EnableMaster ) {		// MASTER
		Running_TimeCount++;
		status = (uint8_t)(Running_TimeCount % SecondOfOneTimes);
		// status = (uint8_t)(Running_TimeCount & 0x0001);		// every 2 seconds

		for(LoraNum = 0 ; LoraNum < MAX_LoraNodeNum ; LoraNum++) {

			if((LoraNodeDevice[LoraNum] != NULL) && (DeviceNodeSleepAndRandomHop[LoraNum] != NULL)) {
				if((DeviceNodeSleepAndRandomHop[LoraNum]->isNowSleeping == false) && (status == 0)) {
//					snprintf( cb, sizeof( cb ), "%d, RandomHop[%d]->P2 = %d \r\n", __LINE__, LoraNum, DeviceNodeSleepAndRandomHop[LoraNum]->EventCountPriority2 );
//					Console_Output_String( cb );

					if(DeviceNodeSleepAndRandomHop[LoraNum]->EventCountPriority2 == 0) {
						LoraLinkListEvent_BuildLoraEvent( LoraEventPriority2, LoraNum, Master_AcsipProtocol_Poll, LoraNodeDevice[LoraNum]->NodeAddress, NULL, NULL );
					}
				} else {
					if(Running_TimeCount >= DeviceNodeSleepAndRandomHop[LoraNum]->WakeUpTimePoint) {
						DeviceNodeSleepAndRandomHop[LoraNum]->isNowSleeping = false;
						DeviceNodeSleepAndRandomHop[LoraNum]->WakeUpTimePoint = 0;
					}
				}
			}
		}
	}
*/
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
void	RTC_Alarm_IRQHandler(void)
#elif STM32F072
void	RTC_IRQHandler(void)
#endif

{
	/* Check on the Alarm A flag and on the number of interrupts per Second (60*8) */
	if( RTC_GetITStatus( RTC_IT_ALRA ) != RESET ) {
		/* Clear RTC AlarmA Flags */
		RTC_ClearITPendingBit( RTC_IT_ALRA );

		RTC__Process_ISR();
	}

	/* Clear the EXTI line 17 */
	EXTI_ClearITPendingBit( EXTI_Line17 );
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
