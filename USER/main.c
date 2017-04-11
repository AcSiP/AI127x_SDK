
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lbtafa.h"

#include "Base_Driver__UART1.h"
#include "UART_Console.h"
#include "Base_Driver__ADC1.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef STM32F072
	#define APPLICATION_ADDRESS		(uint32_t)0x0800C000
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

///////////////// for STM32F0 IAP //////////////////////////////
#ifdef STM32F072
	#if (defined ( __CC_ARM ))
	__IO uint32_t			VectorTable[48] __attribute__((at(0x20000000)));
	#elif (defined (__ICCARM__))
	#pragma location = 0x20000000
	__no_init __IO uint32_t		VectorTable[48];
	#elif defined (  __GNUC__  )
	__IO uint32_t			VectorTable[48] __attribute__((section(".RAMVectorTable")));
	#elif defined ( __TASKING__ )
	__IO uint32_t			VectorTable[48] __at(0x20000000);
	#endif
#endif

///////////////// for System //////////////////////////////
__IO uint32_t			SystemOperMode;
bool				LoraStartWork = 1;		// 1=start or 0=stop LORA
bool				EnableMaster;			// 1=Master or 0=Slave selection
bool				Flag__Init_RTC = false;
uint32_t			Last_Measure_TickCounter = 0;




///////////////// for CmdUART & CLI ///////////////////////
extern __IO bool		LoRaOn;
uint8_t				CLI_Buf[ 128 ];

Byte_Queue_Struct		Console__RX_Queue;


///////////////// for USB Cmd & CLI ///////////////////////
#ifdef STM32F401xx
#ifdef	USBD_VCP_Console
extern __IO bool		UsbDegugOn;
#endif
#endif

///////////////// for Lora Radio //////////////////////////
extern tLoRaSettings		LoRaSettings;
__IO bool			LoraNeedTxData;
__IO size_t			LoraTxPayloadSize;
__IO size_t			LoraRxPayloadSize;
__IO uint8_t			LoraTxBuffer[LoraBufferLength];
__IO uint8_t			LoraRxBuffer[LoraBufferLength];
tRadioDriver *			Radio = NULL;
uint8_t				LoraOperationMode;

///////////////// for Acsip Protocol //////////////////////
__IO tAcsipProtocolFrame	TxFrame;
__IO tAcsipProtocolFrame	RxFrame;
__IO tLoraRunningEvent		LoraRunningEvent;

extern tLoraDeviceNode *		LoraNodeDevice[MAX_LoraNodeNum];			// for MASTER
extern tDeviceNodeSleepAndRandomHop *	DeviceNodeSleepAndRandomHop[MAX_LoraNodeNum];		// for MASTER
extern tDeviceNodeSensor *		DeviceNodeSensor[MAX_LoraNodeNum];			// for MASTER
extern uint8_t				LoraNodeCount;						// for MASTER
extern tLoraDeviceNode *		LoraGateWay;						// for SLAVE
extern tDeviceNodeSensor *		MySensor;						// for SLAVE


///////////////// for Button ///////////////////////////
extern __IO bool		PowerButton;
extern __IO bool		FactoryResetButton;

///////////////// for Sleep & Random Hopping Channel //////
__IO bool			Slave_PollEventAccomplish = false;		// for SLAVE
extern __IO uint16_t		SLAVE_LoraPollEventInterval;			// for SLAVE
extern __IO bool		Slave_PollEvent;				// for SLAVE
extern __IO bool		Slave_PollEvent_UTCnotZero;			// for SLAVE
extern __IO uint8_t		SLAVE_LoraHoppingStartChannel;			// for SLAVE


///////////////// for Product Verification /////////////////
__IO uint32_t		LoraPV_RxCount;
__IO bool		LoraPV_RxTest;
__IO bool		LoraPV_TxTest;
__IO bool		LoraPV_DefaultTxSet;
__IO uint32_t		LoraPV_TxCount;
__IO uint32_t		LoraPV_TxTimes;

///////////////// for Ping-Pong test ///////////////////////
static uint16_t		BufferSize = 9;			// RF buffer size
__IO static uint64_t	RX_COUNT = 0;
__IO static uint64_t	RX_Done_COUNT = 0;
__IO static uint64_t	RX_HasData_COUNT = 0;
static uint8_t		Buffer[9];			// RF buffer
const uint8_t		PingMsg[] = "PING";
const uint8_t		PongMsg[] = "PONG";


/* Private function prototypes -----------------------------------------------*/
#ifdef	USBD_VCP_Console
static void	RX_DataCopy(uint8_t *, const uint8_t *, uint16_t);
#endif

static bool	MasterLoraEvent_PROCESS(void);
static void	OnMasterForNormal(void);
static void	OnSlaveForNormal(void);
static void	ForLoraProductVerification(void);
static void	ForFskProductVerification(void);
static void	OnMasterForPingPongTest(void);
static void	OnSlaveForPingPongTest(void);
static void	RxTestOutput(void);


/* Private functions ---------------------------------------------------------*/
void		Clear_LoRa_TX_Buffer( void )
{
	LoraTxPayloadSize = 0;
	memset( (void *) LoraTxBuffer, 0, LoraBufferLength );
}

