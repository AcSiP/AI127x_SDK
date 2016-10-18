/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  
  * @version 
  * @date    
  * @brief   Main program body
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/** 
  * 
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
///////////////// for System //////////////////////////////
__IO uint32_t SystemOperMode;
bool LoraStartWork = 1;       // 1=start or 0=stop LORA
bool EnableMaster;        // 1=Master or 0=Slave selection
///////////////// for USART2 CmdUART & CLI ///////////////////////
extern __IO uint16_t CmdUART_RX_Length;
extern __IO uint8_t RX_Buf1[CmdUART_RX_BufLength];
extern __IO uint8_t RX_Buf2[CmdUART_RX_BufLength];
extern __IO bool isRX_Buf1Full;
extern __IO bool isRX_Buf2Full;
extern __IO bool LoRaOn;
uint8_t CLI_Buf[CmdUART_RX_BufLength];
///////////////// for USB Cmd & CLI ///////////////////////
#ifdef STM32F401xx
#ifdef	USBD_VCP_Console
extern __IO bool UsbDegugOn;
#endif
#endif
///////////////// for Lora Radio //////////////////////////
extern tLoRaSettings LoRaSettings;
__IO bool LoraNeedTxData;
__IO size_t LoraTxPayloadSize;
__IO size_t LoraRxPayloadSize;
__IO uint8_t LoraTxBuffer[LoraBufferLength];
__IO uint8_t LoraRxBuffer[LoraBufferLength];
tRadioDriver *Radio = NULL;
uint8_t LoraOperationMode;
///////////////// for Acsip Protocol //////////////////////
__IO tAcsipProtocolFrame TxFrame;
__IO tAcsipProtocolFrame RxFrame;
__IO tLoraRunningEvent LoraRunningEvent;
//static uint8_t LoraPollEventCount;
extern tLoraDeviceNode *LoraNodeDevice[MAX_LoraNodeNum];      //for MASTER
extern tDeviceNodeSleepAndRandomHop *DeviceNodeSleepAndRandomHop[MAX_LoraNodeNum];  //for MASTER 
extern tDeviceNodeSensor *DeviceNodeSensor[MAX_LoraNodeNum];  //for MASTER
extern uint8_t LoraNodeCount;                                 //for MASTER
extern tLoraDeviceNode *LoraGateWay;                          //for SLAVE
extern tDeviceNodeSensor *MySensor;                           //for SLAVE
///////////////// for Lora Event ///////////////////////////
extern uint8_t EventCountPriority0, EventCountPriority1, EventCountPriority2;
///////////////// for Button ///////////////////////////
extern __IO bool PowerButton;
extern __IO bool FactoryResetButton;
///////////////// for Sleep ///////////////////////////
__IO bool Slave_PollEventAccomplish = false;                   //for SLAVE
extern __IO bool Slave_PollEvent;                              //for SLAVE
extern __IO bool Slave_PollEvent_UTCnotZero;                   //for SLAVE
///////////////// for Product Verification /////////////////
__IO uint32_t LoraPV_RxCount;
__IO bool LoraPV_RxTest;
__IO bool LoraPV_TxTest;
__IO bool LoraPV_DefaultTxSet;
__IO uint32_t LoraPV_TxCount;
__IO uint32_t LoraPV_TxTimes;
///////////////// for Ping-Pong test ///////////////////////
static uint16_t BufferSize = 9;             // RF buffer size
__IO static uint64_t RX_COUNT = 0;
__IO static uint64_t RX_Done_COUNT = 0;
__IO static uint64_t RX_HasData_COUNT = 0;
static uint8_t Buffer[9];                   // RF buffer
const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";




/* Private function prototypes -----------------------------------------------*/
static void RCC_Configuration(void);
static void RX_DataCopy(uint8_t *, const uint8_t *, uint16_t);
static bool MasterLoraEvent_PROCESS(void);
static void OnMasterForNormal(void);
static void OnSlaveForNormal(void);
static void ForLoraProductVerification(void);
static void ForFskProductVerification(void);
static void OnMasterForPingPongTest(void);
static void OnSlaveForPingPongTest(void);
static void RxTestOutput(void);
static void PacketOutput(uint8_t *, size_t);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int	main( void )
{
	/* Add your application code here */
	//static uint8_t	LoraNum = 0;
  uint32_t delayTick, totalDelay;

	SystemOperMode = SystemInNormal;
	//SystemOperMode = SystemInPingPognTest;
	//SystemOperMode = SystemInProductVerification;
	LoraNeedTxData = false;
	LoraTxPayloadSize = 0;
	memset((void *)LoraTxBuffer, 0, LoraBufferLength);
	LoraRxPayloadSize = 0;
	memset((void *)LoraRxBuffer, 0, LoraBufferLength);
  
#ifdef STM32F401xx
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0xC000);
#endif

	RCC_Configuration();
  RTC_TimerConfig();
  
