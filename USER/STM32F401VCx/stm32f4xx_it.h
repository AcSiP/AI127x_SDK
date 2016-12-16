
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
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.h 
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    21-October-2015
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_STM32F401VCX_STM32F4XX_IT_H_
#define USER_STM32F401VCX_STM32F4XX_IT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void	NMI_Handler(void);
void	HardFault_Handler(void);
void	MemManage_Handler(void);
void	BusFault_Handler(void);
void	UsageFault_Handler(void);
void	SVC_Handler(void);
void	DebugMon_Handler(void);
void	PendSV_Handler(void);
void	SysTick_Handler(void);

#ifdef STM32F401xx
#ifdef USBD_VCP_Console
void	OTG_FS_WKUP_IRQHandler( void );
void	OTG_FS_IRQHandler( void );
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif		// USER_STM32F401VCX_STM32F4XX_IT_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
