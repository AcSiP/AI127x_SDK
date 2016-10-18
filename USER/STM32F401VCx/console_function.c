/**
  ******************************************************************************
  * @file    Project/ARM-Lora/console_function.c
  * @author  JC
  * @version V1.0.0
  * @date    24-May-2016
  * @brief   console_function program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "config.h"
#include "usart2.h"
#include "usbd_cdc_vcp.h"
#include "console_function.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Const variables -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef STM32F401xx
#ifdef USBD_VCP_Console
__IO bool UsbDegugOn = true;
#endif
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/***************************************************************************************************
 *  Function Name: ConsoleFunction_CmdOutput
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
void ConsoleFunction_CmdOutput(uint8_t *Str, uint32_t Length) {
  
  USART2_UartWrite(Str, Length);
  
#ifdef STM32F401xx
#ifdef USBD_VCP_Console
  if(UsbDegugOn == true) {
    VCP_DataTx(Str, Length);
  }
#endif
#endif
  
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


