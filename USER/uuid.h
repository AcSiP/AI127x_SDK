/**
  ******************************************************************************
  * @file    Project/ARM-Lora/uuid.h 
  * @author  JC
  * @version V1.0.0
  * @date    21-Mar-2016
  * @brief   UUID program body
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UUID_H
#define __UUID_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
  #include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif

/* Private define ------------------------------------------------------------*/
#ifdef STM32F072
	#define UUID__FLASH_ADDR        ((uint32_t)0x1FFFF7AC)
#endif
#ifdef STM32F401xx
	#define UUID__FLASH_ADDR        ((uint32_t)0x1FFF7A10)
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UUID_Read(uint32_t *);
void UUID_ReadLeast3Bytes(uint8_t *);



#endif /* __UUID_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


