
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
#include "gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO bool				isInSleep = false;					// for SLAVE
__IO uint8_t				SLAVE_LoraHoppingStartChannel = 0;			// for SLAVE
extern __IO uint16_t			Running_TimeCount;					// for MASTER & SLAVE
extern __IO uint16_t			SLAVE_LoraPollEventInterval;				// for SLAVE

extern tLoraDeviceNode *		LoraGateWay;				// for SLAVE
extern tDeviceNodeSensor *		MySensor;				// for SLAVE
extern __IO uint16_t			Sleep_TimeCount;			// for SLAVE
extern __IO bool			Slave_PollEventAccomplish;		// for SLAVE
extern __IO bool			Slave_PollEvent;			// for SLAVE
extern __IO bool			Slave_PollEvent_UTCnotZero;		// for SLAVE
extern bool				EnableMaster;				// 1=Master or 0=Slave selection
extern tRadioDriver *			Radio;
extern __IO tLoraRunningEvent		LoraRunningEvent;
extern __IO bool			GPS_HandmadeOff;

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
void	SLEEP_SYSCLKConfigFromSTOPMode(void)
{
	uint32_t	delay;

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) {
	}

	/* Enable PLL */
	RCC_PLLCmd(ENABLE);

	/* Wait till PLL is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
	}

	/* Select PLL as system clock source */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	/* Wait till PLL is used as system clock source */
	while (RCC_GetSYSCLKSource() != 0x08) {
	}

	delay = GET_TICK_COUNT( );
	while( ( GET_TICK_COUNT( ) - delay ) < TICK_RATE_MS( 20 ) ) {
	}
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
void	SLEEP_SlaveSleepVariableSet(void)
{
	Running_TimeCount = 0;
	SLAVE_LoraPollEventInterval = 0;
	// GPS_Located = false;
	Slave_PollEvent = false;
	Slave_PollEvent_UTCnotZero = false;
	Slave_PollEventAccomplish = false;
	isInSleep = false;
}


void		SLEEP_SlaveSleep( uint16_t *SleepTime_sec )
{
	if( ! SleepTime_sec ) return;
	if( *SleepTime_sec == 0 ) {
		isInSleep = false;
		return;
	}

	if( *SleepTime_sec > 10 ) {
		*SleepTime_sec -= 5;
		SLEEP_SlaveSleep_STANDBY_Mode( SleepTime_sec );
		return;
	}

	SLEEP_SlaveSleep_STOP_Mode( SleepTime_sec );
}


void	GPIOs__AnalogConfig( void )
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	/* Configure GPIOs as Analog input to reduce current consumption*/
	/* Enable GPIOs clock */
#ifdef STM32F072
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE );
#endif

#ifdef STM32F401xx
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOH, ENABLE );
#endif

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);

#ifdef STM32F401xx
	GPIO_Init(GPIOH, &GPIO_InitStructure);
#endif

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Disable GPIOs clock */
#ifdef STM32F072
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, DISABLE );
#endif

#ifdef STM32F401xx
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOH, DISABLE );
#endif
}



/**
  * @brief  This function configures the system to enter Stop mode
  *         for current consumption measurement purpose.
  *         ===========================================
  *           - Regulator in Low Power mode
  *           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
  *           - No IWDG
  *           - FLASH in deep power down mode
  *           - Wakeup using WakeUp Pin (PA.00)
  * @param  None
  * @retval None
  */