void		Clear_LoRa_RX_Buffer( void )
{
	LoraRxPayloadSize = 0;
	memset( (void *) LoraRxBuffer, 0, LoraBufferLength );
}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int	main( void )
{
	/* Add your application code here */
	// static uint8_t	LoraNum = 0;

#ifdef Board__A22_Tracker
	uint32_t		delayTick, totalDelay;
#endif


#ifdef STM32F072
	uint32_t		i;

	/* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/
	/* Copy the vector table from the Flash (mapped at the base of the application load address 0x0800C000) to the base address of the SRAM at 0x20000000. */
	for ( i = 0; i < 48; i++ ) {
		VectorTable[i] = *(__IO uint32_t*)( APPLICATION_ADDRESS + ( i << 2 ) );
	}

	/* Enable the SYSCFG peripheral clock*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE );

	/* Remap SRAM at 0x00000000 */
	SYSCFG_MemoryRemapConfig( SYSCFG_MemoryRemap_SRAM );
#endif


	if( RCC_GetFlagStatus( RCC_FLAG_PINRST ) != RESET ) Flag__Init_RTC = true;		// Pin reset
	if( RCC_GetFlagStatus( RCC_FLAG_PORRST ) != RESET ) Flag__Init_RTC = true;		// POR/PDR reset
// 	if( RCC_GetFlagStatus( RCC_FLAG_SFTRST ) != RESET ) Flag__Init_RTC = true;		// Software reset
	if( RCC_GetFlagStatus( RCC_FLAG_IWDGRST ) != RESET ) Flag__Init_RTC = true;		// Independent Watchdog reset
	if( RCC_GetFlagStatus( RCC_FLAG_WWDGRST ) != RESET ) Flag__Init_RTC = true;		// Window Watchdog reset
	if( RCC_GetFlagStatus( RCC_FLAG_LPWRRST ) != RESET ) Flag__Init_RTC = true;		// Low Power reset

#ifdef STM32F072
	if( RCC_GetFlagStatus( RCC_FLAG_OBLRST ) != RESET ) Flag__Init_RTC = true;		// Option Byte Loader (OBL) reset
// 	if( RCC_GetFlagStatus( RCC_FLAG_V18PWRRSTF ) != RESET ) Flag__Init_RTC = true;		// V1.8 power domain reset
#endif

	SystemOperMode = SystemInNormal;
	// SystemOperMode = SystemInPingPognTest;
	// SystemOperMode = SystemInProductVerification;
	LoraNeedTxData = false;
	Clear_LoRa_TX_Buffer();
	Clear_LoRa_RX_Buffer();

// 	if( Flag__Init_RTC ){
		RTC_TimerConfig();
// 	} else {
// 		RTC_AlarmStop();
// 	}

#ifdef STM32F401xx
#ifdef	USBD_VCP_Console
	if(UsbDegugOn == true) {
		USBD_Init( &USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc,  &USBD_CDC_cb,  &USR_cb );
		TIM9_TimerConfig();
	}
#endif
#endif

	CmdUART__Init();
	enableGlobalInterrupts();

#ifdef Board__A22_Tracker
	Led_BootFlashLed();

	BlueTooth_DA14580Run(ComPortBaudRate);
	if(EnableMaster == false) {				// ┐
		CmdUART__Open(ComPortBaudRate);		// ├暫時給 SLAVE 測試用,若不執行此,則 CMD UART 輸出會一直等待完成而使得無限等待
		CmdTIMER_TimerConfig();				// ┘
	}

	if(EnableMaster == false) GPS_MT3333Run();
#else
	CmdUART__Open( ComPortBaudRate );
	CmdTIMER_TimerConfig();
#endif


	BoardInit();
	Radio = RadioDriverInit();

	LoraPara_LoadAndConfiguration();

	Radio->Init();
	SX1276LoRaSetHopPeriod( Lora_RFHoppingPeriod );
	Radio->StartRx();

	AcsipProtocol_SetMyAddress();
	memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
	memset((void *)&RxFrame, 0, sizeof(tAcsipProtocolFrame));
	LoraLinkListEvent_Initialization();

	SaveRecord_ReadOutLoraGateWayPara();
	SaveRecord_ReadOutLoraNodePara();
	SaveRecord_TraceDataSpaceInitialization();

#ifdef Board__A22_Tracker
	Button_ButtonInitialization();
	Led_PinInitialization();
#endif

	LBT__Initialize( LBT__Option__Japan );

	Console_Output_String( "\r\n" );


	// Dump power ON status register
#ifdef STM32F072
	if( RCC_GetFlagStatus( RCC_FLAG_OBLRST ) != RESET ) Console_Output_String( "RCC_FLAG_OBLRST\r\n" );			// Option Byte Loader (OBL) reset
	if( RCC_GetFlagStatus( RCC_FLAG_V18PWRRSTF ) != RESET ) Console_Output_String( "RCC_FLAG_V18PWRRSTF\r\n" );		// V1.8 power domain reset
#endif
	if( RCC_GetFlagStatus( RCC_FLAG_PINRST ) != RESET ) Console_Output_String( "RCC_FLAG_PINRST\r\n" );			// Pin reset
	if( RCC_GetFlagStatus( RCC_FLAG_PORRST ) != RESET ) Console_Output_String( "RCC_FLAG_PORRST\r\n" );			// POR/PDR reset
	if( RCC_GetFlagStatus( RCC_FLAG_SFTRST ) != RESET ) Console_Output_String( "RCC_FLAG_SFTRST\r\n" );			// Software reset
	if( RCC_GetFlagStatus( RCC_FLAG_IWDGRST ) != RESET ) Console_Output_String( "RCC_FLAG_IWDGRST\r\n" );			// Independent Watchdog reset
	if( RCC_GetFlagStatus( RCC_FLAG_WWDGRST ) != RESET ) Console_Output_String( "RCC_FLAG_WWDGRST\r\n" );			// Window Watchdog reset
	if( RCC_GetFlagStatus( RCC_FLAG_LPWRRST ) != RESET ) Console_Output_String( "RCC_FLAG_LPWRRST\r\n" );			// Low Power reset
	RCC_ClearFlag();

	RTC_AlarmConfig();
	RTC_AlarmRun();

	Console_Output_String( "FirmwareVersion=" );
	Console_Output_String( FirmwareVersion );
	Console_Output_String( " for " );
	Console_Output_String( Target_Lora_Device );
	Console_Output_String( "\r\n" );
	Console_Output_String( "LORA module start RUNNING...\r\n" );

	SLEEP_SlaveSleepVariableSet();
	RandomHopStartChannel_SlaveDefaultHoppingChannel();

	// SLEEP_SlaveSleepAandRandomHopChannelProcedure(60);		// for sleep test
	// Console_Output_String( "wake up...\r\n" );			// for sleep test

	while(1) {
		// CmdUART__UartGet_String( uint8_t *buffer, uint16_t buf_sz, uint16_t *ret_length );
		if( CmdUART__UartGet_String( CLI_Buf, sizeof( CLI_Buf ), NULL ) ) {
			goto CLI_PROCESS;		// CLI_Buf處理
		}

#ifdef STM32F401xx
#ifdef	USBD_VCP_Console
		if(UsbDegugOn == true) {
			if( isVCP_RX_Buf1Full ) {
				memset( (void *) CLI_Buf, 0, VCP_RX_BufLength );
				RX_DataCopy( CLI_Buf, (const uint8_t *) VCP_RX_Buf1, VCP_RX_Length );
				memset( (void *) VCP_RX_Buf1, 0, VCP_RX_BufLength );
				isVCP_RX_Buf1Full = false;
				goto CLI_PROCESS;		// CLI_Buf處理
			} else {
				if( isVCP_RX_Buf2Full ) {
					memset( (void *) CLI_Buf, 0, VCP_RX_BufLength );
					RX_DataCopy( CLI_Buf, (const uint8_t *) VCP_RX_Buf2, VCP_RX_Length );
					memset( (void *) VCP_RX_Buf2, 0, VCP_RX_Length );
					isVCP_RX_Buf2Full = false;
					goto CLI_PROCESS;		// CLI_Buf處理
				}
			}
		}
#endif
#endif


#ifdef Board__A22_Tracker
		if( PowerButton ) {
			Button_PowerButtonAction();
			PowerButton = false;
		}

		if( FactoryResetButton ) {
			totalDelay = 0;
			while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0) {					// PB4=0, to mean keep press button
				delayTick = GET_TICK_COUNT();							// every 20ms, scan button
				while( ( GET_TICK_COUNT( ) - delayTick ) < TICK_RATE_MS( 20 ) ) {		// if button still pressed, then total++
				}
				totalDelay++;									// button release, then go out while loop
			}

			if(totalDelay >= 250) {				// if keep press button > 5s (250 * 20ms = 5000ms)
				CLI_FactoryReset();			// then Factory Reset
				Console_Output_String( "Factory Reset OK!\r\n" );
			}
			FactoryResetButton = false;
		}
#endif


		switch( SystemOperMode ) {
			case SystemInNormal:
				if( ! LoraStartWork ) break;

				if( EnableMaster ) {
					OnMasterForNormal();
					SLEEP_MasterSleepProcedure();
				} else {
					if( Last_Measure_TickCounter < TickCounter ) {
						Last_Measure_TickCounter = TickCounter + 3000;

						// Show ADC values
// 						ADC1__Dump_Values();

						if(MySensor != NULL) {
							MySensor->GPS_Latitude =	ADC1__Get_Converted_Value( ADC_IDX___ADC0 );
							MySensor->GPS_Longitude =	ADC1__Get_Converted_Value( ADC_IDX___ADC1 );
							MySensor->UTC =			ADC1__Get_Converted_Value( ADC_IDX___ADC4 );
							MySensor->Battery =		ADC1__Get_Converted_Value( ADC_IDX___VBat );
						}
					}

#ifdef Board__A22_Tracker
					GPS_ReadIn(MySensor);
#endif

					OnSlaveForNormal();
					if( LoraGateWay ) SLEEP_SlaveSleepAandRandomHopChannelProcedure( & LoraGateWay->Interval );
				}
				break;

			case SystemInProductVerification:
				if( ! LoraStartWork ) break;


				if(LoRaOn == true) {
					ForLoraProductVerification();
				} else {
					ForFskProductVerification();
				}
				break;

			case SystemInPingPognTest:
				if( ! LoraStartWork ) break;

				if( ( EnableMaster ) ) {
					OnMasterForPingPongTest();
				} else {
					OnSlaveForPingPongTest();
				}
				break;

			case SystemInIdle:
				break;

			default:
				break;
		}

		while(0) {
CLI_PROCESS:		// (可考慮把此部分寫成一個函式)

			CLI_process( (char *)CLI_Buf );
			// 藉由判斷ret來了解接收到的資料狀態,非指令(如ret回傳狀態=SHELL_PROCESS_ERR_CMD_UNKN)則當作資料傳輸出去(也要判斷lora是否有連上線)
			// 目前構思每一個cli指令執行後都會回傳一個狀態(ok、err)或結果
			memset((void *)CLI_Buf, 0, sizeof( CLI_Buf ) );
		}
	}
}


