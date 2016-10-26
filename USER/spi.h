
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

/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       spi.h
 * \brief      SPI hardware driver
 *
 * \version    1.0
 * \date       Feb 12 2010
 * \author     Miguel Luis
 */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef USER_SPI_H_
#define USER_SPI_H_

/* Includes ------------------------------------------------------------------*/

#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

#if ( STM32F4XX )
	#include "stm32f4xx_spi.h"
#elif defined( STM32F2XX )
	#include "stm32f2xx_spi.h"
#elif defined( STM32F1XX )
	#include "stm32f10x_spi.h"
#elif defined( STM32F072 )
	#include "stm32f0xx_spi.h"
#elif defined( STM32F401xx )
	#include "stm32f4xx_spi.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void		SpiInit( void );
uint8_t		SpiInOut( uint8_t outData );

#endif		// USER_SPI_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