/*
void	PWR_StopLowPwrRegFlashPwrDown( void )
{
	// Clear Wakeup flag
	PWR_ClearFlag( PWR_FLAG_WU );

	// Enable the wakeup pin
//	PWR_WakeUpPinCmd( ENABLE );

	// FLASH Deep Power Down Mode enabled
	PWR_FlashPowerDownCmd( ENABLE );

	// Enter Stop Mode Reg LP
	PWR_EnterSTOPMode( PWR_Regulator_LowPower, PWR_STOPEntry_WFI );
}

static	void	Sleep_Mode_Exit ( void )
{
	// Clock init configuration
	RCC_DeInit();

	// Disable HSE
	RCC_HSEConfig(RCC_HSE_OFF);

	// Enable HSI
	RCC_HSICmd(ENABLE);

	// Wait till HSI is ready
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET) {
	}

	// Select HSI as system clock source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

	// Wait till HSI is used as system clock source
	while (RCC_GetSYSCLKSource() != 0x00) {
	}

	// Enable PWR APB1 Clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	// Configure Leds GPIOs
	LedsConfig();

	// Configure Wakeup pin
	WakeUpPinInt_configuration();
}
*/

/***************************************************************************************************
 *  Function Name: SLEEP_SlaveSleep_STOP_Mode
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SLEEP_SlaveSleep_STOP_Mode( uint16_t *SleepTime )
{
	uint32_t	delay;

	Console_Output_String( "\r\nSleep(STOP)\r\n" );
	SX1276LoRaSetOpMode(RFLR_OPMODE_SLEEP);

#ifdef Board__A22_Tracker
	GPS_MT3333Stop();
#endif

	if(MySensor != NULL) memset((void *)MySensor, 0, sizeof(tDeviceNodeSensor));

	Sleep_TimeCount = 0;
	isInSleep = true;
	delay = GET_TICK_COUNT( );
	while( true ) {
		if( CmdUART__is_TX_Queue_Empty() ) {
// 			if( USART_GetITStatus( CmdUART, USART_IT_TXE ) == SET ) break;

			delay = GET_TICK_COUNT( );
			while( ( GET_TICK_COUNT( ) - delay ) < TICK_RATE_MS( 2 ) ) {
			}
			break;
		}

		if( ( GET_TICK_COUNT( ) - delay ) > TICK_RATE_MS( 100 ) ) break;
	}

	while(true) {
		if(Sleep_TimeCount < *SleepTime) {
			/* Enter Stop Mode */
			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
		} else {
			break;
		}
	}

	SLEEP_SYSCLKConfigFromSTOPMode();
	Console_Output_String( "\r\nWake(STOP)\r\n" );

#ifdef Board__A22_Tracker
	if(GPS_HandmadeOff == false) GPS_MT3333Run();
#endif

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	Radio->StartRx();
	isInSleep = false;
}

void	SLEEP_SlaveSleep_Deep_STOP_Mode( uint16_t *SleepTime_sec )
{
	uint32_t	delay;

	Console_Output_String( "\r\nSleep(Deep STOP)\r\n" );
	SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );

#ifdef Board__A22_Tracker
	GPS_MT3333Stop();
#endif

	RTC_AlarmStop();
// 	RTC_TimerConfig();		// Reset RTC Domain to clear sub-second alarm

	RTC_AlarmConfig_Timeout( *SleepTime_sec );
// 	RTC_AlarmRun();

// 	Sleep_TimeCount = 0;
	isInSleep = true;
	delay = GET_TICK_COUNT( );
	while( true ) {
		if( CmdUART__is_TX_Queue_Empty() ) {
// 			if( USART_GetITStatus( CmdUART, USART_IT_TXE ) == SET ) break;

			delay = GET_TICK_COUNT( );
			while( ( GET_TICK_COUNT( ) - delay ) < TICK_RATE_MS( 2 ) ) {
			}
			break;
		}

		if( ( GET_TICK_COUNT( ) - delay ) > TICK_RATE_MS( 100 ) ) break;
	}

	Board_DeInit();
	CmdUART__Close();
	GPIOs__AnalogConfig();

	SX127x_Init_NSS();

	/* Disable SysTick ISR */
// 	SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);