/***************************************************************************************************
 *  Function Name: LoraPara_LoadAndConfiguration
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	LoraPara_LoadAndConfiguration(void)
{
	EnableMaster = 0;				// 1=Master or 0=Slave selection
	LoRaSettings.Power = 20;
	LoRaSettings.SignalBw = 7;			// LORA [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
							// 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]
	LoRaSettings.SpreadingFactor = 10;		// LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096 chips]
	LoRaSettings.FreqHopOn = 0;			// [0: OFF, 1: ON]
	LoRaSettings.PayloadLength = MaxPacketSize;
	LoRaSettings.MaxPayloadLength = MaxPacketSize;
	LoRaSettings.PreambleLength = 12;

	LoRaSettings.RFFrequency = Lora_RFFrequency;
	LoRaSettings.HopPeriod = Lora_RFHoppingPeriod;		// Hops every frequency hopping period symbols
	LoRaSettings.ErrorCoding = 1;				// LORA [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
	LoRaSettings.CrcOn = true;				// [0: OFF, 1: ON]
	LoRaSettings.ImplicitHeaderOn = 0;			// [0: OFF, 1: ON]
	LoRaSettings.RxSingleOn = 1;				// [0: Continuous, 1 Single]
	LoRaSettings.LowDataRateOptimize = 1;			// [0: OFF, 1: ON]
	LoRaSettings.TxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.RxPacketTimeout = LoRaSettings.TxPacketTimeout;
	// LoRaSettings.Oscillator = OscillatorTCXO;
	LoRaSettings.Oscillator = OscillatorCrystal;
	SaveRecord_ReadOutMyselfPara();
}


#ifdef	USBD_VCP_Console
void	RX_DataCopy( uint8_t *dst, const uint8_t *sou, uint16_t length)
{
	uint16_t	count;

	for( count = 0 ; count < length ; count++ ) dst[count] = sou[count];
}
#endif

void	Main__Configure_RX( void )
{
	if( LoRaSettings.FreqHopOn || LBT__Option_Mode == LBT__Option__None ) {
		Radio->StartRx();
	} else {
		LBT__Configure_Receving();
	}
}

/***************************************************************************************************
 *  Function Name: MasterLoraEvent_PROCESS
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
static	bool	MasterLoraEvent_PROCESS( void )
{
	static uint8_t	tx_size;

	/* 目前寫到最後的想法是將 NodeCount 搬到上一層去做++與range判斷
	因為若是在這裡做這件事,在後面進到NormalMaster函式裡後,帶入的值
	會再受 join、data、leave等影響此值,用此外面加值方式,可能可以把LoraEvent拿掉*/

	/* 進到switch( Radio->Process( ) )後,須注意一件事,就是會有其他狀態,如busy,如此很在此NodeCount下可能就跳過
	必須等待rx_timeout或rx_done等的狀況,所以須如實等待,也需防止無限的busy等當機狀態,(注意slave是否也須注意此情形)  */

	if( ! LoraLinkListEvent_DispatcherLoraEvent() ) {
		// Console_Output_String( "DispatcherLoraEvent=false\r\n" );			// test output
		// snprintf( (char *)str, sizeof(str), "%02u", LoraEvent.NodeEvent );		// test output
		// Console_Output_String( "LoraEvent=" );					// test output
		// Console_Output_String( (const char *)str );					// test output
		// Console_Output_String( "\r\n" );						// test output
		// snprintf( (char *)str, sizeof(str), "%02d", EventCountPriority2 );		// test output
		// Console_Output_String( "EventCountPriority2=" );				// test output
		// Console_Output_String( (const char *)str );					// test output
		// Console_Output_String( "\r\n" );						// test output

		if( LoraRunningEvent.RunNodeEvent == Master_AcsipProtocol_Poll ) {
			// Console_Output_String( "LoraEvent=Poll\r\n" );		// test output
			if( Event_Count[2] > 0 ) Event_Count[2]--;
		}
		// Console_Output_String( "MemsetLoraEvent\r\n" );			// test output
		memset( (void *)&LoraRunningEvent, 0, sizeof(tLoraRunningEvent) );
		return( false );
	}

	// Console_Output_String( "DispatcherLoraEvent=true\r\n" );		// test output
	switch( LoraRunningEvent.RunNodeEvent ) {
	case Master_AcsipProtocol_Broadcast:
		// 增加廣播請求函式
		// LoraTxPayloadSize = tx_size;
		return( false );

	case Master_AcsipProtocol_Join:
		AcsipProtocol_LoraJoinNetworkRequest( LoraRunningEvent.RunNodeAddr, &TxFrame, LoraTxBuffer, &tx_size );
		LoraTxPayloadSize = tx_size;
		break;

	case Master_AcsipProtocol_Poll:
		AcsipProtocol_LoraPollRequest( LoraNodeDevice[LoraRunningEvent.RunNodeNumber], &TxFrame, LoraTxBuffer, &tx_size );
		LoraTxPayloadSize = tx_size;
		// LoraPollEventCount--;
		break;

	case Master_AcsipProtocol_Data:
		AcsipProtocol_LoraDataRequest( LoraNodeDevice[LoraRunningEvent.RunNodeNumber], LoraRunningEvent.RunNodeData, &LoraRunningEvent.RunNodeDataSize, &TxFrame, LoraTxBuffer, &tx_size );
		LoraTxPayloadSize = tx_size;
		break;

	case Master_AcsipProtocol_Leave:
		AcsipProtocol_LoraLeaveNetworkRequest( LoraNodeDevice[LoraRunningEvent.RunNodeNumber], &TxFrame, LoraTxBuffer, &tx_size );
		LoraTxPayloadSize = tx_size;
		break;

	case Master_AcsipProtocol_Interval:
		AcsipProtocol_LoraIntervalRequest( LoraNodeDevice[LoraRunningEvent.RunNodeNumber], &TxFrame, LoraTxBuffer, &tx_size );
		LoraTxPayloadSize = tx_size;
		// Console_Output_String( "LoraEvent_PROCESS_Interval\r\n" );		// test output
		break;

	default:
		LoraTxPayloadSize = 0;
	}

	if( ! LoraTxPayloadSize ) return( false );

	switch( LoraRunningEvent.RunNodeEvent ) {
	case Master_AcsipProtocol_Broadcast:
	case Master_AcsipProtocol_Join:
		RandomHopStartChannel_SetHoppingStartChannelFreq(0);		// 若有跳頻,廣播或 LoraJoinEvent 從跳頻隨機起始通道 0 開始
		break;

	case Master_AcsipProtocol_Poll:
	case Master_AcsipProtocol_Data:
	case Master_AcsipProtocol_Leave:
	case Master_AcsipProtocol_Interval:
		if( DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber] ) {
			RandomHopStartChannel_SetHoppingStartChannelFreq( DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->LoraHoppingStartChannel );
		}
		break;

	default:
		break;
	}


	if( LoRaSettings.FreqHopOn || LBT__Option_Mode == LBT__Option__None ) {
		Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
		return( true );
	}

	switch( LBT__Option_Mode ) {
	case LBT__Option__Japan:
		if( LBT__Wait_RF_Channel_Clear( LoRaSettings.TxPacketTimeout ) ){
			Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
			return( true );
		}
		Console_Output_String( "Channel is not free for JP-LBT.\r\n" );
		break;

	case LBT__Option__Europe_w_AFA:
		if ( LBTandAFA_TX() ) {
			Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
			return( true );
		}
		Console_Output_String( "Channel is not free for CE-LBT.\r\n" );
		break;

	default:
		Console_Output_String( "CE-LBT or JP-LBT have problem.\r\n" );
	}

	Clear_LoRa_TX_Buffer();
	memset( (void *) & TxFrame, 0, sizeof(tAcsipProtocolFrame) );
	return( false );
}



