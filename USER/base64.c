/**
  ******************************************************************************
  * @file    Project/ARM-Lora/base64.c 
  * @author  JC
  * @version V1.0.0
  * @date    25-Feb-2016
  * @brief   base64 program body
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "base64.h"
#include "config.h"
#include "main.h"

#ifdef STM32F072
	#include "stm32f0xx.h"
	#include "usart2.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
	#include "usart2.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BASE64_SIZE_T_MAX   ( (size_t) -1 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const uint8_t base64_enc_map[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '+', '/' };
static const uint8_t base64_dec_map[128] = {
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127,  62, 127, 127, 127,  63,  52,  53,
     54,  55,  56,  57,  58,  59,  60,  61, 127, 127,
    127,  64, 127, 127, 127,   0,   1,   2,   3,   4,
      5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
     25, 127, 127, 127, 127, 127, 127,  26,  27,  28,
     29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
     39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
     49,  50,  51, 127, 127, 127, 127, 127 };



/***************************************************************************************************
 *  Function Name: Base64_encode
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
int32_t Base64_encode( uint8_t *dst, size_t dlen, size_t *olen, const uint8_t *src, size_t slen ) {
	
	size_t i, n;
	int32_t C1, C2, C3;
	uint8_t *p;

	if( slen == 0 ) {
		*olen = 0;
		return ( 0 );
	}
	
	n = slen / 3 + ( slen % 3 != 0 );

	if( n > ( BASE64_SIZE_T_MAX - 1 ) / 4 ) {
		*olen = BASE64_SIZE_T_MAX;
		return ( ERR_BASE64_BUFFER_TOO_SMALL );
	}

	n *= 4;

	if( dlen < n + 1 ) {
		*olen = n + 1;
		return ( ERR_BASE64_BUFFER_TOO_SMALL );
	}

	n = ( slen / 3 ) * 3;

	for( i = 0, p = dst ; i < n ; i += 3 ) {
		C1 = *src++;
		C2 = *src++;
		C3 = *src++;
		
		*p++ = base64_enc_map[(C1 >> 2) & 0x3F];
		*p++ = base64_enc_map[(((C1 &  3) << 4) + (C2 >> 4)) & 0x3F];
		*p++ = base64_enc_map[(((C2 & 15) << 2) + (C3 >> 6)) & 0x3F];
		*p++ = base64_enc_map[C3 & 0x3F];
	}

	if( i < slen ) {
		C1 = *src++;
		C2 = ( ( i + 1 ) < slen ) ? *src++ : 0;
		
		*p++ = base64_enc_map[(C1 >> 2) & 0x3F];
		*p++ = base64_enc_map[(((C1 & 3) << 4) + (C2 >> 4)) & 0x3F];

		if( ( i + 1 ) < slen ) {
			*p++ = base64_enc_map[((C2 & 15) << 2) & 0x3F];
		} else {
			*p++ = '=';
		}
		
		*p++ = '=';
	}
	
	*olen = p - dst;
	*p = 0;

	return ( 0 );
		
}



/***************************************************************************************************
 *  Function Name: Base64_decode
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
int32_t Base64_decode( uint8_t *dst, size_t dlen, size_t *olen, const uint8_t *src, size_t slen ) {
	
	size_t i, n;
	uint32_t j, x;
	uint8_t *p;

	/* First pass: check for validity and get output length */
	for( i = n = j = 0; i < slen; i++ ) {
		
		/* Skip spaces before checking for EOL */
		x = 0;
		while( i < slen && src[i] == ' ' ) {
			++i;
			++x;
		}

		/* Spaces at end of buffer are OK */
		if( i == slen )
			break;

		if( ( slen - i ) >= 2 && src[i] == '\r' && src[i + 1] == '\n' )
			continue;

		if( src[i] == '\n' )
			continue;

		/* Space inside a line is an error */
		if( x != 0 )
			return ( ERR_BASE64_INVALID_CHARACTER );

		if( src[i] == '=' && ++j > 2 )
			return ( ERR_BASE64_INVALID_CHARACTER );

		if( src[i] > 127 || base64_dec_map[src[i]] == 127 )
			return ( ERR_BASE64_INVALID_CHARACTER );

		if( base64_dec_map[src[i]] < 64 && j != 0 )
			return ( ERR_BASE64_INVALID_CHARACTER );

		n++;
	}

	if( n == 0 ) {
		*olen = 0;
		return ( 0 );
	}

	n = ( ( n * 6 ) + 7 ) >> 3;
	n -= j;

	if( dst == NULL || dlen < n ) {
		*olen = n;
		return ( ERR_BASE64_BUFFER_TOO_SMALL );
	}

	for( j = 3, n = x = 0, p = dst; i > 0; i--, src++ ) {
		if( *src == '\r' || *src == '\n' || *src == ' ' )
			continue;

		j -= ( base64_dec_map[*src] == 64 );
		x  = ( x << 6 ) | ( base64_dec_map[*src] & 0x3F );

		if( ++n == 4 ) {
			n = 0;
			if( j > 0 ) *p++ = (uint8_t)( x >> 16 );
			if( j > 1 ) *p++ = (uint8_t)( x >>  8 );
			if( j > 2 ) *p++ = (uint8_t)( x );
		}
	}

	*olen = p - dst;

	return ( 0 );
	
}