// 	RTC_ITConfig( RTC_IT_ALRA, DISABLE );

	/* Clear Power Wake-up (CWUF) flag */
	PWR_ClearFlag( PWR_FLAG_WU );

	/* Enable RTC Alarm A Interrupt */
	RTC_ITConfig( RTC_IT_ALRA, ENABLE );
	RTC_ClearFlag( RTC_FLAG_ALRAF );

	RTC_AlarmRun();

#ifdef STM32F401xx
	/* FLASH Deep Power Down Mode enabled */
	PWR_FlashPowerDownCmd( ENABLE );
#endif

	PWR_EnterSTOPMode( PWR_Regulator_LowPower, PWR_STOPEntry_WFI );
	// ==============================================================================
	// Sleep
	// ==============================================================================


	// ==============================================================================
	// Wake up
	// ==============================================================================
	SLEEP_SYSCLKConfigFromSTOPMode();

#ifdef Board__A22_Tracker
// 	Led_BootFlashLed();

// 	BlueTooth_DA14580Run(ComPortBaudRate);
// 	if(EnableMaster == false) {				// ┐
// 		CmdUART__Open(ComPortBaudRate);		// ├暫時給 SLAVE 測試用,若不執行此,則 CMD UART 輸出會一直等待完成而使得無限等待
// 		CmdTIMER_TimerConfig();				// ┘
// 	}

// 	if(EnableMaster == false) GPS_MT3333Run();
#else
	CmdUART__Open( ComPortBaudRate );
// 	CmdTIMER_TimerConfig();
#endif
	Console_Output_String( "\r\nWake(STOP)\r\n" );

// 	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	BoardInit();
	Console_Output_String( "Board Initialized.\r\n" );

	Radio->Init();
	Console_Output_String( "Radio Initialized.\r\n" );

#ifdef Board__A22_Tracker
	if(GPS_HandmadeOff == false) GPS_MT3333Run();
#endif

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	Radio->StartRx();
	isInSleep = false;
}


void	SLEEP_SlaveSleep_STANDBY_Mode( uint16_t *SleepTime_sec )
{
	uint32_t	delay;

	Console_Output_String( "\r\nSleep(STANDBY)\r\n" );
	SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );

#ifdef Board__A22_Tracker
	GPS_MT3333Stop();
#endif

	RTC_AlarmStop();
// 	RTC_TimerConfig();		// Reset RTC Domain to clear sub-second alarm

	RTC_AlarmConfig_Timeout( *SleepTime_sec );
// 	RTC_AlarmRun();

// 	Sleep_TimeCount = 0;
	isInSleep = true;
	delay = GET_TICK_COUNT( );
	while( true ) {
		if( CmdUART__is_TX_Queue_Empty() ) {
// 			if( USART_GetITStatus( CmdUART, USART_IT_TXE ) == SET ) break;

			delay = GET_TICK_COUNT( );
			while( ( GET_TICK_COUNT( ) - delay ) < TICK_RATE_MS( 2 ) ) {
			}
			break;
		}

		if( ( GET_TICK_COUNT( ) - delay ) > TICK_RATE_MS( 100 ) ) break;
	}

	/* Disable SysTick ISR */
// 	SysTick->CTRL &= (~SysTick_CTRL_TICKINT_Msk);

// 	RTC_ITConfig( RTC_IT_ALRA, DISABLE );

	/* Clear Power Wake-up (CWUF) flag */
	PWR_ClearFlag( PWR_FLAG_WU );

	/* Enable RTC Alarm A Interrupt */
	RTC_ITConfig( RTC_IT_ALRA, ENABLE );
	RTC_ClearFlag( RTC_FLAG_ALRAF );

	RTC_AlarmRun();

	/* Request to enter STANDBY mode (Wake Up flag is cleared in PWR_EnterSTANDBYMode function) */
	PWR_EnterSTANDBYMode();

	/* Infinite loop */
	while (1) {
	}
}


