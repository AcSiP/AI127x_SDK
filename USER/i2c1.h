
//---------------------------------------------------------------------------
/*
//==========================================
// Author : Fox<shenfa@acsip.com.tw>
// Copyright 2016(C) AcSiP Technology Inc.
// 版權所有：群登科技股份有限公司
// http://www.acsip.com.tw
//==========================================
*/
//---------------------------------------------------------------------------

  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef H__STM32F4__I2C1__H
#define H__STM32F4__I2C1__H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

#include "stm32f4xx.h"


/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define MCU_I2C__TIMEOUT_MS		200

#define MCU_I2C__MASTER			I2C1
#define MCU_I2C__CLK			RCC_APB1Periph_I2C1

#define MCU_I2C__SDA_PIN		GPIO_Pin_9
#define MCU_I2C__SDA_SOURCE		GPIO_PinSource9
#define MCU_I2C__SDA_PORT		GPIOB
#define MCU_I2C__SDA_CLK		RCC_AHB1Periph_GPIOB
#define MCU_I2C__SDA_AF			GPIO_AF_I2C1

#define MCU_I2C__SCL_PIN		GPIO_Pin_8
#define MCU_I2C__SCL_SOURCE		GPIO_PinSource8
#define MCU_I2C__SCL_PORT		GPIOB
#define MCU_I2C__SCL_CLK		RCC_AHB1Periph_GPIOB
#define MCU_I2C__SCL_AF			GPIO_AF_I2C1




/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void	I2C_Master__Init( uint8_t device_address );
bool	I2C_Master__Write( uint8_t Dev_Addr, uint8_t Reg_Addr, uint16_t NumByteToWrite, const uint8_t * pBuffer );
bool	I2C_Master__Write_Raw( uint8_t Dev_Addr, uint16_t NumByteToWrite, const uint8_t * pBuffer );
bool	I2C_Master__Read( uint8_t Dev_Addr, uint8_t Reg_Addr, uint16_t NumByteToRead, uint8_t * pBuffer, uint16_t Wait_Before_Read_mS );
bool	I2C_Master__Read_Raw( uint8_t Dev_Addr, uint16_t NumByteToRead, uint8_t * pBuffer );
void Configure_IRQ_PA7(void) ;
/* Exported variables ------------------------------------------------------------*/



#endif		/* H__STM32F4__I2C1__H */


/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/


