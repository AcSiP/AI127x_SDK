
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
 * \file       board.h
 * \brief        
 *
 * \version    1.0
 * \date       Nov 21 2012
 * \author     Miguel Luis
 */
#ifndef USER_BOARD_H_
#define USER_BOARD_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

#define SUCCESS		1
#define FAIL		0

#define USE_USB		0

#define FW_VERSION	"2.0.B2"
#define SK_NAME		"Bleeper"

/*!
 * Functions return codes definition
 */
typedef enum {
	SX_OK,
	SX_ERROR,
	SX_BUSY,
	SX_EMPTY,
	SX_DONE,
	SX_TIMEOUT,
	SX_UNSUPPORTED,
	SX_WAIT,
	SX_CLOSE,
	SX_YES,
	SX_NO
} tReturnCodes;

extern volatile uint32_t	TickCounter;

/**
  * @brief   Small printf for GCC/RAISONANCE
  */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE	int __io_putchar(int ch)

#endif /* __GNUC__ */

/*!
 * Initializes board peripherals
 */
void	BoardInit( void );
void	Board_DeInit( void );

/*!
 * Delay code execution for "delay" ms
 */
void	Delay ( uint32_t delay );

/*!
 * Delay code execution for "delay" s
 */
void	LongDelay ( uint8_t delay );

/*!
 * \brief Computes a random number between min and max
 *
 * \param [IN] min range minimum value
 * \param [IN] max range maximum value
 * \retval random random value in range min..max
 */
uint32_t	randr( uint32_t min, uint32_t max );

#endif		// USER_BOARD_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
