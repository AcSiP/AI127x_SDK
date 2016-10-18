/**
  ******************************************************************************
  * @file    Project/ARM-Lora/rtc.h 
  * @author  JC
  * @version V1.0.0
  * @date    06-Jun-2016
  * @brief   Header for rtc.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __rtc_H
#define __rtc_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Exported define -----------------------------------------------------------*/
#define SecondOfOneTimes                (5)

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RTC_TimerConfig(void);
void RTC_AlarmConfig(void);
void RTC_AlarmRun(void);
void RTC_AlarmStop(void);
#ifdef STM32F401xx
void RTC_Alarm_IRQHandler(void);
#elif STM32F072
void RTC_IRQHandler(void);
#endif



#endif /* __rtc_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