#ifdef STM32F401xx
#ifdef	USBD_VCP_Console
  if(UsbDegugOn == true) {
	  USBD_Init( &USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc,  &USBD_CDC_cb,  &USR_cb );
    TIM9_TimerConfig();
  }
#endif
#endif
  
  LoraPara_LoadAndConfiguration();
	BoardInit();
	Radio = RadioDriverInit();
	Radio->Init();
  SX1276LoRaSetHopPeriod(Lora_RFHoppingPeriod);
	Radio->StartRx();

	AcsipProtocol_SetMyAddress();
	memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
	memset((void *)&RxFrame, 0, sizeof(tAcsipProtocolFrame));
	LoraLinkListEvent_Initialization();
  
  SaveRecord_ReadOutLoraGateWayPara();
  SaveRecord_ReadOutLoraNodePara();
  SaveRecord_TraceDataSpaceInitialization();
  
  Button_ButtonInitialization();
  Led_PinInitialization();

	enableGlobalInterrupts();
  
  Led_BootFlashLed();
  
  BlueTooth_DA14580Run(ComPortBaudRate);
  if(EnableMaster == false) {                //┐
    CmdUART_UartInit(ComPortBaudRate);       //├暫時給 SLAVE 測試用,若不執行此,則 CMD UART 輸出會一直等待完成而使得無限等待
	  CmdTIMER_TimerConfig();                  //┘
  }
  
  if(EnableMaster == false) {
    GPS_MT3333Run();
  }
  
  RTC_AlarmConfig();
  RTC_AlarmRun();
  
  CmdUART_UartWrite((uint8_t *)"FirmwareVersion=", strlen("FirmwareVersion="));
	CmdUART_UartWrite((uint8_t *)FirmwareVersion, strlen(FirmwareVersion));
	CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
	CmdUART_UartWrite((uint8_t *)"LORA module start RUNNING...\r\n", strlen("LORA module start RUNNING...\r\n"));
  
  SLEEP_SlaveSleepVariableSet();
  
  //SLEEP_SlaveSleepProcedure(60);       //for sleep test
  //CmdUART_UartWrite((uint8_t *)"wake up...\r\n", strlen("wake up...\r\n"));       //for sleep test
  
	while( 1 ){
		//Program_Sample
		LEDBlink(1000,1000,10000);
		//Program_Sample end
		if(isRX_Buf1Full == true) {
			memset((void *)CLI_Buf, 0, CmdUART_RX_BufLength);
			RX_DataCopy(CLI_Buf, (const uint8_t *)RX_Buf1, CmdUART_RX_Length);
			memset((void *)RX_Buf1, 0, CmdUART_RX_BufLength);
			isRX_Buf1Full = false;
			goto CLI_PROCESS;	//CLI_Buf處理

		}
		else{
			if(isRX_Buf2Full == true) {
				memset((void *)CLI_Buf, 0, CmdUART_RX_BufLength);
				RX_DataCopy(CLI_Buf, (const uint8_t *)RX_Buf2, CmdUART_RX_Length);
				memset((void *)RX_Buf2, 0, CmdUART_RX_BufLength);
				isRX_Buf2Full = false;
				goto CLI_PROCESS;	//CLI_Buf處理
			}
		}
    
#ifdef STM32F401xx
#ifdef	USBD_VCP_Console
    if(UsbDegugOn == true) {
      if( isVCP_RX_Buf1Full == true ){
        memset( (void *) CLI_Buf, 0, VCP_RX_BufLength );
        RX_DataCopy( CLI_Buf, (const uint8_t *) VCP_RX_Buf1, VCP_RX_Length );
        memset( (void *) VCP_RX_Buf1, 0, VCP_RX_BufLength );
        isVCP_RX_Buf1Full = false;
        goto CLI_PROCESS;	//CLI_Buf處理
      }
      else {
        if( isVCP_RX_Buf2Full == true ) {
          memset( (void *) CLI_Buf, 0, VCP_RX_BufLength );
          RX_DataCopy( CLI_Buf, (const uint8_t *) VCP_RX_Buf2, VCP_RX_Length );
          memset( (void *) VCP_RX_Buf2, 0, VCP_RX_Length );
          isVCP_RX_Buf2Full = false;
          goto CLI_PROCESS;	//CLI_Buf處理
        }
      }
    }
#endif
#endif
    
    if(PowerButton == true) {
      Button_PowerButtonAction();
      PowerButton = false;
    }
    
    if(FactoryResetButton == true) {
      totalDelay = 0;
      while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0) {            // PB4=0, to mean keep press button
        delayTick = GET_TICK_COUNT( );                                      // every 20ms, scan button
        while( ( GET_TICK_COUNT( ) - delayTick ) < TICK_RATE_MS( 20 ) );    // if button still pressed, then total++
        totalDelay++;                                                        // button release, then go out while loop
      }
      if(totalDelay >= 250) {                                                // if keep press button > 5s (250 * 20ms = 5000ms)
        CLI_FactoryReset();                                             // then Factory Reset
        CmdUART_UartWrite((uint8_t *)"Factory Reset OK!\r\n", strlen("Factory Reset OK!\r\n"));
      }
      FactoryResetButton = false;
    }
    
		switch(SystemOperMode) {
			case SystemInNormal:
				if( (LoraStartWork == true) && (EnableMaster == true) ) {
					OnMasterForNormal();
          SLEEP_MasterSleepProcedure();
					/*if( EventCountPriority2 < 2){
						if( LoraNodeDevice[LoraNum] != NULL ){
							LoraLinkListEvent_BuildLoraEvent(LoraEventPriority2, LoraNum, Master_AcsipProtocol_Poll, LoraNodeDevice[LoraNum]->NodeAddress, NULL, NULL);
						}
						LoraNum++;
						if( LoraNum >= MAX_LoraNodeNum ) LoraNum = 0;
					}*/
				}
				else{
					if( (LoraStartWork == true) &&  (EnableMaster == false) ) {
            GPS_ReadIn(MySensor);
            OnSlaveForNormal();
            if(LoraGateWay != NULL) {
              SLEEP_SlaveSleepProcedure(&LoraGateWay->Interval);
            }
          }
				}
				break;

			case SystemInProductVerification:
				if( LoraStartWork == true ){
					if(LoRaOn == true){
						ForLoraProductVerification();
					}
					else{
						ForFskProductVerification();
					}
				}
				break;

			case SystemInPingPognTest:
				if( (EnableMaster == true) && (LoraStartWork == true) ) {
					OnMasterForPingPongTest();
				}
				else {
					if( LoraStartWork == true ) {
						OnSlaveForPingPongTest();
					}
				}
				break;

			case SystemInIdle:
				break;

			default:
				break;
		}

		while( 0 ){
CLI_PROCESS:		//(可考慮把此部分寫成一個函式)
			CLI_process( (char *)CLI_Buf );
			//藉由判斷ret來了解接收到的資料狀態,非指令(如ret回傳狀態=SHELL_PROCESS_ERR_CMD_UNKN)則當作資料傳輸出去(也要判斷lora是否有連上線)
			//目前構思每一個cli指令執行後都會回傳一個狀態(ok、err)或結果
			memset((void *)CLI_Buf, 0, CmdUART_RX_BufLength);
			CmdUART_RX_Length = 0;
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
void LoraPara_LoadAndConfiguration(void) {
  
  EnableMaster = 1;        // 1=Master or 0=Slave selection
  
  
	LoRaSettings.Power = 20;
	LoRaSettings.SignalBw = 7;		// LORA [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
						// 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]  
	LoRaSettings.SpreadingFactor = 10;	// LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
	LoRaSettings.FreqHopOn = 0;		// [0: OFF, 1: ON]
	LoRaSettings.PayloadLength = MaxPacketSize;
	LoRaSettings.PreambleLength = 12;
  
  SaveRecord_ReadOutMyselfPara();
  
  LoRaSettings.RFFrequency = Lora_RFFrequency;
  LoRaSettings.HopPeriod = Lora_RFHoppingPeriod;		// Hops every frequency hopping period symbols
  LoRaSettings.ErrorCoding = 1;		// LORA [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
  LoRaSettings.CrcOn = true;		// [0: OFF, 1: ON]
	LoRaSettings.ImplicitHeaderOn = 0;	// [0: OFF, 1: ON]
  LoRaSettings.RxSingleOn = 1;		// [0: Continuous, 1 Single]
  LoRaSettings.LowDataRateOptimize = 1;	// [0: OFF, 1: ON]
  LoRaSettings.TxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.RxPacketTimeout = LoRaSettings.TxPacketTimeout;
  //LoRaSettings.Oscillator = OscillatorTCXO;
	LoRaSettings.Oscillator = OscillatorCrystal;
  
}



/***************************************************************************************************
 *  Function Name: RCC_Configuration
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	RCC_Configuration( void )
{
	uint16_t	i;

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
	for(i = 0 ; i < 50000 ; i++) ;

	while(RCC_WaitForHSEStartUp() != SUCCESS);
	/*!< Wait till HSE is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
  
	RCC_PLLCmd(ENABLE);
#ifdef STM32F072
	RCC_PLLConfig(RCC_PLLSource_HSE, RCC_PLLMul_2);
#endif

#ifdef STM32F401xx
	/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
	/* SYSCLK = PLL_VCO / PLL_P */
	RCC_PLLConfig(RCC_PLLSource_HSE, 13, 168, 4, 7);
#endif

	for(i = 0 ; i < 50000 ; i++);
	/*!< Wait till PLL is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	/* Check if the PLL is still locked */
	if (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET) { 
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	}

#ifdef STM32F401xx
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	for(i = 0 ; i < 50000 ; i++);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	for(i = 0 ; i < 50000 ; i++);
	RCC_PCLK2Config(RCC_HCLK_Div2);
	for(i = 0 ; i < 50000 ; i++);
#endif
  
  
  //Below, for LSE(32768Hz) test
#ifdef STM32F072
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);  /* Enable the PWR clock */
  //RCC_LSEConfig(RCC_LSE_ON);    /* Enable the LSE OSC */
  //while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);  /* Wait till LSE is ready */
