/**
  ******************************************************************************
  * @file    Project/FlashTestMain.c 
  * @author  
  * @version 
  * @date    
  * @brief   FlashTestMain program body
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FlashTestMain.h"

/** 
  * 
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint16_t CmdUART_RX_Length;
extern __IO uint8_t RX_Buf1[CmdUART_RX_BufLength];
extern __IO uint8_t RX_Buf2[CmdUART_RX_BufLength];
extern __IO bool isRX_Buf1Full;
extern __IO bool isRX_Buf2Full;
extern tLoRaSettings LoRaSettings;
uint8_t CLI_Buf[CmdUART_RX_BufLength];
__IO tAcsipProtocolFrame TxFrame;
__IO tAcsipProtocolFrame RxFrame;
__IO tLoraRunningEvent LoraRunningEvent;
__IO uint32_t SystemOperMode;
__IO bool LoraNeedTxData;
__IO bool LoraNeedTxAck;
__IO size_t LoraTxPayloadSize;
__IO size_t LoraRxPayloadSize;
__IO bool LoraNeedRxReplyAck;
__IO uint32_t LoraPV_RxCount;
__IO bool LoraPV_RxTest;
__IO bool LoraPV_TxTest;
__IO bool LoraPV_DefaultTxSet;
__IO uint32_t LoraPV_TxCount;
__IO uint32_t LoraPV_TxTimes;
uint8_t LoraTxBuffer[MAX_LoraPayloadLength];
uint8_t LoraRxBuffer[MAX_LoraPayloadLength];
uint8_t LoraOperationMode;
tRadioDriver *Radio = NULL;
bool LoraStartWork = 0;       // 1=start or 0=stop LORA
bool EnableMaster = 0; 				// 1=Master/ 0=Slave selection



#define BUFFER_SIZE                                 9 // Define the payload size here
static uint16_t BufferSize = BUFFER_SIZE;			// RF buffer size
__IO static uint64_t RX_COUNT = 0;
__IO static uint64_t RX_Done_COUNT = 0;
__IO static uint64_t RX_HasData_COUNT = 0;
static uint8_t Buffer[BUFFER_SIZE];					// RF buffer
const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

/* Private function prototypes -----------------------------------------------*/
void RX_DataCopy(uint8_t *, const uint8_t *, uint16_t);
void RCC_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void) {

  /* Add your application code here */
	uint8_t flash_test_string[] = "qwertASDFzxcv123";
	uint8_t flash_string[16];
	uint8_t flash_test_bytesdata[12] = {0x01, 0x02, 0x03, 0x04, 'A', 'B', 'C', 'D', 'z', 'w', 'x', 'z'};
	uint8_t flash_bytesdata[12] = {0};
	uint16_t flash_test_2bytesdata[9] = {0x0102, 0x0203, 0x0304, 0x0405, 0x0506, 0x0607, 0x0708, 0x0809, 0x0910};
	uint16_t flash_2bytesdata[9] = {0};
	uint32_t flash_test_4bytesdata[5] = {0x01020304, 0x45671234, 0x36251425, 0x04050607, 0x07891011};
	uint32_t flash_4bytesdata[5] = {0};
	uint64_t flash_test_8bytesdata[3] = {0x0102030405060708, 0x4567123498765432, 0x362514250198ABCD};
	uint64_t flash_8bytesdata[3] = {0};
	uint16_t ret;
	
	RCC_Configuration();
	CmdUART_UartInit(38400);
	CmdTIMER_TimerConfig();
	
#ifdef STM32F072
	//FLASH_EraseAllRecordPage();
  FLASH_EraseRecordPage(ADDR_FLASH_PAGE_62);
  FLASH_EraseRecordPage(ADDR_FLASH_PAGE_63);
	FLASH_WriteByte((Record_WRITE_START_ADDR + 4), flash_test_bytesdata, 12);
	FLASH_ReadByte((Record_WRITE_START_ADDR + 4), flash_bytesdata, 12);
	FLASH_WriteHalfWord((Record_WRITE_START_ADDR + 520), flash_test_2bytesdata, 9);
	FLASH_ReadHalfWord((Record_WRITE_START_ADDR + 520), flash_2bytesdata, 9);
	FLASH_WriteWord((Record_WRITE_START_ADDR + 1496), flash_test_4bytesdata, 5);
	FLASH_ReadWord((Record_WRITE_START_ADDR + 1496), flash_4bytesdata, 5);
	FLASH_WriteDoubleWord((Record_WRITE_START_ADDR + 3000), flash_test_8bytesdata, 3);
  FLASH_ReadDoubleWord((Record_WRITE_START_ADDR + 3000), flash_8bytesdata, 3);
	FLASH_WriteString((Record_WRITE_START_ADDR + 4000), flash_test_string, strlen((const char *)flash_test_string));
	FLASH_ReadString((Record_WRITE_START_ADDR + 4000), flash_string, 16);
	FLASH_WriteString((Record_WRITE_START_ADDR + 8), flash_test_string, strlen((const char *)flash_test_string));
	FLASH_WriteByte((Record_WRITE_START_ADDR + 20), flash_test_bytesdata, 12);
	FLASH_WriteString((Record_WRITE_START_ADDR + 32), flash_test_string, strlen((const char *)flash_test_string));
	//FLASH_EraseAllRecordPage();
  FLASH_EraseRecordPage(ADDR_FLASH_PAGE_62);
  FLASH_EraseRecordPage(ADDR_FLASH_PAGE_63);
#elif STM32F401xx
	//FLASH_EraseAllRecordSector();
  FLASH_EraseRecordSector(ADDR_FLASH_SECTOR_4);
	FLASH_WriteByte((Record_WRITE_START_ADDR + 4), flash_test_bytesdata, 12);
	FLASH_ReadByte((Record_WRITE_START_ADDR + 4), flash_bytesdata, 12);
	FLASH_WriteHalfWord((Record_WRITE_START_ADDR + 520), flash_test_2bytesdata, 9);
	FLASH_ReadHalfWord((Record_WRITE_START_ADDR + 520), flash_2bytesdata, 9);
	FLASH_WriteWord((Record_WRITE_START_ADDR + 1496), flash_test_4bytesdata, 5);
	FLASH_ReadWord((Record_WRITE_START_ADDR + 1496), flash_4bytesdata, 5);
	FLASH_WriteDoubleWord((Record_WRITE_START_ADDR + 5000), flash_test_8bytesdata, 3);
  FLASH_ReadDoubleWord((Record_WRITE_START_ADDR + 5000), flash_8bytesdata, 3);
	FLASH_WriteString((Record_WRITE_START_ADDR + 15005), flash_test_string, strlen((const char *)flash_test_string));
	FLASH_ReadString((Record_WRITE_START_ADDR + 15005), flash_string, 16);
	//FLASH_EraseAllRecordSector();
  FLASH_EraseRecordSector(ADDR_FLASH_SECTOR_4);
#endif
	
	enableGlobalInterrupts();
	
  while(1) {
		
		if(isRX_Buf1Full == true) {
			memset((void *)CLI_Buf, 0, CmdUART_RX_BufLength);
			RX_DataCopy(CLI_Buf, (const uint8_t *)RX_Buf1, CmdUART_RX_Length);
			memset((void *)RX_Buf1, 0, CmdUART_RX_BufLength);
			isRX_Buf1Full = false;
			goto CLI_PROCESS;      //CLI_Buf處理
		} else if(isRX_Buf2Full == true) {
			memset((void *)CLI_Buf, 0, CmdUART_RX_BufLength);
			RX_DataCopy(CLI_Buf, (const uint8_t *)RX_Buf2, CmdUART_RX_Length);
			memset((void *)RX_Buf2, 0, CmdUART_RX_BufLength);
			isRX_Buf2Full = false;
			goto CLI_PROCESS;      //CLI_Buf處理
		}
		
		while(0) {
CLI_PROCESS://(可考慮把此部分寫成一個函式)
			ret = CLI_process((char *)CLI_Buf);
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
void RCC_Configuration(void) {
	
	uint16_t i;
	
	/* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
	for(i = 0 ; i < 50000 ; i++) {};
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
		
	for(i = 0 ; i < 50000 ; i++) {};
	/*!< Wait till PLL is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	/* Check if the PLL is still locked */
  if (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET) { 
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  }

#ifdef STM32F401xx
  RCC_HCLKConfig(RCC_SYSCLK_Div1);
	for(i = 0 ; i < 50000 ; i++) {};
  RCC_PCLK1Config(RCC_HCLK_Div2);
  for(i = 0 ; i < 50000 ; i++) {};
  RCC_PCLK2Config(RCC_HCLK_Div2);
  for(i = 0 ; i < 50000 ; i++) {};
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
void RX_DataCopy(uint8_t *dst, const uint8_t *sou, uint16_t length) {
	
	uint16_t count;
	
	for(count = 0 ; count < length ; count++) {
		dst[count] = sou[count];
	}
	
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * 
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