/***************************************************************************************************
 *  Function Name: SLEEP_SlaveSleepAandRandomHopChannelProcedure
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	SLEEP_SlaveSleepAandRandomHopChannelProcedure(uint16_t *SleepTime)
{
	if( EnableMaster || ! LoraGateWay ) {
		Running_TimeCount = 0;		// 當此裝置不是 SLAVE 或還未加入網域時就不睡
		return;
	}

#ifdef Board__A22_Tracker
	if( Running_TimeCount >= GPSnoLocated_RunningTime && ! Slave_PollEventAccomplish ) {
		if( SLAVE_LoraPollEventInterval >= GPSnoLocated_RunningTime ) {
			// Console_Output_String( "Clear&Default\r\n" );
			SLAVE_LoraPollEventInterval = 0;
			RandomHopStartChannel_SlaveDefaultHoppingChannel();
		}

		if( *SleepTime ) {
			SLEEP_SlaveSleep_Deep_STOP_Mode( SleepTime );
			SLAVE_LoraPollEventInterval = 0;
			RandomHopStartChannel_SlaveDefaultHoppingChannel();
		}
		Running_TimeCount = 0;
	} else {
		if( Slave_PollEventAccomplish ) {
			if( *SleepTime ) {
				SLEEP_SlaveSleep_Deep_STOP_Mode( SleepTime );
				RandomHopStartChannel_SetHoppingStartChannelFreq(SLAVE_LoraHoppingStartChannel);
			}
			Running_TimeCount = 0;
			SLAVE_LoraPollEventInterval = 0;
			Slave_PollEventAccomplish = false;
		} else {
			if( SLAVE_LoraPollEventInterval >= GPSnoLocated_RunningTime ) {
				if( *SleepTime ) SLEEP_SlaveSleep_Deep_STOP_Mode( SleepTime );

				RandomHopStartChannel_SlaveDefaultHoppingChannel();
				SLAVE_LoraPollEventInterval = 0;
				Running_TimeCount = 0;
				Slave_PollEventAccomplish = false;
			}
		}
	}
#else
	if( Slave_PollEventAccomplish ) {
		if( *SleepTime ) {
			SLEEP_SlaveSleep_Deep_STOP_Mode( SleepTime );
			RandomHopStartChannel_SetHoppingStartChannelFreq(SLAVE_LoraHoppingStartChannel);
		}
		Running_TimeCount = 0;
		SLAVE_LoraPollEventInterval = 0;
		Slave_PollEventAccomplish = false;
	} 
#endif
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
void		SLEEP_MasterSleepProcedure(void)
{
	uint8_t		count;
	bool		sleep = false;

	// 表目前有事件正在執行中
	if(LoraRunningEvent.RunNodeEvent != 0) return;

	// 表共同事件排程裡有事件準備執行
	if( Event_Count[0] ) return;
	if( Event_Count[1] ) return;
	if( Event_Count[2] ) return;

	for( count = 0 ; count < MAX_LoraNodeNum ; count++ ) {
		// 此 SLAVE Node 裝置是否存在,不存在就跳到下一值去判斷
		if( ! Device_Information[count].Flag_Valid ) continue;

		// 判斷個別事件排程裡是否有事件準備執行
		if( ! Device_Information[count].Node_Sleep_Hop.isNowSleeping ) {
			if( Device_Information[count].Node_Sleep_Hop.Event_Count[0] ) return;
			if( Device_Information[count].Node_Sleep_Hop.Event_Count[1] ) return;
			if( Device_Information[count].Node_Sleep_Hop.Event_Count[2] ) return;
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
		NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP, DISABLE);
		NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT, DISABLE);
		__WFI();
	}
	/* Enable SysTick ISR */
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

	SLEEP_SYSCLKConfigFromSTOPMode();

	/* Lora sx1276/78 into running mode */
	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	Radio->StartRx();

	/* 進入 SLEEP Mode 必須先將 SYS_TICK 中斷關閉, 不然一睡下去之後,立刻 SYS_TICK 中斷產生進入 執行,如此根本不用睡了,因為 SYS_TICK=1ms */
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