#endif
#ifdef STM32F401xx
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);  /* Enable the PWR clock */
  //RCC_LSEConfig(RCC_LSE_ON);    /* Enable the LSE OSC */
  //while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);  /* Wait till LSE is ready */
#endif
  
}



/***************************************************************************************************
 *  Function Name: RX_DataCopy
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void	RX_DataCopy( uint8_t *dst, const uint8_t *sou, uint16_t length)
{
	uint16_t	count;

	for( count = 0 ; count < length ; count++ ){
		dst[count] = sou[count];
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
	//int8_t	str[5];		//test output
	static bool	temp;
	static uint8_t	tx_size, i;
  
	/* 目前寫到最後的想法是將 NodeCount 搬到上一層去做++與range判斷
	因為若是在這裡做這件事,在後面進到NormalMaster函式裡後,帶入的值
	會再受 join、data、leave等影響此值,用此外面加值方式,可能可以把LoraEvent拿掉*/

	/* 進到switch( Radio->Process( ) )後,須注意一件事,就是會有其他狀態,如busy,如此很在此NodeCount下可能就跳過
	必須等待rx_timeout或rx_done等的狀況,所以須如實等待,也需防止無限的busy等當機狀態,(注意slave是否也須注意此情形)  */

	temp = false;
	if(LoraLinkListEvent_DispatcherLoraEvent() == true) {
		//CmdUART_UartWrite((uint8_t *)"DispatcherLoraEvent=true\r\n", strlen("DispatcherLoraEvent=true\r\n"));    //test output
		switch(LoraRunningEvent.RunNodeEvent) {
		case Master_AcsipProtocol_Broadcast:
			//增加廣播請求函式
			//LoraTxPayloadSize = tx_size;
			//temp = true;
			break;

		case Master_AcsipProtocol_Join:
			AcsipProtocol_LoraJoinNetworkRequest(LoraRunningEvent.RunNodeAddr, &TxFrame, LoraTxBuffer, &tx_size);
			LoraTxPayloadSize = tx_size;
			temp = true;
			break;

		case Master_AcsipProtocol_Poll:
			AcsipProtocol_LoraPollRequest(LoraNodeDevice[LoraRunningEvent.RunNodeNumber], &TxFrame, LoraTxBuffer, &tx_size);
			LoraTxPayloadSize = tx_size;
			//LoraPollEventCount--;
			temp = true;
			break;

		case Master_AcsipProtocol_Data:
			AcsipProtocol_LoraDataRequest(LoraNodeDevice[LoraRunningEvent.RunNodeNumber], LoraRunningEvent.RunNodeData, &LoraRunningEvent.RunNodeDataSize, &TxFrame, LoraTxBuffer, &tx_size);
			LoraTxPayloadSize = tx_size;
			temp = true;
			break;

		case Master_AcsipProtocol_Leave:
			AcsipProtocol_LoraLeaveNetworkRequest(LoraNodeDevice[LoraRunningEvent.RunNodeNumber], &TxFrame, LoraTxBuffer, &tx_size);
			LoraTxPayloadSize = tx_size;
			temp = true;
			break;
    
    case Master_AcsipProtocol_Interval:
			AcsipProtocol_LoraIntervalRequest(LoraNodeDevice[LoraRunningEvent.RunNodeNumber], &TxFrame, LoraTxBuffer, &tx_size);
      LoraTxPayloadSize = tx_size;
			temp = true;
      //CmdUART_UartWrite((uint8_t *)"LoraEvent_PROCESS_Interval\r\n", strlen("LoraEvent_PROCESS_Interval\r\n"));    //test output
      break;

		default:
			break;
		}
    
    if(LoraTxPayloadSize != 0) {
      for(i = LoraTxPayloadSize ; i < MaxPacketSize ; i++) LoraTxBuffer[i] = i;
      LoraTxPayloadSize = MaxPacketSize;
      Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
    }
    
	} else {
		//CmdUART_UartWrite((uint8_t *)"DispatcherLoraEvent=false\r\n", strlen("DispatcherLoraEvent=false\r\n"));    //test output
		//sprintf((char *)str, "%02u", LoraEvent.NodeEvent);    //test output
		//CmdUART_UartWrite((uint8_t *)"LoraEvent=", strlen("LoraEvent="));    //test output
		//CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));    //test output
		//CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));    //test output
		//sprintf((char *)str, "%02d", EventCountPriority2);    //test output
		//CmdUART_UartWrite((uint8_t *)"EventCountPriority2=", strlen("EventCountPriority2="));    //test output
		//CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));    //test output
		//CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));    //test output
		if(LoraRunningEvent.RunNodeEvent == Master_AcsipProtocol_Poll) {
			//CmdUART_UartWrite((uint8_t *)"LoraEvent=Poll\r\n", strlen("LoraEvent=Poll\r\n"));    //test output
			if(EventCountPriority2 > 0) EventCountPriority2--;
		}
    //CmdUART_UartWrite((uint8_t *)"MemsetLoraEvent\r\n", strlen("MemsetLoraEvent\r\n"));    //test output
	  memset((void *)&LoraRunningEvent, 0, sizeof(tLoraRunningEvent));
	}

	return temp;
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
  int8_t count, str[10];
  
	switch( Radio->Process() ) {
	case RF_RX_TIMEOUT:
		//CmdUART_UartWrite((uint8_t *)"RX_TIMEOUT\r\n", strlen("RX_TIMEOUT\r\n"));    //test output
    /*
		if(MasterLoraEvent_PROCESS() == true) {   //看看是否有新的  LoraEvent 需要執行
			for(i = LoraTxPayloadSize ; i < MaxPacketSize ; i++) LoraTxBuffer[i] = i;

			LoraTxPayloadSize = MaxPacketSize;
			//PacketOutput(LoraTxBuffer, LoraTxPayloadSize);    //test output
			Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
		} else {
			Radio->StartRx( );
		}
    */
    
    if((LoraRunningEvent.RunNodeEvent == Master_AcsipProtocol_Poll) && (LoraNodeDevice[LoraRunningEvent.RunNodeNumber] != NULL)) {
      CmdUART_UartWrite((uint8_t *)"Node=", strlen("Node="));
      for(count = 2 ; count >= 0 ; count--) {
        sprintf((char *)str, "%02x", LoraRunningEvent.RunNodeAddr[count]);
        CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
      }
      CmdUART_UartWrite((uint8_t *)" EVT=GPS ", strlen(" EVT=GPS "));
      CmdUART_UartWrite((uint8_t *)"0 0 0 0 0\r\n", strlen("0 0 0 0 0\r\n"));
    }       // 用來通知上層(如藍芽、APP),SLAVE Node 沒有回應, 此時 SLAVE Node 可能睡覺或已離線
    
    if((LoraRunningEvent.RunNodeEvent != Master_AcsipProtocol_Broadcast) && (LoraRunningEvent.RunNodeEvent != Master_AcsipProtocol_Join) && (LoraRunningEvent.RunNodeEvent != 0)) {
      if(DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber] != NULL) {
        DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->LoraRxFailureTimes += 1;
        if(DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->isLoraDisconnecting == false) {
          if(DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->LoraRxFailureTimes > (LoraReceptionFailureTimes + DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->DefineLoraRxFailureTimes)) {
            CmdUART_UartWrite((uint8_t *)"Node=", strlen("Node="));
            for(count = 2 ; count >= 0 ; count--) {
              sprintf((char *)str, "%02x", LoraRunningEvent.RunNodeAddr[count]);
              CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
            }
            CmdUART_UartWrite((uint8_t *)" EVT=Disconnecting\r\n", strlen(" EVT=Disconnecting\r\n"));
            DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->isLoraDisconnecting = true;
          }
        }
      }    // 用來通知上層(如藍芽、APP),SLAVE 的 Lora 是斷線狀態
    }      //當沒有接收到 SLAVE Node 傳過來的訊息時,連續累積到一定次數後,只輸出一次裝置為斷線狀態,並把狀態標註起來
    
    if((LoraRunningEvent.RunNodeEvent == Master_AcsipProtocol_Interval) && (LoraNodeDevice[LoraRunningEvent.RunNodeNumber] != NULL)) {
      LoraNodeDevice[LoraRunningEvent.RunNodeNumber]->Interval = 0;
    }
    
    if(MasterLoraEvent_PROCESS() == false) {
      Radio->StartRx( );
    }
		break;

	case RF_RX_DONE:
		//CmdUART_UartWrite((uint8_t *)"RX_Done\r\n", strlen("RX_Done\r\n"));    //test output
		Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );
		//PacketOutput(LoraRxBuffer, LoraRxPayloadSize);    //test output
		LoraRxPayloadSize = LoraRxBuffer[0] + 8;
		if(AcsipProtocol_PacketToFrameProcess(LoraRxBuffer, (uint8_t)LoraRxPayloadSize, &RxFrame) == true) {
			if((LoraRunningEvent.RunNodeEvent != 0)) {
				//
				NormalMaster(&LoraRunningEvent);
				//上面的或下面有額外狀況需處理
				/*
				if(NormalMaster(&LoraEvent) == AcsipProtocol_OK) {
				} //else { }  //格式錯誤是否重發或等待
				*/
			}
		}
		//else { }   //不是對方傳過來的,是否重新等待

		memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
		memset((void *)&RxFrame, 0, sizeof(tAcsipProtocolFrame));
		memset((void *)LoraTxBuffer, 0, LoraBufferLength);
		memset((void *)LoraRxBuffer, 0, LoraBufferLength);
		LoraTxPayloadSize = 0;
		LoraRxPayloadSize = 0;
    
    /*
		if(MasterLoraEvent_PROCESS() == true) {
			for(i = LoraTxPayloadSize ; i < MaxPacketSize ; i++) LoraTxBuffer[i] = i;

			LoraTxPayloadSize = MaxPacketSize;
			//PacketOutput(LoraTxBuffer, LoraTxPayloadSize);    //test output
			Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
		}
		else {
			Radio->StartRx( );
		}
    */
    if((LoraRunningEvent.RunNodeEvent != Master_AcsipProtocol_Broadcast) && (LoraRunningEvent.RunNodeEvent != Master_AcsipProtocol_Join) && (LoraRunningEvent.RunNodeEvent != 0)) {
      if(DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber] != NULL) {
        DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->isLoraDisconnecting = false;
        DeviceNodeSleepAndRandomHop[LoraRunningEvent.RunNodeNumber]->LoraRxFailureTimes = 0;
      }
    }       //當有成功接收到相關訊息,則持續將 SLAVE Node 判定為連線狀態
    
    if(MasterLoraEvent_PROCESS() == false) {
      Radio->StartRx( );
    }
		break;

	case RF_TX_DONE:
		//CmdUART_UartWrite((uint8_t *)"TX_Done\r\n", strlen("TX_Done\r\n"));    //test output
		if(TxFrame.FrameFlag == FrameFlag_Broadcast) {
			memset((void *)LoraTxBuffer, 0, LoraBufferLength);
			LoraTxPayloadSize = 0;
		}
		Radio->StartRx( );
		break;

	case RF_TX_TIMEOUT:
		//CmdUART_UartWrite((uint8_t *)"TX_TIMEOUT\r\n", strlen("TX_TIMEOUT\r\n"));    //test output
		memset((void *)LoraTxBuffer, 0, LoraBufferLength);
		LoraTxPayloadSize = 0;
		memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
    /*
		if(MasterLoraEvent_PROCESS() == true) {
			for(i = LoraTxPayloadSize ; i < MaxPacketSize ; i++) LoraTxBuffer[i] = i;

			LoraTxPayloadSize = MaxPacketSize;
			//PacketOutput(LoraTxBuffer, LoraTxPayloadSize);    //test output
			Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
		}
		else {
			Radio->StartRx( );
		}
    */
    if(MasterLoraEvent_PROCESS() == false) {
      Radio->StartRx( );
    }
		break;

	default:
    //CmdUART_UartWrite((uint8_t *)"default\r\n", strlen("default\r\n"));    //test output
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
  static uint8_t base64_data[((MaxMsgDataSize/3)*4)];
  size_t dsize;
	uint8_t		i;
	int8_t		count;
	int8_t		str[10];

	switch( Radio->Process( ) ){
	case RF_RX_TIMEOUT:
		//CmdUART_UartWrite((uint8_t *)"RX_TIMEOUT\r\n", strlen("RX_TIMEOUT\r\n"));    //test output
		Radio->StartRx( );
		break;

	case RF_RX_DONE:
		//CmdUART_UartWrite((uint8_t *)"RX_Done\r\n", strlen("RX_Done\r\n"));    //test output
		Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );
		//PacketOutput(LoraRxBuffer, LoraRxPayloadSize);    //test output
		LoraRxPayloadSize = LoraRxBuffer[0] + 8;
		if(AcsipProtocol_PacketToFrameProcess(LoraRxBuffer, (uint8_t)LoraRxPayloadSize, &RxFrame) == true) {
			//CmdUART_UartWrite((uint8_t *)"error5\r\n", strlen("error5\r\n"));    //test output
			if(NormalSlave() == AcsipProtocol_OK) {
				//CmdUART_UartWrite((uint8_t *)"error7\r\n", strlen("error7\r\n"));    //test output
				for(i = LoraTxPayloadSize ; i < MaxPacketSize ; i++) LoraTxBuffer[i] = i;

				LoraTxPayloadSize = MaxPacketSize;
				//PacketOutput(LoraTxBuffer, LoraTxPayloadSize);    //test output
				Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
        
				if((RxFrame.FrameFlag == FrameFlag_Data) && (RxFrame.MsgLength > 0)) {
          memset((void *)base64_data, 0, ((MaxMsgDataSize/3)*4));
          if(Base64_encode( base64_data, ((MaxMsgDataSize/3)*4), &dsize, (const uint8_t	*)RxFrame.MsgData, RxFrame.MsgLength ) == 0) {
					  CmdUART_UartWrite((uint8_t *)"LoraGateWayAddr=", strlen("LoraGateWayAddr="));
					  for( count = 2 ; count >= 0 ; count-- ){
						  sprintf((char *)str, "%02x", LoraGateWay->NodeAddress[count]);
						  CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
					  }
            CmdUART_UartWrite((uint8_t *)"  DataLength=", strlen("  DataLength="));
            sprintf((char *)str, "%u", dsize);
            CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
            CmdUART_UartWrite((uint8_t *)"  Data=", strlen("  Data="));
            CmdUART_UartWrite(base64_data, dsize);
            CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
					  //CmdUART_UartWrite((uint8_t *)"  ", 2);
					  //CmdUART_UartWrite((uint8_t *)"DataLength=", strlen("DataLength="));
					  //sprintf((char *)str, "%u", RxFrame.MsgLength);
					  //CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
					  //CmdUART_UartWrite((uint8_t *)"  ", 2);
					  //CmdUART_UartWrite((uint8_t *)"Data=", strlen("Data="));
					  //CmdUART_UartWrite((uint8_t *)RxFrame.MsgData, RxFrame.MsgLength);
					  //CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
					  //或是存下來或是透過藍芽傳出去
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
        
			}
		}

		memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
		memset((void *)&RxFrame, 0, sizeof(tAcsipProtocolFrame));
		memset((void *)LoraRxBuffer, 0, LoraBufferLength);
		LoraRxPayloadSize = 0;
		break;

	case RF_TX_DONE:
		//CmdUART_UartWrite((uint8_t *)"TX_Done\r\n", strlen("TX_Done\r\n"));    //test output
    if(Slave_PollEvent == true) {
      Slave_PollEvent = false;
      if(Slave_PollEvent_UTCnotZero == true) {
        Slave_PollEvent_UTCnotZero = false;
        Slave_PollEventAccomplish = true;
      } else {
        Slave_PollEventAccomplish = false;
      }
    }
		memset((void *)LoraTxBuffer, 0, LoraBufferLength);
		LoraTxPayloadSize = 0;
		memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
		Radio->StartRx( );
		break;

	case RF_TX_TIMEOUT:
		//CmdUART_UartWrite((uint8_t *)"TX_TIMEOUT\r\n", strlen("TX_TIMEOUT\r\n"));    //test output
    if(Slave_PollEvent == true) {
      Slave_PollEvent = false;
    }
		memset((void *)LoraTxBuffer, 0, LoraBufferLength);
		LoraTxPayloadSize = 0;
		Radio->StartRx( );
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
	switch( Radio->Process( ) ){
	case RF_RX_DONE:
		if( LoraPV_RxTest == true ) {
			Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );	//取值
			LoraPV_RxCount++;
			Radio->StartRx( );
		}
		else{
			if( LoraPV_TxTest == true ) {
				Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );	//取值
				memset((void *)LoraRxBuffer, 0, LoraBufferLength);		//清空buf
				//若有ACK需求就需比對ACK,目前暫定在生產驗證上不須回傳ACK
				if( LoraPV_DefaultTxSet == true ){
					Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );	//Lora傳值出去
					LoraPV_TxCount++;
				}
				else{
					if( LoraPV_TxCount < LoraPV_TxTimes ){
						Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );	//Lora傳值出去
						LoraPV_TxCount++;
					}
					else{
						CLI_LoraPV_TXresult();
					}
				}
			}
			else{
				Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );	//取值
				memset((void *)LoraRxBuffer, 0, LoraBufferLength);		//清空buf
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
		}
		else{
			if( LoraPV_TxTest == true ){
				if( LoraPV_DefaultTxSet == true ){
					Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );		//Lora傳值出去
					LoraPV_TxCount++;
				}
				else{
					if(LoraPV_TxCount < LoraPV_TxTimes) {
						Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );	//Lora傳值出去
						LoraPV_TxCount++;
					}
					else {
						CLI_LoraPV_TXresult();
					}
				}
			}
			else{
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
	switch( Radio->Process() ){
	case RF_RX_DONE:
		Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );	//取值
		memset((void *)LoraRxBuffer, 0, LoraBufferLength);		//清空buf
		Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );	//Lora傳值出去
		break;

	case RF_IDLE:
	case RF_TX_DONE:
	case RF_RX_TIMEOUT:
	case RF_TX_TIMEOUT:
	case RF_LEN_ERROR:
	case RF_CHANNEL_EMPTY:
	case RF_CHANNEL_ACTIVITY_DETECTED:
	case RF_BUSY:
		Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );      //Lora傳值出去
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

	switch( Radio->Process() ){
	case RF_RX_TIMEOUT:
		CmdUART_UartWrite( (uint8_t *)"RF_RX_TIMEOUT\r\n", strlen("RF_RX_TIMEOUT\r\n") );
		// Send the next PING frame
		Buffer[0] = 'P';
		Buffer[1] = 'I';
		Buffer[2] = 'N';
		Buffer[3] = 'G';
		for( i = 4; i < BufferSize; i++ ){
			Buffer[i] = i - 4;
		}
		Radio->SetTxPacket( Buffer, BufferSize );
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
				Buffer[0] = 'P';
				Buffer[1] = 'I';
				Buffer[2] = 'N';
				Buffer[3] = 'G';
				// We fill the buffer with numbers for the payload 
				for( i = 4; i < BufferSize; i++ ) {
					Buffer[i] = i - 4;
				}
				Radio->SetTxPacket( Buffer, BufferSize );
			}
			else{
				if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 ) {
					// A master already exists then become a slave
					EnableMaster = false;
				}
			}
		}
		break;

	case RF_TX_DONE:
		Radio->StartRx( );
		break;

	case RF_TX_TIMEOUT:
		CmdUART_UartWrite( (uint8_t *)"RF_TX_TIMEOUT\r\n", strlen("RF_TX_TIMEOUT\r\n") );
		Radio->StartRx( );
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

	switch( Radio->Process() ){
	case RF_RX_DONE:
		Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
		RX_Done_COUNT++;
		if( BufferSize > 0 ) {
			RX_HasData_COUNT++;
			if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 ) {
				RX_COUNT++;
				RxTestOutput();
				// Send the reply to the PONG string
				Buffer[0] = 'P';
				Buffer[1] = 'O';
				Buffer[2] = 'N';
				Buffer[3] = 'G';
				// We fill the buffer with numbers for the payload 
				for( i = 4; i < BufferSize; i++ ) {
					Buffer[i] = i - 4;
				}
				Radio->SetTxPacket( Buffer, BufferSize );
			}
		}
		break;

	case RF_TX_DONE:
		Radio->StartRx( );
		break;

	case RF_TX_TIMEOUT:
		CmdUART_UartWrite((uint8_t *)"RF_TX_TIMEOUT\r\n", strlen("RF_TX_TIMEOUT\r\n"));
		Radio->StartRx( );
		break;

	case RF_RX_TIMEOUT:
		CmdUART_UartWrite((uint8_t *)"RF_RX_TIMEOUT\r\n", strlen("RF_RX_TIMEOUT\r\n"));
		Radio->StartRx( );
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

	CmdUART_UartWrite((uint8_t *)"Rx=", strlen("Rx="));
	CmdUART_UartWrite((uint8_t *)Buffer, 4);
	CmdUART_UartWrite((uint8_t *)"    ", 4);

	sprintf((char *)str, "%d", (int)RX_HasData_COUNT);
	CmdUART_UartWrite((uint8_t *)"RxCount=", strlen("RxCount="));
	CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
	CmdUART_UartWrite((uint8_t *)"    ", 4);

	sprintf((char *)str, "%d", (int)RX_COUNT);
	CmdUART_UartWrite((uint8_t *)"RxCompleteCount=", strlen("RxCompleteCount="));
	CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
	CmdUART_UartWrite((uint8_t *)"    ", 4);

	rssi = SX1276LoRaGetPacketRssi();
	sprintf((char *)str, "%3.2f", rssi);
	CmdUART_UartWrite((uint8_t *)"RSSI=", strlen("RSSI="));
	CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
	CmdUART_UartWrite((uint8_t *)"    ", 4);

	snr = SX1276LoRaGetPacketSnr();
	sprintf((char *)str, "%d", snr);
	CmdUART_UartWrite((uint8_t *)"SNR=", strlen("SNR="));
	CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
	CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
	
}



/***************************************************************************************************
 *  Function Name: PacketOutput
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
static void	PacketOutput( uint8_t *array, size_t size )
{
	int8_t		str[10];    //test output
	uint16_t	count;
  
	CmdUART_UartWrite((uint8_t *)"PacketSize=", strlen("PacketSize="));
	sprintf((char *)str, "%u", size);
	CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
	CmdUART_UartWrite((uint8_t *)"  ", 2);

	CmdUART_UartWrite((uint8_t *)"Packet=", strlen("Packet="));

	for(count = 0 ; count < size ; count++) {
		sprintf((char *)str, "%02x", array[count]);    //test output
		CmdUART_UartWrite((uint8_t *)str, strlen((const char *)str));
		CmdUART_UartWrite((uint8_t *)"  ", 2);
	}

	CmdUART_UartWrite((uint8_t *)"\r\n", strlen("\r\n"));
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
	while (1){
	}
}
#endif

/**
  * 
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
