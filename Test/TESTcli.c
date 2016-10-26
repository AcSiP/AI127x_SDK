/**
  ******************************************************************************
  * @file    Project/ARM-Lora/TESTcli.c 
  * @author  JC
  * @version V1.0.0
  * @date    10-Mar-2016
  * @brief   Command Line Interface program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#ifdef STM32F072
  #include "stm32f0xx.h"
	#include "usart1.h"
#endif

#ifdef STM32F401xx
  #include "stm32f4xx.h"
	#include "usart2.h"
#endif
#include "shell.h"
#include "TESTcli.h"
#include "config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
shell_cmds LORA_shell_cmds = {
	.count = 2,
	.cmds = {
		{
			.cmd	= "testPrint1",
			.desc	= "Shell command for the test1",
			.func	= CLI_ShellCmd_testPrint1,
		},
		
		{
			.cmd	= "testPrint2",
			.desc	= "Shell command for the test2",
			.func	= CLI_ShellCmd_testPrint2,
		},
	},
}; 

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/***************************************************************************************************
 *  Function Name: CLI_process
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
int CLI_process(char *cmd_line) {
	
	int ret = shell_process_cmds(&LORA_shell_cmds, cmd_line);
	
	switch(ret) {
		case SHELL_PROCESS_OK:
			// is OK
#ifdef STM32F072
		  USART1_UartWrite((uint8_t *)okCMD, strlen(okCMD));
#endif
#ifdef STM32F401xx
		  USART2_UartWrite((uint8_t *)okCMD, strlen(okCMD));
#endif
			break;
    case SHELL_PROCESS_ERR_CMD_UNKN:
        // Unknown command
#ifdef STM32F072
		  USART1_UartWrite((uint8_t *)unkownCMD, strlen(unkownCMD));
#endif
#ifdef STM32F401xx
		  USART2_UartWrite((uint8_t *)unkownCMD, strlen(unkownCMD));
#endif
        break;
    case SHELL_PROCESS_ERR_ARGS_LEN:
        // Argument to long
#ifdef STM32F072
		  USART1_UartWrite((uint8_t *)errorCMD, strlen(errorCMD));
#endif
#ifdef STM32F401xx
		  USART2_UartWrite((uint8_t *)errorCMD, strlen(errorCMD));
#endif
        break;
    case SHELL_PROCESS_ERR_ARGS_MAX:
        // Too many arguments
#ifdef STM32F072
		  USART1_UartWrite((uint8_t *)errorCMD, strlen(errorCMD));
#endif
#ifdef STM32F401xx
		  USART2_UartWrite((uint8_t *)errorCMD, strlen(errorCMD));
#endif
        break;
    default:
        // OK
        break;
    }
	
    return ret;
		
}



/***************************************************************************************************
 *  Function Name: CLI_ShellCmd_testPrint1
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
int CLI_ShellCmd_testPrint1(shell_cmd_args *args) {
	
	#ifdef STM32F072
    USART1_UartWrite((uint8_t *)"\r\ntestPrint1\r\n", 14);
	#endif
	#ifdef STM32F401xx
    USART2_UartWrite((uint8_t *)"\r\ntestPrint1\r\n", 14);
  #endif
	
	return 0;
	
}



/***************************************************************************************************
 *  Function Name: CLI_ShellCmd_testPrint2
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
int CLI_ShellCmd_testPrint2(shell_cmd_args *args) {
	
	#ifdef STM32F072
    USART1_UartWrite((uint8_t *)"\r\ntestPrint2\r\n", 14);
	#endif
	#ifdef STM32F401xx
    USART2_UartWrite((uint8_t *)"\r\ntestPrint2\r\n", 14);
  #endif
	
	return 0;
	
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