/***************************************************************************************************
 *  Function Name: OnMasterForNormal
 *
 *  Description: Manages the Master normal operation
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
static void	OnMasterForNormal( void )
{
	int8_t		count, str[10];

	switch( Radio->Process() ) {
	case RF_CHANNEL_EMPTY:
//		Console_Output_String( "EMPTY\r\n" );
		// CLI_LoraGetRFFrequency( );
		if( LBTandAFA_RX() ){
			// Keeping CAD
			break;
		}

		// Try CAD too many times, Keep go to process by  RF_RX_TIMEOUT


	case RF_RX_TIMEOUT:
		// Console_Output_String( "RX_TIMEOUT\r\n" );		//test output

		if( (LoraRunningEvent.RunNodeEvent == Master_AcsipProtocol_Poll) && (LoraNodeDevice[LoraRunningEvent.RunNodeNumber] ) ) {
			Console_Output_String( "Node=" );
			for(count = 2 ; count >= 0 ; count--) {
				snprintf( (char *)str, sizeof(str), "%02x", LoraRunningEvent.RunNodeAddr[count]);
				Console_Output_String( (const char *)str );
			}

#ifdef Board__A22_Tracker
			Console_Output_String( " EVT=GPS 0 0 0 0 0.0\r\n" );
#else
			Console_Output_String( " EVT=Poll 0.0\r\n" );
#endif
		}		// 用來通知上層(如藍芽、APP),SLAVE Node 沒有回應, 此時 SLAVE Node 可能睡覺或已離線

		if( (LoraRunningEvent.RunNodeEvent != Master_AcsipProtocol_Broadcast) && (LoraRunningEvent.RunNodeEvent != Master_AcsipProtocol_Join) && (LoraRunningEvent.RunNodeEvent != 0) ) {
			if( DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber] ) {
				DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->LoraRxFailureTimes += 1;
				if( ! DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->isLoraDisconnecting ) {
					if( DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->LoraRxFailureTimes > ( LoraReceptionFailureTimes + DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->DefineLoraRxFailureTimes ) ) {
						Console_Output_String( "Node=" );
						for(count = 2 ; count >= 0 ; count--) {
							snprintf( (char *)str, sizeof(str), "%02x", LoraRunningEvent.RunNodeAddr[count] );
							Console_Output_String( (const char *)str );
						}
						Console_Output_String( " EVT=Disconnecting\r\n" );
						DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->isLoraDisconnecting = true;
						DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->LoraHoppingStartChannel = 0;
					}
				}		// 用來通知上層(如藍芽、APP),SLAVE 的 Lora 是斷線狀態
			}		// 當沒有接收到 SLAVE Node 傳過來的訊息時,連續累積到一定次數後,只輸出一次裝置為斷線狀態,並把狀態標註起來
		}		// ,更將針對此 Node 的跳頻隨機起始通道設定為 0。

		if( (LoraRunningEvent.RunNodeEvent == Master_AcsipProtocol_Interval) && LoraNodeDevice[LoraRunningEvent.RunNodeNumber] ){
			LoraNodeDevice[LoraRunningEvent.RunNodeNumber]->Interval = 0;
		}
		// 當 LoraEvent 為 Master_AcsipProtocol_Interval 時,表針對此 SLAVE Node 須設定其 Interval,
		// 所以當 SLAVE Node 在此 LoraEvent 無回應時,就針對此 SLAVE Node 的 Interval 清除為 0。

		if( ! MasterLoraEvent_PROCESS() ) Main__Configure_RX();
		break;

	case RF_RX_DONE:
		// Console_Output_String( "RX_Done\r\n" );		// test output
		Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );

		// LoraRxPayloadSize = LoraRxBuffer[0] + 8;
		LoraRxPayloadSize = LoraRxBuffer[0] + 9;
		if( AcsipProtocol_PacketToFrameProcess(LoraRxBuffer, (uint8_t)LoraRxPayloadSize, &RxFrame) ) {
			if((LoraRunningEvent.RunNodeEvent != 0)) {
				NormalMaster(&LoraRunningEvent);
				// 上面的或下面有額外狀況需處理
				/*
				if(NormalMaster(&LoraEvent) == AcsipProtocol_OK) {
				} //else { }  //格式錯誤是否重發或等待
				*/
			}
		}
		// else { }		// 不是對方傳過來的,是否重新等待

		memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
		memset((void *)&RxFrame, 0, sizeof(tAcsipProtocolFrame));
		Clear_LoRa_TX_Buffer();
		Clear_LoRa_RX_Buffer();

		if((LoraRunningEvent.RunNodeEvent != Master_AcsipProtocol_Broadcast) && (LoraRunningEvent.RunNodeEvent != Master_AcsipProtocol_Join) && (LoraRunningEvent.RunNodeEvent != 0)) {
			if(DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber] != NULL) {
				DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->isLoraDisconnecting = false;
				DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->LoraRxFailureTimes = 0;
			}
		}		// 當有成功接收到相關訊息,則持續將 SLAVE Node 判定為連線狀態

		if ( ! MasterLoraEvent_PROCESS() ) Main__Configure_RX();
		break;

	case RF_TX_DONE:
		// Console_Output_String( "TX_Done\r\n" );		// test output
		if( TxFrame.FrameFlag == FrameFlag_Broadcast ) {
			memset((void *)LoraTxBuffer, 0, LoraBufferLength);
			LoraTxPayloadSize = 0;
		}
		Main__Configure_RX();
		break;

	case RF_TX_TIMEOUT:
		// Console_Output_String( "TX_TIMEOUT\r\n" );		// test output
		memset((void *)LoraTxBuffer, 0, LoraBufferLength);
		LoraTxPayloadSize = 0;
		memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));

		if ( ! MasterLoraEvent_PROCESS() ) Main__Configure_RX();
		break;

	case RF_CHANNEL_ACTIVITY_DETECTED:
//		Console_Output_String( "DETECTED\r\n" );		// test output
		// CLI_LoraGetRFFrequency( );				// test output
		Radio->StartRx( );
		break;

	default:
		// Console_Output_String( "default\r\n" );		// test output
		break;
	}
}


/***************************************************************************************************
 *  Function Name: OnSlaveForNormal
 *
 *  Description: Manages the slave normal operation
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
static void	OnSlaveForNormal( void )
{
	static uint8_t	base64_data[((MaxMsgDataSize/3)*4)];
	size_t		dsize;
	int8_t		count;
	int8_t		str[10];

	switch( Radio->Process( ) ) {
	case RF_RX_TIMEOUT:
		// Console_Output_String( "RX_TIMEOUT\r\n" );		// test output
		Main__Configure_RX();
		break;

	case RF_RX_DONE:
		// Console_Output_String( "RX_Done\r\n" );		// test output
		Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );

		// LoraRxPayloadSize = LoraRxBuffer[0] + 8;
		LoraRxPayloadSize = LoraRxBuffer[0] + 9;
		if( ! AcsipProtocol_PacketToFrameProcess( LoraRxBuffer, (uint8_t)LoraRxPayloadSize, &RxFrame ) ) break;

		// Console_Output_String( "error5\r\n" );		// test output
		if( NormalSlave() != AcsipProtocol_OK ) break;

		// Console_Output_String( "error7\r\n" );		// test output
// 		for(i = LoraTxPayloadSize ; i < MaxPacketSize ; i++) LoraTxBuffer[i] = i;
// 		LoraTxPayloadSize = MaxPacketSize;

		if( LoRaSettings.FreqHopOn || LBT__Option_Mode == LBT__Option__None ) {
			Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
		} else {
			switch( LBT__Option_Mode ) {
			case LBT__Option__Japan:
				if( LBT__Wait_RF_Channel_Clear( LoRaSettings.TxPacketTimeout ) ){
					Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
				} else {
					Console_Output_String( "Channel is not clear for JP-LBT.\r\n" );

					Clear_LoRa_TX_Buffer();
					Clear_LoRa_RX_Buffer();
					memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
					memset((void *)&RxFrame, 0, sizeof(tAcsipProtocolFrame));
					LBT__Configure_Receving();
				}
				break;

			case LBT__Option__Europe_w_AFA:
				if ( LBTandAFA_TX() ) {
					Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
				} else {
					Console_Output_String( "Channel is not clear for CE-LBT.\r\n" );

					Clear_LoRa_TX_Buffer();
					Clear_LoRa_RX_Buffer();
					memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
					memset((void *)&RxFrame, 0, sizeof(tAcsipProtocolFrame));
					LBT__Configure_Receving();
				}
				break;

			default:
				Console_Output_String( "CE-LBT or JP-LBT have problem.\r\n" );		// output

				Clear_LoRa_TX_Buffer();
				Clear_LoRa_RX_Buffer();
				memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
				memset((void *)&RxFrame, 0, sizeof(tAcsipProtocolFrame));
				LBT__Configure_Receving();
			}
		}

		if( ( RxFrame.FrameFlag == FrameFlag_Data ) && ( RxFrame.MsgLength > 0 ) ) {
			memset((void *)base64_data, 0, ((MaxMsgDataSize/3)*4));
			if( Base64_encode( base64_data, ((MaxMsgDataSize/3)*4), &dsize, (const uint8_t	*)RxFrame.MsgData, RxFrame.MsgLength ) == 0 ) {
				Console_Output_String( "LoraGateWayAddr=" );
				for( count = 2 ; count >= 0 ; count-- ) {
					snprintf( (char *)str, sizeof(str), "%02x", LoraGateWay->NodeAddress[count] );
					Console_Output_String( (const char *)str );
				}

				Console_Output_String( "  DataLength=" );
				snprintf( (char *)str, sizeof(str), "%u", dsize );
				Console_Output_String( (const char *)str );
				Console_Output_String( "  Data=" );
				Console_Write( base64_data, dsize );
				Console_Output_String( "\r\n" );

				// Console_Output_String( "  " );
				// Console_Output_String( "DataLength=" );
				// snprintf( (char *)str, sizeof(str), "%u", RxFrame.MsgLength );
				// Console_Output_String( (const char *)str );
				// Console_Output_String( "  " );
				// Console_Output_String( "Data=" );
				// Console_Write( (uint8_t *)RxFrame.MsgData, RxFrame.MsgLength );
				// Console_Output_String( "\r\n" );
				// 或是存下來或是透過藍芽傳出去
			}
		}

		if((RxFrame.FrameFlag == FrameFlag_Join) || (RxFrame.FrameFlag == FrameFlag_Leave) || (RxFrame.FrameFlag == FrameFlag_Interval)) {
#ifdef STM32F401xx
			SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara();
#endif

#ifdef STM32F072
			SaveRecord_WriteInMyselfParaAndLoraGateWayPara();
#endif
			SaveRecord_WriteInLoraMode();
		}
		break;

	case RF_TX_DONE:
		// Console_Output_String( "TX_Done\r\n" );		// test output
		if(Slave_PollEvent == true) {
			Slave_PollEvent = false;
			SLAVE_LoraPollEventInterval = 0;
			// Console_Output_String( "TX_Clear\r\n" );		// test output
			if(Slave_PollEvent_UTCnotZero == true) {
				// Slave_PollEvent = false;
				Slave_PollEvent_UTCnotZero = false;
				Slave_PollEventAccomplish = true;
			} else {
				Slave_PollEventAccomplish = false;
			}
		}		// 廣播不設定,訊框為空不設定

		if((RxFrame.FrameFlag != FrameFlag_Broadcast) && (RxFrame.FrameCRC != 0) && (TxFrame.FrameCRC != 0)) {
			if((RxFrame.FrameFlag == FrameFlag_Leave) && (TxFrame.FrameFlag == FrameFlag_LeaveResponse)) {
				RandomHopStartChannel_SlaveDefaultHoppingChannel();		// SLAVE Node 離開網域後就設定回預設的起始通道 0,等待連線
			} else {
				SLAVE_LoraHoppingStartChannel = RxFrame.LoraRF_NextChannel;
				RandomHopStartChannel_SetHoppingStartChannelFreq(SLAVE_LoraHoppingStartChannel);
			}
		}
		memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
		memset((void *)&RxFrame, 0, sizeof(tAcsipProtocolFrame));
		Clear_LoRa_TX_Buffer();
		Clear_LoRa_RX_Buffer();

		Main__Configure_RX();
		break;

	case RF_TX_TIMEOUT:
		// Console_Output_String( "TX_TIMEOUT\r\n" );		// test output
		if(Slave_PollEvent == true) Slave_PollEvent = false;
		memset((void *)LoraTxBuffer, 0, LoraBufferLength);
		LoraTxPayloadSize = 0;

		Main__Configure_RX();
		break;

	case RF_CHANNEL_EMPTY:
		// Console_Output_String( "EMPTY\r\n" );		// test output
		// CLI_LoraGetRFFrequency( );				// test output
		if( ! LBTandAFA_RX() ) Main__Configure_RX();
		break;

	case RF_CHANNEL_ACTIVITY_DETECTED:
		// Console_Output_String( "DETECTED\r\n" );		// test output
		// CLI_LoraGetRFFrequency( );				// test output
		Radio->StartRx();
		break;

	default:
		break;
	}
}


/***************************************************************************************************
 *  Function Name: ForLoraProductVerification
 *
 *  Description: Manages the product verification operation
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	ForLoraProductVerification( void )
{
	switch( Radio->Process( ) ) {
	case RF_RX_DONE:
		if( LoraPV_RxTest == true ) {
			Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );		// 取值
			LoraPV_RxCount++;
			Radio->StartRx( );
		} else {
			if( LoraPV_TxTest == true ) {
				Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );		// 取值
				memset((void *)LoraRxBuffer, 0, LoraBufferLength);		// 清空buf
				// 若有ACK需求就需比對ACK,目前暫定在生產驗證上不須回傳ACK
				if( LoraPV_DefaultTxSet == true ) {
					Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );		// Lora傳值出去
					LoraPV_TxCount++;
				} else {
					if( LoraPV_TxCount < LoraPV_TxTimes ) {
						Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );		// Lora傳值出去
						LoraPV_TxCount++;
					} else {
						CLI_LoraPV_TXresult();
					}
				}
			} else {
				Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );		// 取值
				memset((void *)LoraRxBuffer, 0, LoraBufferLength);		// 清空buf
				Radio->StartRx( );
			}
		}
		break;

	case RF_IDLE:
	case RF_TX_DONE:
	case RF_RX_TIMEOUT:
	case RF_TX_TIMEOUT:
		if( LoraPV_RxTest == true ) {
			Radio->StartRx();
		} else {
			if( LoraPV_TxTest == true ) {
				if( LoraPV_DefaultTxSet == true ) {
					Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );		// Lora傳值出去
					LoraPV_TxCount++;
				} else {
					if(LoraPV_TxCount < LoraPV_TxTimes) {
						Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );		// Lora傳值出去
						LoraPV_TxCount++;
					} else {
						CLI_LoraPV_TXresult();
					}
				}
			} else {
				Radio->StartRx( );
			}
		}
		break;

	default:
		break;
	}
}



/***************************************************************************************************
 *  Function Name: ForFskProductVerification
 *
 *  Description: Manages the product verification operation
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	ForFskProductVerification( void )
{
	switch( Radio->Process() ) {
	case RF_RX_DONE:
		Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );		// 取值
		memset((void *)LoraRxBuffer, 0, LoraBufferLength);		// 清空buf
		Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );		// Lora傳值出去
		break;

	case RF_IDLE:
	case RF_TX_DONE:
	case RF_RX_TIMEOUT:
	case RF_TX_TIMEOUT:
	case RF_LEN_ERROR:
	case RF_CHANNEL_EMPTY:
	case RF_CHANNEL_ACTIVITY_DETECTED:
	case RF_BUSY:
		Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );		// Lora傳值出去
		break;

	default:
		break;
	}
}


/***************************************************************************************************
 *  Function Name: OnMasterForPingPongTest
 *
 *  Description: Manages the master Ping-Pong test operation
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	OnMasterForPingPongTest( void )
{
	uint8_t		i;

	switch( Radio->Process() ) {
	case RF_RX_TIMEOUT:
		Console_Output_String( "RF_RX_TIMEOUT\r\n" );

		// Send the next PING frame
		// Buffer[0] = 'P';
		// Buffer[1] = 'I';
		// Buffer[2] = 'N';
		// Buffer[3] = 'G';
		// for( i = 4; i < BufferSize; i++ ) Buffer[i] = i - 4;
		// Radio->SetTxPacket( Buffer, BufferSize );
		Radio->StartCAD();
		break;

	case RF_RX_DONE:
		Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
		RX_Done_COUNT++;
		if( BufferSize > 0 ) {
			RX_HasData_COUNT++;
			if( strncmp( ( const char* )Buffer, ( const char* )PongMsg, 4 ) == 0 ) {
				RX_COUNT++;
				RxTestOutput();

				// Send the next PING frame
				// Buffer[0] = 'P';
				// Buffer[1] = 'I';
				// Buffer[2] = 'N';
				// Buffer[3] = 'G';
				// We fill the buffer with numbers for the payload
				// for( i = 4; i < BufferSize; i++ ) Buffer[i] = i - 4;
				// Radio->SetTxPacket( Buffer, BufferSize );
				Radio->StartCAD();
			} else {
				if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 ) {
					// A master already exists then become a slave
					EnableMaster = false;
				}
				Radio->StartCAD();
			}
		}
		break;

	case RF_TX_DONE:
		Radio->StartRx( );
		break;

	case RF_TX_TIMEOUT:
		Console_Output_String( "RF_TX_TIMEOUT\r\n" );
		Radio->StartRx( );
		break;

	case RF_CHANNEL_EMPTY:
		// Console_Output_String( "RF_CHANNEL_EMPTY\r\n" );
		// Send the next PING frame
		Buffer[0] = 'P';
		Buffer[1] = 'I';
		Buffer[2] = 'N';
		Buffer[3] = 'G';
		for( i = 4; i < BufferSize; i++ ) Buffer[i] = i - 4;
		Radio->SetTxPacket( Buffer, BufferSize );
		break;

	case RF_CHANNEL_ACTIVITY_DETECTED:
		Console_Output_String( "RF_CHANNEL_ACTIVITY_DETECTED\r\n" );
		Radio->StartCAD();
		break;

	default:
		break;
	}
}


/***************************************************************************************************
 *  Function Name: OnSlaveForPingPongTest
 *
 *  Description: Manages the slave Ping-Pong test operation
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	OnSlaveForPingPongTest( void )
{
	uint8_t		i;

	switch( Radio->Process() ) {
	case RF_RX_DONE:
		Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
		RX_Done_COUNT++;
		if( BufferSize > 0 ) {
			RX_HasData_COUNT++;
			if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 ) {
				RX_COUNT++;
				RxTestOutput();

				// Send the reply to the PONG string
				// Buffer[0] = 'P';
				// Buffer[1] = 'O';
				// Buffer[2] = 'N';
				// Buffer[3] = 'G';
				// We fill the buffer with numbers for the payload
				// for( i = 4; i < BufferSize; i++ ) {
				// 	Buffer[i] = i - 4;
				// }
				// Radio->SetTxPacket( Buffer, BufferSize );
			}
			Radio->StartCAD();
		}
		break;

	case RF_TX_DONE:
		Radio->StartRx( );
		break;

	case RF_TX_TIMEOUT:
		Console_Output_String( "RF_TX_TIMEOUT\r\n" );
		Radio->StartRx( );
		break;

	case RF_RX_TIMEOUT:
		Console_Output_String( "RF_RX_TIMEOUT\r\n" );
		Radio->StartRx( );
		break;

	case RF_CHANNEL_EMPTY:
		// Console_Output_String( "RF_CHANNEL_EMPTY\r\n" );
		// Send the next PING frame
		Buffer[0] = 'P';
		Buffer[1] = 'O';
		Buffer[2] = 'N';
		Buffer[3] = 'G';
		for( i = 4; i < BufferSize; i++ ) Buffer[i] = i - 4;
		Radio->SetTxPacket( Buffer, BufferSize );
		break;

	case RF_CHANNEL_ACTIVITY_DETECTED:
		Console_Output_String( "RF_CHANNEL_ACTIVITY_DETECTED\r\n" );
		Radio->StartCAD();
		break;

	default:
		break;
	}
}


/***************************************************************************************************
 *  Function Name: RxTestOutput
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	RxTestOutput( void )
{
	double		rssi;
	int8_t		snr;
	int8_t		str[8];

	Console_Output_String( "Rx=" );
	Console_Write( (uint8_t *)Buffer, 4 );
	Console_Output_String( "    " );

	snprintf( (char *)str, sizeof(str), "%d", (int)RX_HasData_COUNT );
	Console_Output_String( "RxCount=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "    " );

	snprintf( (char *)str, sizeof(str), "%d", (int)RX_COUNT );
	Console_Output_String( "RxCompleteCount=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "    " );

	rssi = SX1276LoRaGetPacketRssi();
	snprintf( (char *)str, sizeof(str), "%3.2f", rssi );
	Console_Output_String( "RSSI=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "    " );

	snr = SX1276LoRaGetPacketSnr();
	snprintf( (char *)str, sizeof(str), "%d", snr );
	Console_Output_String( "SNR=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void	assert_failed( uint8_t* file, uint32_t line )
{
	/* User can add his own implementation to report the file name and line number,
	ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1) {
	}
}
#endif

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