/***************************************************************************************************
 *  Function Name: Base64_self_test
 *
 *  Description:
 *  Input :
 *  Output:
 *  Return:
 *  Example :
 **************************************************************************************************/
int32_t Base64_self_test( int8_t verbose ) {
	
	size_t len;
	const uint8_t *src;
	uint8_t buffer[128];
	
	const uint8_t base64_test_dec[64] = {
    0x24, 0x48, 0x6E, 0x56, 0x87, 0x62, 0x5A, 0xBD,
    0xBF, 0x17, 0xD9, 0xA2, 0xC4, 0x17, 0x1A, 0x01,
    0x94, 0xED, 0x8F, 0x1E, 0x11, 0xB3, 0xD7, 0x09,
    0x0C, 0xB6, 0xE9, 0x10, 0x6F, 0x22, 0xEE, 0x13,
    0xCA, 0xB3, 0x07, 0x05, 0x76, 0xC9, 0xFA, 0x31,
    0x6C, 0x08, 0x34, 0xFF, 0x8D, 0xC2, 0x6C, 0x38,
    0x00, 0x43, 0xE9, 0x54, 0x97, 0xAF, 0x50, 0x4B,
    0xD1, 0x41, 0xBA, 0x95, 0x31, 0x5A, 0x0B, 0x97 };
  const uint8_t base64_test_enc[] =
    "JEhuVodiWr2/F9mixBcaAZTtjx4Rs9cJDLbpEG8i7hPK"
    "swcFdsn6MWwINP+Nwmw4AEPpVJevUEvRQbqVMVoLlw==";
	
	if( verbose != 0 )
		CmdUART_UartWrite((uint8_t *)"Base64 encoding test : ", strlen("Base64 encoding test : "));
	
	src = base64_test_dec;

	if( Base64_encode( buffer, sizeof( buffer ), &len, src, 64 ) != 0 || memcmp( base64_test_enc, buffer, 88 ) != 0 ) {
		if( verbose != 0 )
			CmdUART_UartWrite((uint8_t *)"failed\r\n", strlen("failed\r\n"));

		return ( 1 );
	}
	
	if( verbose != 0 )
		CmdUART_UartWrite((uint8_t *)"passed\r\nBase64 decoding test : ", strlen("passed\r\nBase64 decoding test : "));

	src = base64_test_enc;
	
	if( Base64_decode( buffer, sizeof( buffer ), &len, src, 88 ) != 0 || memcmp( base64_test_dec, buffer, 64 ) != 0 ) {
		if( verbose != 0 )
			CmdUART_UartWrite((uint8_t *)"failed\r\n", strlen("failed\r\n"));
		
		return ( 1 );
	}
	
	if( verbose != 0 )
		CmdUART_UartWrite((uint8_t *)"passed\r\n", strlen("passed\r\n"));

	return ( 0 );
	
}



/************************ (C) COPYRIGHT Acsip ******************END OF FILE****/


