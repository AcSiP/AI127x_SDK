/**
  ******************************************************************************
  * @file    Project/ARM-Lora/TESTcli.h 
  * @author  JC
  * @version V1.0.0
  * @date    10-Mar-2016
  * @brief   Command Line Interface program body
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TESTcli_H
#define __TESTcli_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
  #include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif

#include "shell.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int CLI_process(char *);
int CLI_ShellCmd_testPrint1(shell_cmd_args *);
int CLI_ShellCmd_testPrint2(shell_cmd_args *);



#endif /* __TESTcli_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


