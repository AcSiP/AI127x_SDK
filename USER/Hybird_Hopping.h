
//---------------------------------------------------------------------------
/*
//==========================================
// Author : JC<jc@acsip.com.tw>
// Copyright 2017(C) AcSiP Technology Inc.
// 版權所有：群登科技股份有限公司
// http://www.acsip.com.tw
//==========================================
*/
//---------------------------------------------------------------------------


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef Hybird_Hopping_H_
#define Hybird_Hopping_H_

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif
#include "acsip_protocol.h"

/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define   FCC_HybirdHopping_ONorOFF           (true)        //true:turn on Hybird Hopping ; false:turn off Hybird Hopping
#define   FCC_HybirdHopping_2or4Channels      (false)         // 2:false, 4:true

#define   FCC_HybirdHopping_Frequency0_Hz     (925000000)
#define   FCC_HybirdHopping_Frequency1_Hz     (906000000)
#define   FCC_HybirdHopping_Frequency2_Hz     (920000000)
#define   FCC_HybirdHopping_Frequency3_Hz     (910000000)

//SF=10, BW=8, TX 2 TIMES
#define MaxPacketSize_Hybird_SF10_BW250_Tx2times        (30)		// SF=10, BW=250, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=242.69ms
#define MsgDataSize_Hybird_SF10_BW250_Tx2times          (21)		// 30(MaxPacketSize_Hybird_SF10_BW250) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 21
#define HybirdHopping_SF10_BW250_Tx2timesDelay          (400)

//SF=9, BW=8, TX 2 TIMES
#define MaxPacketSize_Hybird_SF9_BW250_Tx2times         (50)		// SF=9, BW=250, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=193.02ms
#define MsgDataSize_Hybird_SF9_BW250_Tx2times           (41)		// 50(MaxPacketSize_Hybird_SF9_BW250) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 41
#define HybirdHopping_SF9_BW250_Tx2timesDelay           (400)

//SF=9, BW=7, TX 2 TIMES
#define MaxPacketSize_Hybird_SF9_BW125_Tx2times         (30)		// SF=9, BW=125, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=283.65ms
#define MsgDataSize_Hybird_SF9_BW125_Tx2times           (21)		// 30(MaxPacketSize_Hybird_SF9_BW125) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 21
#define HybirdHopping_SF9_BW125_Tx2timesDelay           (400)

//SF=8, BW=8, TX 2 TIMES
#define MaxPacketSize_Hybird_SF8_BW250_Tx2times         (109)		// SF=8, BW=250, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=209.15ms
#define MsgDataSize_Hybird_SF8_BW250_Tx2times           (100)		// 109(MaxPacketSize_Hybird_SF8_BW250) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 100
#define HybirdHopping_SF8_BW250_Tx2timesDelay           (250)

//SF=8, BW=7, TX 2 TIMES
#define MaxPacketSize_Hybird_SF8_BW125_Tx2times         (61)		// SF=8, BW=125, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=254.46ms
#define MsgDataSize_Hybird_SF8_BW125_Tx2times           (52)		// 61(MaxPacketSize_Hybird_SF8_BW125) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 52
#define HybirdHopping_SF8_BW125_Tx2timesDelay           (300)

//SF=7, BW=8, TX 2 TIMES
#define MaxPacketSize_Hybird_SF7_BW250_Tx2times         (209)		// SF=7, BW=250, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=227.46ms
#define MsgDataSize_Hybird_SF7_BW250_Tx2times           (200)		// 209(MaxPacketSize_Hybird_SF7_BW250) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 200
#define HybirdHopping_SF7_BW250_Tx2timesDelay           (100)

//SF=7, BW=7, TX 2 TIMES
#define MaxPacketSize_Hybird_SF7_BW125_Tx2times         (109)		// SF=7, BW=125, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=250.11ms
#define MsgDataSize_Hybird_SF7_BW125_Tx2times           (100)		// 109(MaxPacketSize_Hybird_SF7_BW125) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 100
#define HybirdHopping_SF7_BW125_Tx2timesDelay           (250)

//SF=10, BW=8, TX 4 TIMES
#define MaxPacketSize_Hybird_SF10_BW250_Tx4times        (16)		// SF=10, BW=250, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=181.25ms
#define MsgDataSize_Hybird_SF10_BW250_Tx4times          (7)	  	// 16(MaxPacketSize_Hybird_SF10_BW250) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 7
#define HybirdHopping_SF10_BW250_Tx4timesDelay          (400)

//SF=9, BW=8, TX 4 TIMES
#define MaxPacketSize_Hybird_SF9_BW250_Tx4times         (25)		// SF=9, BW=250, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=121.34ms
#define MsgDataSize_Hybird_SF9_BW250_Tx4times           (16)		// 25(MaxPacketSize_Hybird_SF9_BW250) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 16
#define HybirdHopping_SF9_BW250_Tx4timesDelay           (400)

//SF=9, BW=7, TX 4 TIMES
#define MaxPacketSize_Hybird_SF9_BW125_Tx4times         (16)		// SF=9, BW=125, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=201.73ms
#define MsgDataSize_Hybird_SF9_BW125_Tx4times           (7)		  // 16(MaxPacketSize_Hybird_SF9_BW125) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 7
#define HybirdHopping_SF9_BW125_Tx4timesDelay           (400)

//SF=8, BW=8, TX 4 TIMES
#define MaxPacketSize_Hybird_SF8_BW250_Tx4times         (54)		// SF=8, BW=250, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=116.99ms
#define MsgDataSize_Hybird_SF8_BW250_Tx4times           (45)		// 54(MaxPacketSize_Hybird_SF8_BW250) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 45
#define HybirdHopping_SF8_BW250_Tx4timesDelay           (250)

//SF=8, BW=7, TX 4 TIMES
#define MaxPacketSize_Hybird_SF8_BW125_Tx4times         (30)		// SF=8, BW=125, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=152.06ms
#define MsgDataSize_Hybird_SF8_BW125_Tx4times           (21)		// 30(MaxPacketSize_Hybird_SF8_BW125) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 21
#define HybirdHopping_SF8_BW125_Tx4timesDelay           (300)

//SF=7, BW=8, TX 4 TIMES
#define MaxPacketSize_Hybird_SF7_BW250_Tx4times         (104)		// SF=7, BW=250, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=119.94ms
#define MsgDataSize_Hybird_SF7_BW250_Tx4times           (95)		// 104(MaxPacketSize_Hybird_SF7_BW250) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 95
#define HybirdHopping_SF7_BW250_Tx4timesDelay           (100)

//SF=7, BW=7, TX 4 TIMES
#define MaxPacketSize_Hybird_SF7_BW125_Tx4times         (54)		// SF=7, BW=125, ImplicitHeaderOn=1, CodingRate=4/5, PreambleLength=12, TimeOnAir=137.47ms
#define MsgDataSize_Hybird_SF7_BW125_Tx4times           (45)		// 109(MaxPacketSize_Hybird_SF7_BW125) - 6(Length:1、Flag:1、NodeAdd:3、NextHoppingChannel:1) - 3(MyAdd) = 100
#define HybirdHopping_SF7_BW125_Tx4timesDelay           (250)

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void HybirdHopping_SetBack(void);
void HybirdHopping_Initialization(bool);
bool HybirdHopping_TX(void);
void HybirdHopping_SetRXChannel(void);



#endif		// Hybird_Hopping_H_



/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/


