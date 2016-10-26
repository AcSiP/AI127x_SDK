/**
  ******************************************************************************
  * @file    Project/UnionStructTestMain.c 
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
#include "save_record.h"
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
bool EnableMaster = 0;        // 1=Master or 0=Slave selection
///////////////// for CmdUART & CLI ///////////////////////
extern __IO uint16_t CmdUART_RX_Length;
extern __IO uint8_t RX_Buf1[CmdUART_RX_BufLength];
extern __IO uint8_t RX_Buf2[CmdUART_RX_BufLength];
extern __IO bool isRX_Buf1Full;
extern __IO bool isRX_Buf2Full;
extern __IO bool LoRaOn;
uint8_t CLI_Buf[CmdUART_RX_BufLength];
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
extern tDeviceNodeSensor *DeviceNodeSensor[MAX_LoraNodeNum];  //for MASTER
extern uint8_t LoraNodeCount;                                 //for MASTER
extern tLoraDeviceNode *LoraGateWay;                          //for SLAVE
extern tDeviceNodeSensor *MySensor;                           //for SLAVE
///////////////// for Lora Event ///////////////////////////
extern uint8_t EventCountPriority0, EventCountPriority1, EventCountPriority2;
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
static void RX_DataCopy(uint8_t *, const uint8_t *, uint16_t);
static void RCC_Configuration(void);
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
	static uint8_t	LoraNum = 0;
  //uint16_t	ret = 0;

	SystemOperMode = SystemInNormal;
	//SystemOperMode = SystemInPingPognTest;
	//SystemOperMode = SystemInProductVerification;
	LoraNeedTxData = false;
	LoraTxPayloadSize = 0;
	memset((void *)LoraTxBuffer, 0, LoraBufferLength);
	LoraRxPayloadSize = 0;
	memset((void *)LoraRxBuffer, 0, LoraBufferLength);
	LoraOperationMode = 0;



	LoRaSettings.RFFrequency = Lora_RFFrequency;
	LoRaSettings.Power = 17;
	LoRaSettings.SignalBw = 8;		// LORA [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
						// 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]  
	LoRaSettings.SpreadingFactor = 8;	// LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
	LoRaSettings.ErrorCoding = 1;		// LORA [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
	LoRaSettings.CrcOn = true;		// [0: OFF, 1: ON]
	LoRaSettings.ImplicitHeaderOn = 0;	// [0: OFF, 1: ON]
	LoRaSettings.RxSingleOn = 1;		// [0: Continuous, 1 Single]
	LoRaSettings.FreqHopOn = 0;		// [0: OFF, 1: ON]
	LoRaSettings.HopPeriod = 40;		// Hops every frequency hopping period symbols
	LoRaSettings.PayloadLength = MaxPacketSize;
  //LoRaSettings.PayloadLength = 9;
	LoRaSettings.PreambleLength = 12;
	LoRaSettings.LowDataRateOptimize = 1;	// [0: OFF, 1: ON]
	LoRaSettings.TxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.RxPacketTimeout = LoRaSettings.TxPacketTimeout;
	//LoRaSettings.Oscillator = OscillatorTCXO;
	LoRaSettings.Oscillator = OscillatorCrystal;

	RCC_Configuration();
	CmdUART_UartInit(38400);
	CmdTIMER_TimerConfig();

	BoardInit();
	Radio = RadioDriverInit();
	Radio->Init();
  CLI_LoraOP_SF9BW250RS126(false);
	Radio->StartRx();

	AcsipProtocol_SetMyAddress();
	memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
	memset((void *)&RxFrame, 0, sizeof(tAcsipProtocolFrame));
	LoraLinkListEvent_Initialization();
  
  if(EnableMaster == false) {
    GPS_MT3333Run();
  }

	enableGlobalInterrupts();

	Console_Output_String( "FirmwareVersion=" );
	Console_Output_String( FirmwareVersion );
	Console_Output_String( " for " );
	Console_Output_String( Target_Lora_Device );
	Console_Output_String( "\r\n" );
	Console_Output_String( "LORA module start RUNNING..." );
	Console_Output_String( "\r\n" );
  
  //TEST//
  int8_t str[10];
  Console_Output_String( "tLoRaSettings=" );
  sprintf((char *)str, "%d", sizeof(tLoRaSettings));
  Console_Output_String( (const char *)str );
  Console_Output_String( "\r\n" );
  Console_Output_String( "SaveRecordMyPara_t=" );
  sprintf((char *)str, "%d", sizeof(SaveRecordMyPara_t));
  Console_Output_String( (const char *)str );
  Console_Output_String( "\r\n" );
  Console_Output_String( "\r\n" );
  
  Console_Output_String( "tLoraNodePara=" );
  sprintf((char *)str, "%d", sizeof(tLoraNodePara));
  Console_Output_String( (const char *)str );
  Console_Output_String( "\r\n" );
  Console_Output_String( "SaveRecordNodePara_t=" );
  sprintf((char *)str, "%d", sizeof(SaveRecordNodePara_t));
  Console_Output_String( (const char *)str );
  Console_Output_String( "\r\n\r\n" );
  
  Console_Output_String( "tLoraNodeSensor=" );
  sprintf((char *)str, "%d", sizeof(tLoraNodeSensor));
  Console_Output_String( (const char *)str );
  Console_Output_String( "\r\n" );
  Console_Output_String( "SaveRecordNodeTraceData_t=" );
  sprintf((char *)str, "%d", sizeof(SaveRecordNodeTraceData_t));
  Console_Output_String( (const char *)str );
  Console_Output_String( "\r\n\r\n" );

	while( 1 ){
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

		switch(SystemOperMode) {
			case SystemInNormal:
				if( (LoraStartWork == true) && (EnableMaster == true) ) {
					OnMasterForNormal();
					if( EventCountPriority2 < 2){
						if( LoraNodeDevice[LoraNum] != NULL ){
							LoraLinkListEvent_BuildLoraEvent(LoraEventPriority2, LoraNum, Master_AcsipProtocol_Poll, LoraNodeDevice[LoraNum]->NodeAddress, NULL, NULL);
						}
						LoraNum++;
						if( LoraNum >= MAX_LoraNodeNum ) LoraNum = 0;
					}
				}
				else{
					if( (LoraStartWork == true) &&  (EnableMaster == false) ) {
            GPS_ReadIn(MySensor);
            OnSlaveForNormal();
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
		//Console_Output_String( "DispatcherLoraEvent=true\r\n" );
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

		default:
			break;
		}
    
    if(LoraTxPayloadSize != 0) {
      for(i = LoraTxPayloadSize ; i < MaxPacketSize ; i++) LoraTxBuffer[i] = i;
      LoraTxPayloadSize = MaxPacketSize;
      Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
    }
    
	} else {
		//Console_Output_String( "DispatcherLoraEvent=false\r\n" );
		//sprintf((char *)str, "%02u", LoraEvent.NodeEvent);    //test output
		//Console_Output_String( "LoraEvent=" );
		//Console_Output_String( (const char *)str );
		//Console_Output_String( "\r\n" );
		//sprintf((char *)str, "%02d", EventCountPriority2);    //test output
		//Console_Output_String( "EventCountPriority2=" );
		//Console_Output_String( (const char *)str );
		//Console_Output_String( "\r\n" );
		if(LoraRunningEvent.RunNodeEvent == Master_AcsipProtocol_Poll) {
			//Console_Output_String( "LoraEvent=Poll\r\n" );
			if(EventCountPriority2 > 0) EventCountPriority2--;
		}
    //Console_Output_String( "MemsetLoraEvent\r\n" );
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
	//static uint8_t	i;

	switch( Radio->Process() ){
	case RF_RX_TIMEOUT:
		//Console_Output_String( "RX_TIMEOUT\r\n" );
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
    if(MasterLoraEvent_PROCESS() == false) {
      Radio->StartRx( );
    }
		break;

	case RF_RX_DONE:
		//Console_Output_String( "RX_Done\r\n" );
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
    if(MasterLoraEvent_PROCESS() == false) {
      Radio->StartRx( );
    }
		break;

	case RF_TX_DONE:
		//Console_Output_String( "TX_Done\r\n" );
		if(TxFrame.FrameFlag == FrameFlag_Broadcast) {
			memset((void *)LoraTxBuffer, 0, LoraBufferLength);
			LoraTxPayloadSize = 0;
		}
		Radio->StartRx( );
		break;

	case RF_TX_TIMEOUT:
		//Console_Output_String( "TX_TIMEOUT\r\n" );
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
    //Console_Output_String( "default\r\n" );
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
		//Console_Output_String( "RX_TIMEOUT\r\n" );
		Radio->StartRx( );
		break;

	case RF_RX_DONE:
		//Console_Output_String( "RX_Done\r\n" );
		Radio->GetRxPacket( (void *)LoraRxBuffer, ( uint16_t* )&LoraRxPayloadSize );
		//PacketOutput(LoraRxBuffer, LoraRxPayloadSize);    //test output
		LoraRxPayloadSize = LoraRxBuffer[0] + 8;
		if(AcsipProtocol_PacketToFrameProcess(LoraRxBuffer, (uint8_t)LoraRxPayloadSize, &RxFrame) == true) {
			//Console_Output_String( "error5\r\n" );
			if(NormalSlave() == AcsipProtocol_OK) {
				//Console_Output_String( "error7\r\n" );
				for(i = LoraTxPayloadSize ; i < MaxPacketSize ; i++) LoraTxBuffer[i] = i;

				LoraTxPayloadSize = MaxPacketSize;
				//PacketOutput(LoraTxBuffer, LoraTxPayloadSize);    //test output
				Radio->SetTxPacket( (const void *)LoraTxBuffer, LoraTxPayloadSize );
				if((RxFrame.FrameFlag == FrameFlag_Data) && (RxFrame.MsgLength > 0)) {
          memset((void *)base64_data, 0, ((MaxMsgDataSize/3)*4));
          if(Base64_encode( base64_data, ((MaxMsgDataSize/3)*4), &dsize, (const uint8_t	*)RxFrame.MsgData, RxFrame.MsgLength ) == 0) {
					  Console_Output_String( "LoraGateWayAddr=" );
					  for( count = 2 ; count >= 0 ; count-- ){
						  sprintf((char *)str, "%02x", LoraGateWay->NodeAddress[count]);
						  Console_Output_String( (const char *)str );
					  }
            Console_Output_String( "  DataLength=" );
            sprintf((char *)str, "%u", dsize);
            Console_Output_String( (const char *)str );
            Console_Output_String( "  Data=" );
            Console_Write( base64_data, dsize );
            Console_Output_String( "\r\n" );
					  //Console_Output_String( "  " );
					  //Console_Output_String( "DataLength=" );
					  //sprintf((char *)str, "%u", RxFrame.MsgLength);
					  //Console_Output_String( (const char *)str );
					  //Console_Output_String( "  " );
					  //Console_Output_String( "Data=" );
					  //Console_Write( (uint8_t *)RxFrame.MsgData, RxFrame.MsgLength );
					  //Console_Output_String( "\r\n" );
					  //或是存下來或是透過藍芽傳出去
				  }
        }
			}
		}

		memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
		memset((void *)&RxFrame, 0, sizeof(tAcsipProtocolFrame));
		memset((void *)LoraRxBuffer, 0, LoraBufferLength);
		LoraRxPayloadSize = 0;
		break;

	case RF_TX_DONE:
		//Console_Output_String( "TX_Done\r\n" );
		memset((void *)LoraTxBuffer, 0, LoraBufferLength);
		LoraTxPayloadSize = 0;
		memset((void *)&TxFrame, 0, sizeof(tAcsipProtocolFrame));
		Radio->StartRx( );
		break;

	case RF_TX_TIMEOUT:
		//Console_Output_String( "TX_TIMEOUT\r\n" );
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
		Console_Output_String( "RF_RX_TIMEOUT\r\n" );
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
		Console_Output_String( "RF_TX_TIMEOUT\r\n" );
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
		Console_Output_String( "RF_TX_TIMEOUT\r\n" );
		Radio->StartRx( );
		break;

	case RF_RX_TIMEOUT:
		Console_Output_String( "RF_RX_TIMEOUT\r\n" );
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

	Console_Output_String( "Rx=" );
	Console_Write( (uint8_t *)Buffer, 4);
	Console_Output_String( "    " );

	sprintf((char *)str, "%d", (int)RX_HasData_COUNT);
	Console_Output_String( "RxCount=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "    " );

	sprintf((char *)str, "%d", (int)RX_COUNT);
	Console_Output_String( "RxCompleteCount=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "    " );

	rssi = SX1276LoRaGetPacketRssi();
	sprintf((char *)str, "%3.2f", rssi);
	Console_Output_String( "RSSI=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "    " );

	snr = SX1276LoRaGetPacketSnr();
	sprintf((char *)str, "%d", snr);
	Console_Output_String( "SNR=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
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
  
	Console_Output_String( "PacketSize=" );
	sprintf((char *)str, "%u", size);
	Console_Output_String( (const char *)str );
	Console_Output_String( "  " );

	Console_Output_String( "Packet=" );

	for(count = 0 ; count < size ; count++) {
		sprintf((char *)str, "%02x", array[count]);    //test output
		Console_Output_String( (const char *)str );
		Console_Output_String( "  " );
	}

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
	while (1){
	}
}
#endif

/**
  * 
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
