/**
  ******************************************************************************
  * @file    Project/TESTmain.c 
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
#include "TESTmain.h"

/** 
  * 
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef STM32F072
  extern __IO uint16_t USART1_RX_Length;
  extern __IO uint8_t RX_Buf1[USART1_RX_BufLength];
  extern __IO uint8_t RX_Buf2[USART1_RX_BufLength];
	uint8_t CLI_Buf[USART1_RX_BufLength];
#endif
#ifdef STM32F401xx
  extern __IO uint16_t USART2_RX_Length;
  extern __IO uint8_t RX_Buf1[USART2_RX_BufLength];
  extern __IO uint8_t RX_Buf2[USART2_RX_BufLength];
	uint8_t CLI_Buf[USART2_RX_BufLength];
#endif
extern __IO bool isRX_Buf1Full;
extern __IO bool isRX_Buf2Full;

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

  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */ 
	
	
	//system clock test
	/*
	__IO uint8_t sys_clock_source;
	RCC_ClocksTypeDef Clock;
	RCC_Configuration();
	if( SysTick_Config( SystemCoreClock / 1000 ) ) { 
		while (1);
	}
#ifdef STM32F072
	TIM14_TimerConfig();
	TIM14_TimerRunOrStop(ENABLE);
#endif
#ifdef STM32F401xx
	TIM4_TimerConfig();
	TIM4_TimerRunOrStop(ENABLE);
#endif
	GPIO_PinInitialization();
	enableGlobalInterrupts();
	sys_clock_source = RCC_GetSYSCLKSource();
	RCC_GetClocksFreq(&Clock);
	sys_clock_source += 500/2;
	Clock.SYSCLK_Frequency += 500/2;
	while(1);
	*/
	//test end
      
  /* Add your application code here */
	
	uint16_t ret;
	
	RCC_Configuration();
	
#ifdef STM32F072
	USART1_UartInit(38400);
	TIM14_TimerConfig();
#endif
#ifdef STM32F401xx
  USART2_UartInit(38400);
	TIM4_TimerConfig();
#endif

	enableGlobalInterrupts();
	
	//Base64 test
	//Base64_self_test(1);
	//Base64 test end
	
  while(1) {
		
		if(isRX_Buf1Full == true) {
#ifdef STM32F072
			memset((void *)CLI_Buf, 0, USART1_RX_BufLength);
			RX_DataCopy(CLI_Buf, (const uint8_t *)RX_Buf1, USART1_RX_Length);
			memset((void *)RX_Buf1, 0, USART1_RX_BufLength);
#endif
#ifdef STM32F401xx
			memset((void *)CLI_Buf, 0, USART2_RX_BufLength);
			RX_DataCopy(CLI_Buf, (const uint8_t *)RX_Buf1, USART2_RX_Length);
			memset((void *)RX_Buf1, 0, USART2_RX_BufLength);
#endif
			isRX_Buf1Full = false;
			goto CLI_PROCESS;      //CLI_Buf處理
		} else if(isRX_Buf2Full == true) {
#ifdef STM32F072
			memset((void *)CLI_Buf, 0, USART1_RX_BufLength);
			RX_DataCopy(CLI_Buf, (const uint8_t *)RX_Buf2, USART1_RX_Length);
			memset((void *)RX_Buf2, 0, USART1_RX_BufLength);
#endif
#ifdef STM32F401xx
			memset((void *)CLI_Buf, 0, USART2_RX_BufLength);
			RX_DataCopy(CLI_Buf, (const uint8_t *)RX_Buf2, USART2_RX_Length);
			memset((void *)RX_Buf2, 0, USART2_RX_BufLength);
#endif
			isRX_Buf2Full = false;
			goto CLI_PROCESS;      //CLI_Buf處理
		}
		
		while(0) {
CLI_PROCESS://(可考慮把此部分寫成一個函式)
			ret = CLI_process((char *)CLI_Buf);
			//藉由判斷ret來了解接收到的資料狀態,非指令(如ret回傳狀態=SHELL_PROCESS_ERR_CMD_UNKN)則當作資料傳輸出去(也要判斷lora是否有連上線)
			//目前構思每一個cli指令執行後都會回傳一個狀態(ok、err)或結果
#ifdef STM32F072
			memset((void *)CLI_Buf, 0, USART1_RX_BufLength);
			USART1_RX_Length = 0;
#endif
#ifdef STM32F401xx
			memset((void *)CLI_Buf, 0, USART2_RX_BufLength);
			USART2_RX_Length = 0;
#endif
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
	
	//RCC_HSI48Cmd(ENABLE);
	/*!< Wait till HSI is ready */
  //while(RCC_GetFlagStatus(RCC_FLAG_HSI48RDY) == RESET);
	/* Set HSI48 as sys clock*/
	//RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI48);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);

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


