
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
  * @file    Project/ARM-Lora/cli.c 
  * @author  JC
  * @version V1.0.0
  * @date    19-Jan-2016
  * @brief   Command Line Interface program body
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
#include <ctype.h>
#include "shell.h"
#include "cli.h"
#include "config.h"
#include "radio.h"
#include "sx1276-LoRaMisc.h"
#include "sx1276-LoRa.h"
#include "sx1276-FskMisc.h"
#include "sx1276-Fsk.h"
#include "sx1276.h"
#include "base64.h"
#include "uuid.h"
#include "main.h"
#include "spi.h"
#include "UART_Console.h"
#include "acsip_protocol.h"
#include "Base_Driver__ADC1.h"
#include "flash.h"

#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern bool				LoraStartWork;
extern bool				EnableMaster;		// Master/Slave selection
extern tRadioDriver *			Radio;
extern tLoRaSettings			LoRaSettings;
extern __IO uint32_t			SystemOperMode;
extern uint8_t				LoraOperationMode;
extern __IO bool			LoraNeedTxData;
extern size_t				LoraTxPayloadSize;
extern uint8_t				LoraTxBuffer[LoraBufferLength];
extern __IO uint32_t			LoraPV_RxCount;
extern __IO bool			LoraPV_RxTest;
extern __IO bool			LoraPV_TxTest;
extern __IO uint32_t			LoraPV_TxCount;
extern __IO bool			LoraPV_DefaultTxSet;
extern __IO uint32_t			LoraPV_TxTimes;
extern __IO bool			LoRaOn;
extern __IO bool			GPS_HandmadeOff;
extern int16_t				RF__CW_Offset;


extern tLoraDeviceNode *		LoraNodeDevice[MAX_LoraNodeNum];			// for MASTER
extern tDeviceNodeSleepAndRandomHop *	DeviceNodeSleepAndRandomHop[MAX_LoraNodeNum];		// for MASTER
extern tDeviceNodeSensor *		DeviceNodeSensor[MAX_LoraNodeNum];			// for MASTER
extern uint8_t				LoraNodeCount;						// for MASTER
extern tLoraDeviceNode *		LoraGateWay;						// for SLAVE
extern tDeviceNodeSensor *		MySensor;						// for SLAVE
extern __IO uint16_t 			Running_TimeCount;					// for MASTER & SLAVE


extern tFskSettings			FskSettings;
__IO bool				FskPV_TxTest = false;
uint8_t					TxDataArray[MaxMsgDataSize];
uint8_t					TxDataSize;

#ifdef STM32F072
const uint8_t		GPIO_Port[] = {'A', 'B', 'C', 'F'};
const uint16_t		PortAB_Pin[] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3,
					GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7,
					GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11,
					GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};
const uint16_t		PortC_Pin[] = {GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};
const uint16_t		PortOther_Pin[] = {GPIO_Pin_0, GPIO_Pin_1};		// PortOther = PortF
#endif

#ifdef STM32F401xx
const uint8_t		GPIO_Port[] = {'A', 'B', 'C', 'H'};
const uint16_t		PortAB_Pin[] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3,
					GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7,
					GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11,
					GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};
					// in STM32F401CB(package=WLCSP49) Port B not have Pin11
const uint16_t		PortC_Pin[] = {GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};
const uint16_t		PortOther_Pin[] = {GPIO_Pin_0, GPIO_Pin_1};		// PortOther = PortH
#endif

shell_cmd		CLI__Shell_Command[] = {
	{	.cmd	= "GetBatteryADC",		.func	= CLI_ShellCmd_GetBatteryADC,		.desc	= "Get Conversion Value and Conversion Voltage"		},
	{	.cmd	= "SystemReset",		.func	= CLI_ShellCmd_SystemReset,		.desc	= "Device System Reset"					},
	{	.cmd	= "GPIO_Set",			.func	= CLI_ShellCmd_GPIO_Set,		.desc	= "GPIO Set for PV"					},
	{	.cmd	= "GPIO_Write",			.func	= CLI_ShellCmd_GPIO_Write,		.desc	= "GPIO Write for PV"					},
	{	.cmd	= "GPIO_Read",			.func	= CLI_ShellCmd_GPIO_Read,		.desc	= "GPIO Read for PV"					},

	{	.cmd	= "CmdPortOff",			.func	= CLI_ShellCmd_CmdPortOff,		.desc	= "Close Command USART Port"				},
	{	.cmd	= "FskPV_TXstart",		.func	= CLI_ShellCmd_FskPV_TXstart,		.desc	= "sx1276/78 FSK TX Product Verification"		},
	{	.cmd	= "FskPV_TXstop",		.func	= CLI_ShellCmd_FskPV_TXstop,		.desc	= "sx1276/78 FSK TX Product Verification"		},
	{	.cmd	= "FskPV_Freq",			.func	= CLI_ShellCmd_FskPV_Freq,		.desc	= "sx1276/78 FSK TX Product Verification"		},

	{	.cmd	= "LoraPV_TXresult",		.func	= CLI_ShellCmd_LoraPV_TXresult,		.desc	= "sx1276/78 LORA TX Product Verification (ask result)"	},
	{	.cmd	= "LoraPV_TXstart",		.func	= CLI_ShellCmd_LoraPV_TXstart,		.desc	= "sx1276/78 LORA TX Product Verification"		},
	{	.cmd	= "LoraPV_RXresult",		.func	= CLI_ShellCmd_LoraPV_RXresult,		.desc	= "sx1276/78 LORA RX Product Verification (ask result)"	},
	{	.cmd	= "LoraPV_RXstart",		.func	= CLI_ShellCmd_LoraPV_RXstart,		.desc	= "sx1276/78 LORA RX Product Verification"		},
	{	.cmd	= "LoraJoinNode",		.func	= CLI_ShellCmd_LoraJoinNode,		.desc	= "Acsip Protocol LORA base Join LoraNet"		},

	{	.cmd	= "LoraForceJoinNode",		.func	= CLI_ShellCmd_LoraJoinNode_in_Force,	.desc	= "Acsip Protocol LORA base Join LoraNet in Force"	},
	{	.cmd	= "LoraLeaveNode",		.func	= CLI_ShellCmd_LoraLeaveNode,		.desc	= "Acsip Protocol LORA base Leave LoraNet"		},
	{	.cmd	= "LoraForceLeaveNode",		.func	= CLI_ShellCmd_LoraLeaveNode_in_Force,	.desc	= "Acsip Protocol LORA base Leave LoraNet"		},
	{	.cmd	= "LoraSetNodePara",		.func	= CLI_ShellCmd_LoraSetNodePara,		.desc	= "Set Acsip Protocol LORA node parameter"		},
	{	.cmd	= "LoraGetNodePara",		.func	= CLI_ShellCmd_LoraGetNodePara,		.desc	= "Get Acsip Protocol LORA node parameter"		},

	{	.cmd	= "LoraNodeData",		.func	= CLI_ShellCmd_LoraNodeData,		.desc	= "Acsip Protocol LORA base TX packet"			},
	{	.cmd	= "LoraListNodes",		.func	= CLI_ShellCmd_LoraGetAllNodeAddr,	.desc	= "Acsip Protocol LORA base for All Node address"	},
	{	.cmd	= "LoraGetGateWayAddr",		.func	= CLI_ShellCmd_LoraGetGateWayAddr,	.desc	= "Acsip Protocol LORA base for GateWay address"	},
	{	.cmd	= "OutputTraceData",		.func	= CLI_ShellCmd_OutputTraceData,		.desc	= "Output All Trace Data"				},
	{	.cmd	= "FormatTraceDataSpace",	.func	= CLI_ShellCmd_FormatTraceDataSpace,	.desc	= "Format Trace Data Space"				},

#ifdef Board__A22_Tracker
	{	.cmd	= "BleEnable",			.func	= CLI_ShellCmd_BleEnable,		.desc	= "Bluetooth Enable"					},
	{	.cmd	= "BleDisable",			.func	= CLI_ShellCmd_BleDisable,		.desc	= "Bluetooth Disable"					},
	{	.cmd	= "GpsEnable",			.func	= CLI_ShellCmd_GpsEnable,		.desc	= "GPS Enable"						},
	{	.cmd	= "GpsDisable",			.func	= CLI_ShellCmd_GpsDisable,		.desc	= "GPS Disable"						},
#endif

	{	.cmd	= "LoraGetMyAddr",		.func	= CLI_ShellCmd_LoraGetMyAddr,		.desc	= "Acsip Protocol LORA base for My address"		},
	{	.cmd	= "LoraGetMyInterval",		.func	= CLI_ShellCmd_LoraGetMyInterval,	.desc	= "Sleep Interval"					},
	{	.cmd	= "GetUUID",			.func	= CLI_ShellCmd_GetUUID,			.desc	= "MCU module base for UUID"				},
	{	.cmd	= "GetFWVersion",		.func	= CLI_ShellCmd_GetFWVersion,		.desc	= "MCU module base for Firmware Version"		},
	{	.cmd	= "LoraGetSystemMode",		.func	= CLI_ShellCmd_LoraGetSystemMode,	.desc	= "System Mode"						},

	{	.cmd	= "LoraGetMode",		.func	= CLI_ShellCmd_LoraGetMode,		.desc	= "sx1276/78 LORA Mode"					},
	{	.cmd	= "LoraGetPara",		.func	= CLI_ShellCmd_LoraGetPara,		.desc	= "sx1276/78 LORA RF parameter"				},

	{	.cmd	= "LoraGetRFOsc",		.func	= CLI_ShellCmd_LoraGetRFOsc,		.desc	= "sx1276/78 LORA RF Oscillator"			},
	{	.cmd	= "LoraGetRFFrequency",		.func	= CLI_ShellCmd_LoraGetRFFrequency,	.desc	= "sx1276/78 LORA RF Frequency"				},
	{	.cmd	= "LoraGetRxPacketTimeout",	.func	= CLI_ShellCmd_LoraGetRxPacketTimeout,	.desc	= "sx1276/78 LORA Rx Packet Timeout"			},
	{	.cmd	= "LoraGetTxPacketTimeout",	.func	= CLI_ShellCmd_LoraGetTxPacketTimeout,	.desc	= "sx1276/78 LORA Tx Packet Timeout"			},
	{	.cmd	= "LoraGetRFPower",		.func	= CLI_ShellCmd_LoraGetRFPower,		.desc	= "sx1276/78 LORA RF Power"				},

	{	.cmd	= "LoraGetPreambleLength",	.func	= CLI_ShellCmd_LoraGetPreambleLength,	.desc	= "sx1276/78 LORA packet Preamble Length"		},
	{	.cmd	= "LoraGetPayloadLength",	.func	= CLI_ShellCmd_LoraGetPayloadLength,	.desc	= "sx1276/78 LORA packet Payload Length"		},
	{	.cmd	= "LoraGetCRC",			.func	= CLI_ShellCmd_LoraGetCRC,		.desc	= "sx1276/78 LORA packet CRC"				},
	{	.cmd	= "LoraGetImplicitHeader",	.func	= CLI_ShellCmd_LoraGetImplicitHeader,	.desc	= "sx1276/78 LORA packet Header Implicit"		},
//	{	.cmd	= "LoraGetFreqHopOn",		.func	= CLI_ShellCmd_LoraGetFreqHopOn,	.desc	= "sx1276/78 LORA Freq. Hop On"				},

	{	.cmd	= "LoraGetErrorCoding",		.func	= CLI_ShellCmd_LoraGetErrorCoding,	.desc	= "sx1276/78 LORA packet Error Coding"			},
	{	.cmd	= "LoraGetSF",			.func	= CLI_ShellCmd_LoraGetSF,		.desc	= "sx1276/78 LORA Spreading Factor"			},
	{	.cmd	= "LoraGetBW",			.func	= CLI_ShellCmd_LoraGetBW,		.desc	= "sx1276/78 RF band width"				},
	{	.cmd	= "LoraGetRxGain",		.func	= CLI_ShellCmd_LoraGetRxGain,		.desc	= "sx1276/78 Rx Gain value"				},
	{	.cmd	= "LoraGetSNR",			.func	= CLI_ShellCmd_LoraGetSNR,		.desc	= "sx1276/78 SNR value"					},

	{	.cmd	= "LoraGetRSSI",		.func	= CLI_ShellCmd_LoraGetRSSI,		.desc	= "sx1276/78 RSSI value"				},
	{	.cmd	= "FactoryReset",		.func	= CLI_ShellCmd_FactoryReset,		.desc	= "System Factory Reset"				},
	{	.cmd	= "LoraSystemMode",		.func	= CLI_ShellCmd_LoraSystemMode,		.desc	= "System operation mode"				},

	{	.cmd	= "LoraSF7BW250RS120",		.func	= CLI_ShellCmd_LoraSF7BW250RS120,	.desc	= "sx1276/78 RX sensitivity = 120dBm"			},
	{	.cmd	= "LoraSF7BW125RS123",		.func	= CLI_ShellCmd_LoraSF7BW125RS123,	.desc	= "sx1276/78 RX sensitivity = 123dBm"			},
	{	.cmd	= "LoraSF8BW500RS120",		.func	= CLI_ShellCmd_LoraSF8BW500RS120,	.desc	= "sx1276/78 RX sensitivity = 120dBm"			},
	{	.cmd	= "LoraSF8BW250RS123",		.func	= CLI_ShellCmd_LoraSF8BW250RS123,	.desc	= "sx1276/78 RX sensitivity = 123dBm"			},
	{	.cmd	= "LoraSF8BW125RS126",		.func	= CLI_ShellCmd_LoraSF8BW125RS126,	.desc	= "sx1276/78 RX sensitivity = 126dBm"			},

	{	.cmd	= "LoraSF9BW500RS123",		.func	= CLI_ShellCmd_LoraSF9BW500RS123,	.desc	= "sx1276/78 RX sensitivity = 123dBm"			},
	{	.cmd	= "LoraSF9BW250RS126",		.func	= CLI_ShellCmd_LoraSF9BW250RS126,	.desc	= "sx1276/78 RX sensitivity = 126dBm"			},
	{	.cmd	= "LoraSF9BW125RS129",		.func	= CLI_ShellCmd_LoraSF9BW125RS129,	.desc	= "sx1276/78 RX sensitivity = 129dBm"			},
	{	.cmd	= "LoraSF10BW500RS126",		.func	= CLI_ShellCmd_LoraSF10BW500RS126,	.desc	= "sx1276/78 RX sensitivity = 126dBm"			},
	{	.cmd	= "LoraSF10BW250RS129",		.func	= CLI_ShellCmd_LoraSF10BW250RS129,	.desc	= "sx1276/78 RX sensitivity = 129dBm"			},
	{	.cmd	= "LoraSF10BW125RS132",		.func	= CLI_ShellCmd_LoraSF10BW125RS132,	.desc	= "sx1276/78 RX sensitivity = 132dBm"			},

	{	.cmd	= "LoraSF11BW250RS132",		.func	= CLI_ShellCmd_LoraSF11BW250RS132,	.desc	= "sx1276/78 RX sensitivity = 132dBm"			},
	{	.cmd	= "LoraSF11BW125RS135",		.func	= CLI_ShellCmd_LoraSF11BW125RS135,	.desc	= "sx1276/78 RX sensitivity = 135dBm"			},
	{	.cmd	= "LoraSF12BW250RS134",		.func	= CLI_ShellCmd_LoraSF12BW250RS134,	.desc	= "sx1276/78 RX sensitivity = 134dBm"			},
	{	.cmd	= "LoraSF12BW125RS137",		.func	= CLI_ShellCmd_LoraSF12BW125RS137,	.desc	= "sx1276/78 RX sensitivity = 137dBm"			},

	{	.cmd	= "LoraFreq",			.func	= CLI_ShellCmd_LoraFreq,		.desc	= "sx1276/78 RF Frequency"				},
	{	.cmd	= "LoraBW",			.func	= CLI_ShellCmd_LoraBW,			.desc	= "sx1276/78 Band Width"				},
	{	.cmd	= "LoraSF",			.func	= CLI_ShellCmd_LoraSF,			.desc	= "sx1276/78 Spreading Factor"				},
	{	.cmd	= "LoraErrorCoding",		.func	= CLI_ShellCmd_LoraErrorCoding,		.desc	= "sx1276/78 Error Coding Rate"				},
	{	.cmd	= "LoraFreqHop",		.func	= CLI_ShellCmd_LoraFreqHop,		.desc	= "sx1276/78 freq hop on or off"			},

//	{	.cmd	= "LoraFreqHopOn",		.func	= CLI_ShellCmd_LoraFreqHopOn,		.desc	= "sx1276/78 freq hop on or off"			},
	{	.cmd	= "LoraImplicitHeader",		.func	= CLI_ShellCmd_LoraImplicitHeader,	.desc	= "sx1276/78 packet Implicit header"			},
	{	.cmd	= "LoraCRC",			.func	= CLI_ShellCmd_LoraCRC,			.desc	= "sx1276/78 packet crc on or off"			},

	{	.cmd	= "ListHopChannelList",		.func	= CLI_ShellCmd__List_Hop_Channel_List,	.desc	= "List FHSS channel list"				},
	{	.cmd	= "ClearHopChannelList",	.func	= CLI_ShellCmd__Clear_Hop_Channel_List,	.desc	= "Clear all FHSS channels"				},
	{	.cmd	= "AppendHopChannel",		.func	= CLI_ShellCmd__Append_Hop_Channel,	.desc	= "Append a FHSS channel into list"			},

	{	.cmd	= "LoraPayloadLength",		.func	= CLI_ShellCmd_LoraPayloadLength,	.desc	= "sx1276/78 payload length"				},
	{	.cmd	= "LoraMaxPayloadLength",	.func	= CLI_ShellCmd_MaxLoraPayloadLength,	.desc	= "sx1276/78 max. payload length"			},
	{	.cmd	= "LoraPreambleLength",		.func	= CLI_ShellCmd_LoraPreambleLength,	.desc	= "sx1276/78 preamble length"				},
	{	.cmd	= "LoraMode",			.func	= CLI_ShellCmd_LoraMode,		.desc	= "sx1276/78 operation mode"				},
	{	.cmd	= "LoraStartWork",		.func	= CLI_ShellCmd_LoraStartWork,		.desc	= "sx1276/78 start work"				},

	{	.cmd	= "LoraPower",			.func	= CLI_ShellCmd_LoraPower,		.desc	= "set LORA output power"				},
	{	.cmd	= "UartEchoOn",			.func	= CLI_ShellCmd_UartEchoOn,		.desc	= "URAT echo on"					},
	{	.cmd	= "UartEchoOff",		.func	= CLI_ShellCmd_UartEchoOff,		.desc	= "URAT echo off"					},
	{	.cmd	= "SX127x_NRESET",		.func	= CLI_ShellCmd__SX127x_NRESET,		.desc	= "Manual control level of NRESET"			},
	{	.cmd	= "LoraReset",			.func	= CLI_LoraReInit,			.desc	= "Manual ReInit SX127x"				},

	{	.cmd	= "SX127x_OpMode",		.func	= CLI_ShellCmd__SX127x_OpMode,		.desc	= "Manual change SX127x mode"				},
	{	.cmd	= "SleepStandby",		.func	= CLI_ShellCmd__System_Standby,		.desc	= "Manual sleep mode"					},
	{	.cmd	= "SleepStop",			.func	= CLI_ShellCmd__System_Stop,		.desc	= "Manual sleep mode"					},
	{	.cmd	= "Read_CW_Offset",		.func	= CLI_ShellCmd__Read_CW_Offset,		.desc	= "Get calibrated offset of Carrier Wave"		},
	{	.cmd	= "Write_CW_Offset",		.func	= CLI_ShellCmd__Write_CW_Offset,	.desc	= "Set calibrated offset of Carrier Wave"		},


	{	.cmd	= "GetDeviceInfo",		.func	= CLI_ShellCmd__Read_Device_Info,	.desc	= "MCU module Info"					},
	{	.cmd	= "SX127x_DumpRegs",		.func	= CLI_ShellCmd__Dump_SX127x_Regs,	.desc	= "Dump Registers of SX127x"				},

	{	.cmd	= "Dump_OTP",			.func	= CLI_ShellCmd__Dump_STM32F401_OTP,	.desc	= "Dump OTP of STM32F401"				},
	{	.cmd	= "Write_OTP",			.func	= CLI_ShellCmd__Write_OTP,		.desc	= "Write single byte into OTP on STM32F401"		},

	{	.cmd	= "testPrint1",			.func	= CLI_ShellCmd_testPrint1,		.desc	= "Shell command for the test1"				},
	{	.cmd	= "testPrint2",			.func	= CLI_ShellCmd_testPrint2,		.desc	= "Shell command for the test2"				}
};

/* Private function prototypes -----------------------------------------------*/
void	CLI__Save_LoRa_Configure_into_Flash( void );
void	CLI__Console_Output_Node_Address( const uint8_t *addr );

/* Private functions ---------------------------------------------------------*/


int	CLI_process( char *cmd_line )
{
	uint8_t		n = sizeof( CLI__Shell_Command ) / sizeof( shell_cmd );
	int		ret;

	Console_Output_String( "\r\n" );

	ret = shell_process_cmds( CLI__Shell_Command, n, cmd_line );
	switch( ret ) {
	case SHELL_PROCESS_OK:
		// is OK
		Console_Output_String( okCMD );
		break;

	case SHELL_PROCESS_ERR_CMD_UNKN:
		// Unknown command
		Console_Output_String( errorCMD );
		break;

	case SHELL_PROCESS_ERR_ARGS_LEN:
		// Argument to long
		Console_Output_String( errorCMD );
		break;

	case SHELL_PROCESS_ERR_ARGS_MAX:
		// Too many arguments
		Console_Output_String( errorCMD );
		break;

	default:
		// OK
		break;
	}

	return ret;
}


uint32_t	CLI_LoraTimeOutCalculate( tLoRaSettings *loraSettings )
{
	double		Ts, Tpreamble, Tpayload;
	uint32_t	Npayload, num1, num2, num3;
	uint8_t		crc, ih, de;

	switch( loraSettings->SignalBw ) {
		case 0:		Ts = (double)(pow(2, loraSettings->SpreadingFactor) / 7800);		break;
		case 1:		Ts = (double)(pow(2, loraSettings->SpreadingFactor) / 10400);		break;
		case 2:		Ts = (double)(pow(2, loraSettings->SpreadingFactor) / 15600);		break;
		case 3:		Ts = (double)(pow(2, loraSettings->SpreadingFactor) / 20800);		break;
		case 4:		Ts = (double)(pow(2, loraSettings->SpreadingFactor) / 31250);		break;
		case 5:		Ts = (double)(pow(2, loraSettings->SpreadingFactor) / 41700);		break;
		case 6:		Ts = (double)(pow(2, loraSettings->SpreadingFactor) / 62500);		break;
		case 7:		Ts = (double)(pow(2, loraSettings->SpreadingFactor) / 125000);		break;
		case 8:		Ts = (double)(pow(2, loraSettings->SpreadingFactor) / 250000);		break;
		case 9:		Ts = (double)(pow(2, loraSettings->SpreadingFactor) / 500000);		break;
		default:	break;
	}

	if( loraSettings->CrcOn ) {
		crc = 1;
	} else {
		crc = 0;
	}

	if( loraSettings->ImplicitHeaderOn ) {
		ih = 1;
	} else {
		ih = 0;
	}

	if( loraSettings->LowDataRateOptimize ) {
		de = 1;
	} else {
		de = 0;
	}

	num1 = ((8 * loraSettings->MaxPayloadLength) - (4 * loraSettings->SpreadingFactor) + 28 + (16 * crc) - (20 * ih));
	num2 = (4 * (loraSettings->SpreadingFactor - (2 * de)));
	num3 = (uint32_t)(ceil((double)num1/num2));
	Npayload = 8 + (num3 * (4 + loraSettings->ErrorCoding));

	Tpreamble = ((loraSettings->PreambleLength + 4.25) * Ts ) * 1000;		// ms
	Tpayload = (Npayload * Ts) * 1000;		// ms

	return (uint32_t)( ceil(Tpreamble + Tpayload) + 500 );		// ms
}


int	CLI_LoraReInit( shell_cmd_args *args )
{
/*
	uint32_t	delay;

	SPI_Cmd(SPI_INTERFACE, DISABLE);
	SPI_I2S_DeInit(SPI_INTERFACE);

	delay = GET_TICK_COUNT( );
	while( ( GET_TICK_COUNT( ) - delay ) < TICK_RATE_MS( 300 ) ) {
	}

	SpiInit();
	Radio->Init();
	SX1276LoRaSetHopPeriod(Lora_RFHoppingPeriod);
	Radio->StartRx();
*/
	LoraPara_LoadAndConfiguration();
	Radio->Init();
	Radio->StartRx();
	return SHELL_PROCESS_OK;
}


void	CLI_FactoryReset( void )
{
	uint32_t	count;

	CLI_DeleteAllLoraNodeMemorySpace();
	CLI_DeleteLoraGateWayMemorySpace();
	SaveRecord_ClearAllSaveRecordSpace();
	count = GET_TICK_COUNT( );
	while( ( GET_TICK_COUNT( ) - count ) < TICK_RATE_MS( 200 ) ) {
	}

	NVIC_SystemReset();
}


void	CLI_ClearHoppingRandomChannelNumber(void)
{
	uint8_t		count;

	if(SystemOperMode != SystemInNormal) return;

	if(EnableMaster == true) {		// for MASTER
		for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
			if(DeviceNodeSleepAndRandomHop[count] != NULL) DeviceNodeSleepAndRandomHop[count]->LoraHoppingStartChannel = 0;
		}
	} else {		// for SLAVE
		if(LoraGateWay != NULL) RandomHopStartChannel_SlaveDefaultHoppingChannel();
	}
}


void	CLI_DeleteAllLoraNodeMemorySpace(void)
{
	uint8_t	count;

	for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
		if(LoraNodeDevice[count] != NULL) {
			free(LoraNodeDevice[count]);
			LoraNodeDevice[count] = NULL;
			LoraLinkListEvent_LoraNodeEventDelete(count);
			free(DeviceNodeSleepAndRandomHop[count]);
			free(DeviceNodeSensor[count]);
			DeviceNodeSleepAndRandomHop[count] = NULL;
			DeviceNodeSensor[count] = NULL;
		}
	}

	LoraNodeCount = 0;
	LoraLinkListEvent_LoraEventClearAll();
}


void	CLI_DeleteLoraGateWayMemorySpace(void)
{
	if( LoraGateWay ) {
		free( LoraGateWay );
		LoraGateWay = NULL;
	}

	if( MySensor ) {
		free( MySensor );
		MySensor = NULL;
	}
}


void	CLI_LoraPV_TXresult( void )
{
	int8_t		str[7];

	if( LoraPV_TxTest == false ) {
		LoraPV_DefaultTxSet = false;
		LoraPV_TxCount = 0;
		LoraPV_TxTest = false;
		return;
	}

	LoraTxPayloadSize = 0;
	memset((void *)LoraTxBuffer, 0, LoraBufferLength);

	snprintf( (char *)str, sizeof(str), "%d", LoraPV_TxCount );
	Console_Output_String( "LoraPV_TXresult=" );
	Console_Output_String( (const char *) str );
	Console_Output_String( "\r\n" );

	LoraPV_DefaultTxSet = false;
	LoraPV_TxCount = 0;
	LoraPV_TxTest = false;
}

bool	CLI__Lora_Operation_Mode__Configure( uint8_t mode, uint8_t sf, uint8_t bw, const char *hopping_arg )
{
	bool	hop_en;

	if( strcmp( hopping_arg, "HopOn") == 0 ) {
		hop_en = true;
	} else {
		if( strcmp( hopping_arg, "HopOff") == 0 ) {
			hop_en = false;
		} else {
			return( false );
		}
	}

	LoraOperationMode = mode;
	SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
	SX1276LoRaSetSpreadingFactor( sf );
	SX1276LoRaSetSignalBandwidth( bw );
	LoRaSettings.FreqHopOn = hop_en;
	SX1276LoRaSetHopPeriod( Lora_RFHoppingPeriod );

	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate( & LoRaSettings );
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;
	SX1276LoRaSetRFFrequency( LoRaSettings.RFFrequency );
	Radio->StartRx();

	return( true );
}


int	CLI_GPIO_Examine( shell_cmd_args *args, uint8_t *gpio_port, uint16_t *port_pin )
{
	for(*gpio_port = 0 ; *gpio_port < 4 ; (*gpio_port)++) {
		if(args->args[0].val[0] == GPIO_Port[*gpio_port]) break;
	}
	if(*gpio_port >= 4) return SHELL_PROCESS_ERR_CMD_UNKN;

	*port_pin = (uint16_t)atoi(args->args[1].val);
	if(*gpio_port == 0) {		// PortA
		if(*port_pin <= 15) {
			*port_pin = PortAB_Pin[*port_pin];
		} else {
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	}

	if(*gpio_port == 1) {		// PortB
		if(*port_pin <= 15) {
#ifdef STM32F401xx
			if(*port_pin == 11)  return SHELL_PROCESS_ERR_CMD_UNKN;
#endif
			*port_pin = PortAB_Pin[*port_pin];
		} else {
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	}

	if(*gpio_port == 2) {		// PortC
		if((*port_pin >= 13) && (*port_pin <= 15)) {
			*port_pin = PortAB_Pin[(*port_pin) - 13];
		} else {
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	}

	if(*gpio_port == 3) {		// (in STM32F072, OtherPort=PortF),(in STM32F401, OtherPort=PortH)
		if((*port_pin == 0) || (*port_pin == 1)) {
			*port_pin = PortAB_Pin[*port_pin];
		} else {
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	}
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_GetBatteryADC( shell_cmd_args *args )
{
#ifdef STM32F401xx
	int8_t		str[40];
	uint16_t	adc_val;

	__IO uint32_t	ADC1ConvertedVoltage = 0;
	__IO uint32_t	VBAT = 0;

	if( args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	adc_val = ADC1__Get_Converted_Value( ADC_IDX___ADC0 );
	ADC1ConvertedVoltage = adc_val * 3300/0xFFF;		// note: can do embedded reference voltage(VRefint: Type 1.21
	VBAT = ADC1ConvertedVoltage*4200/3300;
	snprintf( (char *)str, sizeof(str), "V_ADC0=%d, ADC0=%d, VBAT=%d \n\r", ADC1ConvertedVoltage, adc_val, VBAT);		// for cli
	Console_Output_String( (const char *)str );

	adc_val = ADC1__Get_Converted_Value( ADC_IDX___ADC1 );
	ADC1ConvertedVoltage = adc_val * 3300/0xFFF;		// note: can do embedded reference voltage(VRefint: Type 1.21
	VBAT = ADC1ConvertedVoltage*4200/3300;
	snprintf( (char *)str, sizeof(str), "V_ADC1=%d, ADC1=%d, VBAT=%d \n\r", ADC1ConvertedVoltage, adc_val, VBAT);		// for cli
	Console_Output_String( (const char *)str );

	adc_val = ADC1__Get_Converted_Value( ADC_IDX___ADC4 );
	ADC1ConvertedVoltage = adc_val * 3300/0xFFF;		// note: can do embedded reference voltage(VRefint: Type 1.21
	VBAT = ADC1ConvertedVoltage*4200/3300;
	snprintf( (char *)str, sizeof(str), "V_ADC4=%d, ADC4=%d, VBAT=%d \n\r", ADC1ConvertedVoltage, adc_val, VBAT);		// for cli
	Console_Output_String( (const char *)str );

	adc_val = ADC1__Get_Converted_Value( ADC_IDX___ADC5 );
	ADC1ConvertedVoltage = adc_val * 3300/0xFFF;		// note: can do embedded reference voltage(VRefint: Type 1.21
	VBAT = ADC1ConvertedVoltage*4200/3300;
	snprintf( (char *)str, sizeof(str), "V_ADC5=%d, ADC5=%d, VBAT=%d \n\r", ADC1ConvertedVoltage, adc_val, VBAT);		// for cli
	Console_Output_String( (const char *)str );

	adc_val = ADC1__Get_Converted_Value( ADC_IDX___VBat );
	ADC1ConvertedVoltage = adc_val * 3300/0xFFF;		// note: can do embedded reference voltage(VRefint: Type 1.21
	VBAT = ADC1ConvertedVoltage*4200/3300;
	snprintf( (char *)str, sizeof(str), "V_PB0=%d, ADC_PB0=%d, VBAT=%d \n\r", ADC1ConvertedVoltage, adc_val, VBAT);		// for cli
	Console_Output_String( (const char *)str );

#endif

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_SystemReset( shell_cmd_args *args )
{
	if(args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	NVIC_SystemReset();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_GPIO_Set( shell_cmd_args *args )
{
	uint8_t			gpio_port, in_out;
	uint16_t		port_pin;
	GPIO_InitTypeDef	GPIO_InitStructure;

	if(args->count != 3) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(SystemOperMode != SystemInProductVerification) return SHELL_PROCESS_ERR_CMD_UNKN;

	if(CLI_GPIO_Examine(args, &gpio_port, &port_pin) != SHELL_PROCESS_OK) return SHELL_PROCESS_ERR_CMD_UNKN;

	in_out = (uint8_t)atoi(args->args[2].val);
	if((in_out != 0) && (in_out != 1)) return SHELL_PROCESS_ERR_CMD_UNKN;

	GPIO_InitStructure.GPIO_Pin = port_pin;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	if(in_out == 0) {
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	} else {
		if(in_out == 1) {
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		} else {
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	}

	switch(gpio_port) {
	case 0:
#ifdef STM32F072
		RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE );
#endif
#ifdef STM32F401xx
		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
#endif
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		break;

	case 1:
#ifdef STM32F072
		RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE );
#endif
#ifdef STM32F401xx
		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
#endif
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		break;

	case 2:
#ifdef STM32F072
		RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOC, ENABLE );
#endif
#ifdef STM32F401xx
		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
#endif
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		break;

	case 3:
#ifdef STM32F072
		RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOF, ENABLE );
		GPIO_Init(GPIOF, &GPIO_InitStructure);
#endif
#ifdef STM32F401xx
		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOH, ENABLE );
		GPIO_Init(GPIOH, &GPIO_InitStructure);
#endif
		break;

	default:
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_GPIO_Write( shell_cmd_args *args )
{
	uint8_t		gpio_port;
	uint16_t	port_pin;
	BitAction	high_low;

	if(args->count != 3) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(SystemOperMode != SystemInProductVerification) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(CLI_GPIO_Examine(args, &gpio_port, &port_pin) != SHELL_PROCESS_OK) return SHELL_PROCESS_ERR_CMD_UNKN;

	high_low = (BitAction)atoi(args->args[2].val);
	if((high_low != Bit_RESET) && (high_low != Bit_SET)) return SHELL_PROCESS_ERR_CMD_UNKN;

	switch(gpio_port) {
	case 0:
		GPIO_WriteBit(GPIOA, port_pin, high_low);
		break;

	case 1:
		GPIO_WriteBit(GPIOB, port_pin, high_low);
		break;

	case 2:
		GPIO_WriteBit(GPIOC, port_pin, high_low);
		break;

	case 3:
#ifdef STM32F072
		GPIO_WriteBit(GPIOF, port_pin, high_low);
#endif
#ifdef STM32F401xx
		GPIO_WriteBit(GPIOH, port_pin, high_low);
#endif
		break;

	default:
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_GPIO_Read( shell_cmd_args *args )
{
	uint8_t		gpio_port, pin_status;
	uint16_t	port_pin;
	int8_t		str[4];

	if(args->count != 2) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(SystemOperMode != SystemInProductVerification) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(CLI_GPIO_Examine(args, &gpio_port, &port_pin) != SHELL_PROCESS_OK) return SHELL_PROCESS_ERR_CMD_UNKN;

	switch(gpio_port) {
	case 0:
		pin_status = GPIO_ReadInputDataBit(GPIOA, port_pin);
		break;

	case 1:
		pin_status = GPIO_ReadInputDataBit(GPIOB, port_pin);
		break;

	case 2:
		pin_status = GPIO_ReadInputDataBit(GPIOC, port_pin);
		break;

	case 3:
#ifdef STM32F072
		pin_status = GPIO_ReadInputDataBit(GPIOF, port_pin);
#endif
#ifdef STM32F401xx
		pin_status = GPIO_ReadInputDataBit(GPIOH, port_pin);
#endif
		break;

	default:
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	snprintf( (char *)str, sizeof(str), "%d", pin_status);
	Console_Output_String( "GPIO_Read=" );
	Console_Output_String( args->args[0].val );
	Console_Output_String( "," );
	Console_Output_String( args->args[1].val );
	Console_Output_String( "," );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_CmdPortOff( shell_cmd_args *args )
{
	if( args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	CmdUART__Close();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_FskPV_TXstart( shell_cmd_args *args )
{
	if( args->count != 0 )		return SHELL_PROCESS_ERR_CMD_UNKN;
	if( FskPV_TxTest == true)	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_TxTest == true )	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_RxTest == true )	return SHELL_PROCESS_ERR_CMD_UNKN;

	LoraTxPayloadSize = LoraPV_DefaultTxLength;
	memset( (void *)LoraTxBuffer, 0, LoraBufferLength );
	memcpy( (void *)LoraTxBuffer, LoraPV_DefaultTxString, (size_t)LoraTxPayloadSize );

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	SX1276Reset( );
	LoRaOn = false;
	SX1276SetLoRaOn( LoRaOn );

	// Initialize FSK modem
	SX1276FskInit( );

	// Radio->StartRx();
	Radio->SetTxPacket( LoraTxBuffer, (uint16_t)LoraTxPayloadSize );		// Lora傳值出去
	FskPV_TxTest = true;

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_FskPV_TXstop( shell_cmd_args *args )
{
	if( args->count != 0)		return SHELL_PROCESS_ERR_CMD_UNKN;
	if( FskPV_TxTest == false )	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_RxTest == true )	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_TxTest == true )	return SHELL_PROCESS_ERR_CMD_UNKN;

	LoraTxPayloadSize = 0;
	memset( (void *)LoraTxBuffer, 0, LoraBufferLength );

	SX1276FskSetOpMode( RF_OPMODE_STANDBY );
	SX1276Reset( );
	LoRaOn = true;
	SX1276SetLoRaOn( LoRaOn );

	// Initialize LoRa modem
	SX1276LoRaInit( );
	Radio->StartRx();
	FskPV_TxTest = false;

	return SHELL_PROCESS_OK;
}



int	CLI_ShellCmd_FskPV_Freq(shell_cmd_args *args)
{
	uint32_t	freq;

	if(args->count != 1) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(FskPV_TxTest == false) return SHELL_PROCESS_ERR_CMD_UNKN;

	freq = (uint32_t)atoi(args->args[0].val);
	if((freq < MIN_RF_Freq) || (freq > MAX_RF_Freq))  return SHELL_PROCESS_ERR_CMD_UNKN;

	SX1276FskSetOpMode(RF_OPMODE_STANDBY);
	FskSettings.RFFrequency = freq;
	SX1276Reset( );

	// Initialize FSK modem
	SX1276FskInit( );

	// SX1276FskSetRFFrequency(freq);
	// Radio->StartRx();
	Radio->SetTxPacket( (const void *)LoraTxBuffer, (uint16_t)LoraTxPayloadSize );		// Lora傳值出去
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraPV_TXresult( shell_cmd_args *args )
{
	int8_t		str[7];

	if( args->count != 0 )		return SHELL_PROCESS_ERR_CMD_UNKN;
	if( FskPV_TxTest == true)	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_TxTest == false )	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_RxTest == true )	return SHELL_PROCESS_ERR_CMD_UNKN;

	LoraTxPayloadSize = 0;
	memset((void *)LoraTxBuffer, 0, LoraBufferLength);
	snprintf( (char *)str, sizeof(str), "%d", LoraPV_TxCount );

	Console_Output_String( "LoraPV_TXresult=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );

	LoraPV_DefaultTxSet = false;
	LoraPV_TxCount = 0;
	LoraPV_TxTest = false;

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraPV_TXstart( shell_cmd_args *args )
{
	if( (args->count > 2) || (args->count == 1) )	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( FskPV_TxTest == true )			return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_RxTest == true )			return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_TxTest == true )			return SHELL_PROCESS_ERR_CMD_UNKN;

	if( args->count == 0 ) {
		LoraPV_DefaultTxSet = true;
		LoraTxPayloadSize = LoraPV_DefaultTxLength;
		memset( (void *)LoraTxBuffer, 0, LoraBufferLength );
		memcpy( (void *)LoraTxBuffer, LoraPV_DefaultTxString, (size_t)LoraTxPayloadSize );
		// 傳預設的資料、資料長度,且在沒有下令停止前是無限次數傳
		// 若是此部分,則需等上面下 LoraPV_TXresult 指令去停止TX傳輸,並將傳輸次數回報
	} else {
		LoraPV_DefaultTxSet = false;
		LoraPV_TxTimes = atoi(args->args[0].val);
		if( LoraPV_TxTimes == 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;

		LoraTxPayloadSize = strlen(args->args[1].val);
		if( (LoraTxPayloadSize == 0) || (LoraTxPayloadSize > MAX_LoraPayloadLength) ) return SHELL_PROCESS_ERR_CMD_UNKN;

		memset( (void *)LoraTxBuffer, 0, LoraBufferLength );
		memcpy( (void *)LoraTxBuffer, args->args[1].val, (size_t)LoraTxPayloadSize );
		// 有兩參數,第一個參數是傳輸次數,第二參數是欲傳輸的資料(注意參數的存放長度)
		// 判斷第一個參數是否是數值,第二個數值的資料長度是否與設定符合(現在未確認當資料長度低於 Payloadlength 時是否會有問題)
		// 若是此部分,則在傳完指定次數後,就會自動由 USART1 將傳完的狀態回報
	}

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	SX1276LoRaSetPayloadLength(LoraTxPayloadSize);
	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;

	LoraPV_TxCount = 0;
	LoraPV_TxTest = true;
	Radio->StartRx();

	return SHELL_PROCESS_OK;
}



int	CLI_ShellCmd_LoraPV_RXresult( shell_cmd_args *args )
{
	int8_t		str[7];

	if( args->count != 0 )		return SHELL_PROCESS_ERR_CMD_UNKN;
	if( FskPV_TxTest == true )	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_RxTest == false )	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_TxTest == true )	return SHELL_PROCESS_ERR_CMD_UNKN;

	LoraTxPayloadSize = 0;
	memset((void *)LoraTxBuffer, 0, LoraBufferLength);

	snprintf( (char *)str, sizeof(str), "%d", LoraPV_RxCount );

	Console_Output_String( "LoraPV_RXresult=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );

	LoraPV_RxCount = 0;
	LoraPV_RxTest = false;

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraPV_RXstart( shell_cmd_args *args )
{
	if( args->count > 1 )		return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_RxTest == true )	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( FskPV_TxTest == true)	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( LoraPV_TxTest == true )	return SHELL_PROCESS_ERR_CMD_UNKN;

	LoraTxPayloadSize = 0;
	memset((void *)LoraTxBuffer, 0, LoraBufferLength);

	if( args->count == 0 ) {
		LoraTxPayloadSize = LoraPV_DefaultTxLength;
		memcpy( (void *)LoraTxBuffer, LoraPV_DefaultTxString, (size_t)LoraTxPayloadSize );
	} else {
		if( args->count == 1 ) {
			LoraTxPayloadSize = strlen(args->args[0].val);
			if( (LoraTxPayloadSize == 0) || (LoraTxPayloadSize > MAX_LoraPayloadLength) ) return SHELL_PROCESS_ERR_CMD_UNKN;
			memcpy( (void *)LoraTxBuffer, args->args[0].val, (size_t)LoraTxPayloadSize );
		}
	}

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	SX1276LoRaSetPayloadLength(LoraTxPayloadSize);
	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;

	LoraPV_RxCount = 0;
	LoraPV_RxTest = true;
	Radio->StartRx();

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraJoinNode( shell_cmd_args *args )
{
	uint32_t	addr;
	uint8_t		Addr[3];
	// uint8_t	Addr[3], count;

	if( args->count != 1 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ( SystemOperMode != SystemInNormal ) || (EnableMaster != true) ) {
		Console_Output_String( "System NOT in Normal mode,\r\n" );
		Console_Output_String( "Or Lora module NOT Master.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	if( strlen(args->args[0].val) != 6 ) {
		Console_Output_String( "Addr format error.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	if(LoraNodeCount >= MAX_LoraNodeNum) {
		Console_Output_String( "Node number full.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	addr = (uint32_t)strtol(args->args[0].val, NULL, 16);

	if( !! (addr & 0xFF000000) ) {
		Console_Output_String( "Addr format error.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	Addr[0] = (uint8_t)(addr & 0x000000FF);
	Addr[1] = (uint8_t)((addr & 0x0000FF00) >> 8);
	Addr[2] = (uint8_t)((addr & 0x00FF0000) >> 16);

	if( LoraLinkListEvent_BuildLoraEvent(LoraEventPriority1, 0, Master_AcsipProtocol_Join, Addr, NULL, NULL) == false ) return SHELL_PROCESS_ERR_CMD_UNKN;

	/*
	for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
		if(LoraNodeDevice[count] != NULL) {
			if((LoraNodeDevice[count]->NodeAddress[0] == Addr[0]) && (LoraNodeDevice[count]->NodeAddress[1] == Addr[1]) && (LoraNodeDevice[count]->NodeAddress[2] == Addr[2])) {
				break;
			}
		}
	}

	if(count < MAX_LoraNodeNum) {
		Console_Output_String( "This NODE already in AcSipLoraNet.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	} else {
	  if( LoraLinkListEvent_BuildLoraEvent(LoraEventPriority1, 0, Master_AcsipProtocol_Join, Addr, NULL, NULL) == false ) return SHELL_PROCESS_ERR_CMD_UNKN;
	}
	*/

	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd_LoraJoinNode_in_Force( shell_cmd_args *args )
{
	uint32_t	addr;
	uint8_t		Addr[3], ret;
	char		cb[16];

	if( args->count != 1 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ( SystemOperMode != SystemInNormal ) || ( ! EnableMaster ) ) {
		Console_Output_String( "System NOT in Normal mode,\r\n" );
		Console_Output_String( "Or Lora module NOT Master.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	if( strlen(args->args[0].val) != 6 ) {
		Console_Output_String( "Addr format error.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	if(LoraNodeCount >= MAX_LoraNodeNum) {
		Console_Output_String( "Node number full.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	addr = (uint32_t)strtol(args->args[0].val, NULL, 16);

	if( !! (addr & 0xFF000000) ) {
		Console_Output_String( "Addr format error.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	Addr[0] = (uint8_t)(addr & 0x000000FF);
	Addr[1] = (uint8_t)((addr & 0x0000FF00) >> 8);
	Addr[2] = (uint8_t)((addr & 0x00FF0000) >> 16);

// 	if( LoraLinkListEvent_BuildLoraEvent(LoraEventPriority1, 0, Master_AcsipProtocol_Join, Addr, NULL, NULL) == false ) return SHELL_PROCESS_ERR_CMD_UNKN;
	ret = AcsipProtocol__Add_Node( Addr );
	if( ret != AcsipProtocol_OK ) {
		snprintf( cb, sizeof( cb ), "Error %d\r\n", ret );
		Console_Output_String( cb );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraLeaveNode( shell_cmd_args *args )
{
	uint32_t	addr;
	uint8_t		Addr[3], count;

	if(args->count != 1) return SHELL_PROCESS_ERR_CMD_UNKN;

	if( (SystemOperMode != SystemInNormal) || (EnableMaster != true) ) {
		Console_Output_String( "System NOT in Normal mode,\r\n" );
		Console_Output_String( "Or Lora module NOT Master.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	if( strlen(args->args[0].val) != 6 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	addr = (uint32_t)strtol(args->args[0].val, NULL, 16);

	if( !!(addr & 0xFF000000) ) return SHELL_PROCESS_ERR_CMD_UNKN;

	Addr[0] = (uint8_t)(addr & 0x000000FF);
	Addr[1] = (uint8_t)((addr & 0x0000FF00) >> 8);
	Addr[2] = (uint8_t)((addr & 0x00FF0000) >> 16);

	for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
		if(LoraNodeDevice[count] != NULL) {
			if((LoraNodeDevice[count]->NodeAddress[0] == Addr[0]) && (LoraNodeDevice[count]->NodeAddress[1] == Addr[1]) && (LoraNodeDevice[count]->NodeAddress[2] == Addr[2])) {
				break;
			}
		}
	}

	if(count < MAX_LoraNodeNum) {
		if(LoraLinkListEvent_BuildLoraEvent(LoraEventPriority1, count, Master_AcsipProtocol_Leave, Addr, NULL, NULL) == false) {
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	} else {
		Console_Output_String( "NOT this NODE in AcSipLoraNet.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd_LoraLeaveNode_in_Force( shell_cmd_args *args )
{
	if( args->count != 1 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ( SystemOperMode != SystemInNormal ) || ( ! EnableMaster ) ) {
		Console_Output_String( "System NOT in Normal mode,\r\n" );
		Console_Output_String( "Or Lora module NOT Master.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	if( strlen(args->args[0].val) != 6 ) {
		Console_Output_String( "Addr format error.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	uint32_t	addr;
	uint8_t		Addr[3];

	addr = (uint32_t)strtol(args->args[0].val, NULL, 16);

	if( !!(addr & 0xFF000000) ) return SHELL_PROCESS_ERR_CMD_UNKN;

	Addr[0] = (uint8_t)(addr & 0x000000FF);
	Addr[1] = (uint8_t)((addr & 0x0000FF00) >> 8);
	Addr[2] = (uint8_t)((addr & 0x00FF0000) >> 16);

	if( ! AcsipProtocol__Del_Node( (uint8_t *) Addr ) ) {
		Console_Output_String( "This node NOT in AcSipLoraNet.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd_LoraSetNodePara(shell_cmd_args *args)
{
	const uint8_t *		base64_src;
	uint8_t			base64_size, Addr[3];
	uint16_t		count;
	uint32_t		addr, addr0;
	size_t			size = 0;

	if((SystemOperMode != SystemInNormal) || (EnableMaster != true)) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(args->count == 0) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(strlen(args->args[0].val) != 6) return SHELL_PROCESS_ERR_CMD_UNKN;

	addr0 = (uint32_t)strtol(args->args[0].val, NULL, 16);

	if(!!(addr0 & 0xFF000000)) return SHELL_PROCESS_ERR_CMD_UNKN;

	for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
		if(LoraNodeDevice[count] != NULL) {
			addr = (((uint32_t)LoraNodeDevice[count]->NodeAddress[2]) << 16) & 0x00FF0000;
			addr |= (((uint32_t)LoraNodeDevice[count]->NodeAddress[1]) << 8) & 0x0000FF00;
			addr |= ((uint32_t)LoraNodeDevice[count]->NodeAddress[0]) & 0x000000FF;
			Addr[0] = LoraNodeDevice[count]->NodeAddress[0];
			Addr[1] = LoraNodeDevice[count]->NodeAddress[1];
			Addr[2] = LoraNodeDevice[count]->NodeAddress[2];
			if(addr0 == addr) break;
		}
	}

	if( count < MAX_LoraNodeNum ) {
		if(LoraNodeDevice[count] != NULL) {
			switch(args->count) {
			case 1:
				if(LoraLinkListEvent_BuildLoraEvent(LoraEventPriority1, count, Master_AcsipProtocol_Interval, Addr, NULL, NULL) == false) return SHELL_PROCESS_ERR_CMD_UNKN;
				memset((void *)LoraNodeDevice[count]->Aliases, 0, AliasesSize);
				LoraNodeDevice[count]->Interval = 0;
				DeviceNodeSleepAndRandomHop[count]->DefineLoraRxFailureTimes = 0;
				break;

			case 2:
				base64_src = (const uint8_t *)(args->args[1].val);
				base64_size = strlen(args->args[1].val);
				if((base64_src[0] == '"') && (base64_src[base64_size - 1] == '"')) {
					if((base64_size - 2) >= 4) {
						memset((void *)LoraNodeDevice[count]->Aliases, 0, AliasesSize);
						if(Base64_decode( LoraNodeDevice[count]->Aliases, AliasesSize, &size, &base64_src[1], base64_size - 2 ) != 0) goto AliasesError_Out;
					} else {
						if((base64_size - 2) == 0) {
							memset((void *)LoraNodeDevice[count]->Aliases, 0, AliasesSize);
						} else {
							goto AliasesError_Out;
						}
					}
				} else {
					if((base64_src[0] != '"') && (base64_src[base64_size - 1] == '"')) {
						goto AliasesError_Out;
					} else {
						if((base64_src[0] == '"') && (base64_src[base64_size - 1] != '"')) {
							goto AliasesError_Out;
						} else {
							// for(size = 0 ; size < base64_size ; size++) {
								// 這一段程式碼是判斷 Interval 的值是數字字串
							// 	if( isdigit(base64_src[size]) == 0 ) goto SetError_Out;
							// }
							goto IntervalSet;

							/*
							LoraNodeDevice[count]->Interval = atoi(args->args[1].val);
							if(LoraLinkListEvent_BuildLoraEvent(LoraEventPriority1, count, Master_AcsipProtocol_Interval, Addr, NULL, NULL) == false) return SHELL_PROCESS_ERR_CMD_UNKN;
							if(LoraNodeDevice[count]->Interval != 0) {
								DeviceNodeSleepAndRandomHop[count]->DefineLoraRxFailureTimes = ceil(LoraNodeDevice[count]->Interval / SecondOfOneTimes);
							} else {
								DeviceNodeSleepAndRandomHop[count]->DefineLoraRxFailureTimes = 0;
							}
							*/
						}
					}
				}
				break;

			case 3:
				base64_src = (const uint8_t *)(args->args[2].val);
				base64_size = strlen(args->args[2].val);
				if((base64_src[0] == '"') && (base64_src[base64_size - 1] == '"')) {
					if((base64_size - 2) >= 4) {
						memset((void *)LoraNodeDevice[count]->Aliases, 0, AliasesSize);
						if(Base64_decode( LoraNodeDevice[count]->Aliases, AliasesSize, &size, &base64_src[1], base64_size - 2 ) != 0) goto AliasesError_Out;
					} else {
						if((base64_size - 2) == 0) {
							memset((void *)LoraNodeDevice[count]->Aliases, 0, AliasesSize);
						} else {
							goto AliasesError_Out;
						}
					}
				} else {
AliasesError_Out:
					Console_Output_String( "Aliases error.\r\n" );
					return SHELL_PROCESS_ERR_CMD_UNKN;
				}
IntervalSet:
				LoraNodeDevice[count]->Interval = atoi(args->args[1].val);
				if(LoraLinkListEvent_BuildLoraEvent(LoraEventPriority1, count, Master_AcsipProtocol_Interval, Addr, NULL, NULL) == false) return SHELL_PROCESS_ERR_CMD_UNKN;
				if(LoraNodeDevice[count]->Interval != 0) {
					DeviceNodeSleepAndRandomHop[count]->DefineLoraRxFailureTimes = DEF_Allowed_LoRa_Rx_Failure_Times_Times;
				} else {
					DeviceNodeSleepAndRandomHop[count]->DefineLoraRxFailureTimes = 0;
				}
				break;

			default:
				return SHELL_PROCESS_ERR_CMD_UNKN;
			}
		} else {
			goto SetError_Out;
		}
	} else {
SetError_Out:
		Console_Output_String( "This NODE NOT in AcSipLoraNet.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

#ifdef STM32F401xx
	SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara();
	SaveRecord_WriteInLoraMode();
#endif
#ifdef STM32F072
	SaveRecord_WriteInLoraNodePara();
#endif

	Console_Output_String( "Node=" );
	Console_Output_String( args->args[0].val );
	Console_Output_String( " setting SUCCESSED!\r\n" );

	return SHELL_PROCESS_OK;
}



int	CLI_ShellCmd_LoraGetNodePara(shell_cmd_args *args)
{
	int8_t		str[6];
	uint8_t		base64_aliases[AliasesDoubleSize];
	uint8_t		aliases_size;
	uint16_t	count;
	uint32_t	addr, addr0;
	size_t		size = 0;

	if((SystemOperMode != SystemInNormal) || (EnableMaster != true)) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(args->count != 1) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(strlen(args->args[0].val) != 6) return SHELL_PROCESS_ERR_CMD_UNKN;

	addr0 = (uint32_t)strtol(args->args[0].val, NULL, 16);
	if(!!(addr0 & 0xFF000000)) return SHELL_PROCESS_ERR_CMD_UNKN;

	for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
		if(LoraNodeDevice[count] != NULL) {
			addr = (((uint32_t)LoraNodeDevice[count]->NodeAddress[2]) << 16) & 0x00FF0000;
			addr |= (((uint32_t)LoraNodeDevice[count]->NodeAddress[1]) << 8) & 0x0000FF00;
			addr |= ((uint32_t)LoraNodeDevice[count]->NodeAddress[0]) & 0x000000FF;
			if(addr0 == addr) break;
		}
	}

	if( count < MAX_LoraNodeNum ) {
		if(LoraNodeDevice[count] != NULL) {
			memset((void *)base64_aliases, 0, AliasesDoubleSize);
			aliases_size = strlen((const char *)LoraNodeDevice[count]->Aliases);
			if(aliases_size > 0) {
				if(Base64_encode( base64_aliases, ((AliasesSize/3)*4), &size, (const uint8_t	*)LoraNodeDevice[count]->Aliases,  aliases_size) != 0) {
					Console_Output_String( "Aliases error.\r\n" );
					return SHELL_PROCESS_ERR_CMD_UNKN;
				}
			}

			snprintf( (char *)str, sizeof(str), "%u", LoraNodeDevice[count]->Interval );
			Console_Output_String( "Node=");
			Console_Output_String( args->args[0].val );
			Console_Output_String( " EVT=GetNode " );
			Console_Output_String( (const char *)str );
			Console_Output_String( " " );

			if(size > 0) {
				Console_Write( base64_aliases, size );
			} else {
				Console_Write( (uint8_t *)" ", 1 );
			}
		} else {
			goto GetError_Out;
		}
	} else {
GetError_Out:
		Console_Output_String( "This NODE NOT in AcSipLoraNet.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraNodeData(shell_cmd_args *args)
{
	// int8_t	str[5];		// test output
	uint8_t		base64_size;
	const uint8_t	*src;
	size_t		size;
	uint8_t		count;
	uint32_t	addr, addr0;
	uint8_t		dataarray[MaxMsgDataSize];

	if( SystemOperMode != SystemInNormal ) {
		Console_Output_String( "System NOT in Normal mode.\r\n" );
		return SHELL_PROCESS_ERR_CMD_UNKN;
	}

	if( ! EnableMaster ) {
		if( args->count != 1 ) {
			Console_Output_String( "Lora module is Slave.\r\n" );
			Console_Output_String( "Need one parameter.\r\n" );
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}

		if( ! LoraGateWay ) {
			Console_Output_String( "NO LoraGateWay.\r\n" );
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}

		if( LoraNeedTxData ) return SHELL_PROCESS_ERR_CMD_UNKN;

		memset((void *)TxDataArray, 0, MaxMsgDataSize);
		src = (const uint8_t *)(args->args[0].val);
		base64_size = strlen(args->args[0].val);
		if( Base64_decode( TxDataArray, MaxMsgDataSize, &size, src, base64_size ) != 0 ) {
			memset((void *)TxDataArray, 0, MaxMsgDataSize);
			TxDataSize = 0;
			LoraNeedTxData = false;
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}

		if( size > MaxMsgDataSize ) {
			memset((void *)TxDataArray, 0, MaxMsgDataSize);
			TxDataSize = 0;
			LoraNeedTxData = false;
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
		TxDataSize = size;
		LoraNeedTxData = true;
		return SHELL_PROCESS_OK;
	}

	if( EnableMaster ) {
		if(args->count != 2) {
			Console_Output_String( "Lora module is Master.\r\n" );
			Console_Output_String( "Need two parameter.\r\n" );
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}

		if(strlen(args->args[0].val) != 6) {
			// Console_Output_String( "AddrSize=" );		// test output
			// sprintf((char *)str, "%d", strlen(args->args[0].val));
			// Console_Output_String( (const char *)str );		// test output
			// Console_Output_String( "\r\n" );
			Console_Output_String( "Lora node address length NOT EQUAL 6Bytes.\r\n" );
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}

		addr0 = (uint32_t)strtol(args->args[0].val, NULL, 16);

		if(!!(addr0 & 0xFF000000)) return SHELL_PROCESS_ERR_CMD_UNKN;

		for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
			if(LoraNodeDevice[count] != NULL) {
				addr = (((uint32_t)LoraNodeDevice[count]->NodeAddress[2]) << 16) & 0x00FF0000;
				addr |= (((uint32_t)LoraNodeDevice[count]->NodeAddress[1]) << 8) & 0x0000FF00;
				addr |= ((uint32_t)LoraNodeDevice[count]->NodeAddress[0]) & 0x000000FF;
				if(addr0 == addr) break;
			}
		}

		if( count < MAX_LoraNodeNum ) {
			src = (const uint8_t *)(args->args[1].val);
			base64_size = strlen(args->args[1].val);

			memset((void *)dataarray, 0, MaxMsgDataSize);
			if( Base64_decode( dataarray, MaxMsgDataSize, &size, src, base64_size ) != 0 ) {
				return SHELL_PROCESS_ERR_CMD_UNKN;
			} else {
				if(LoraLinkListEvent_BuildLoraEvent(LoraEventPriority1, count, Master_AcsipProtocol_Data, LoraNodeDevice[count]->NodeAddress, dataarray, (uint8_t *)&size) == false) {
					return SHELL_PROCESS_ERR_CMD_UNKN;
				}
			}
		} else {
			Console_Output_String( "This NODE NOT in AcSipLoraNet.\r\n" );
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
		return SHELL_PROCESS_OK;
	}
}

void	CLI__Console_Output_Node_Address( const uint8_t *addr )
{
	char	cs[8];
	int8_t	i;

	for( i = 2 ; i >= 0 ; i-- ) {
		snprintf( cs, sizeof( cs ), "%02x", addr[i] );
		Console_Output_String( cs );
	}
}

int	CLI_ShellCmd_LoraGetAllNodeAddr( shell_cmd_args *args )
{
	uint8_t		count, temp = 0;
	char		str[8];

	if( args->count ) return SHELL_PROCESS_ERR_CMD_UNKN;
	if( SystemOperMode != SystemInNormal || ! EnableMaster ) return SHELL_PROCESS_ERR_CMD_UNKN;

	for( count = 0 ; count < MAX_LoraNodeNum ; count++ ) {
		if( LoraNodeDevice[count] ) temp++;
	}
	Console_Output_String( "Nodes=" );
	snprintf( str, sizeof( str ), "%u ", temp );
	Console_Output_String( str );

	if( temp ) {
		for( count = 0 ; count < MAX_LoraNodeNum ; count++ ) {
			if( LoraNodeDevice[count] ) {
				CLI__Console_Output_Node_Address( LoraNodeDevice[count]->NodeAddress );
				Console_Output_String( " " );
			}
		}
	}
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetGateWayAddr( shell_cmd_args *args )
{
	if( args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;
	if( (SystemOperMode != SystemInNormal) || (EnableMaster == true) ) return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! LoraGateWay ) {
		Console_Output_String( "LoraGateWayAddr=NULL" );
		Console_Output_String( "\r\n" );
		return SHELL_PROCESS_OK;
	}

	Console_Output_String( "LoraGateWayAddr=" );
	CLI__Console_Output_Node_Address( LoraGateWay->NodeAddress );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}



int	CLI_ShellCmd_OutputTraceData( shell_cmd_args *args )
{
	if(args->count != 0) return SHELL_PROCESS_ERR_CMD_UNKN;
	if((SystemOperMode != SystemInNormal) || (EnableMaster != false)) return SHELL_PROCESS_ERR_CMD_UNKN;

	SaveRecord_ReadOutTraceData();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_FormatTraceDataSpace( shell_cmd_args *args )
{
	uint32_t	count = 0;

	if(args->count != 0) return SHELL_PROCESS_ERR_CMD_UNKN;
	if((SystemOperMode != SystemInNormal) || (EnableMaster != false)) return SHELL_PROCESS_ERR_CMD_UNKN;

	SaveRecord_ClearTraceDataSpace();

	do {
		asm("NOP");
		count++;
	} while ( count < 200000 );

	return SHELL_PROCESS_OK;
}

#ifdef Board__A22_Tracker
int	CLI_ShellCmd_BleEnable( shell_cmd_args *args )
{
	if(args->count != 0) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(SystemOperMode != SystemInProductVerification) return SHELL_PROCESS_ERR_CMD_UNKN;

	BlueTooth_DA14580Run(ComPortBaudRate);
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_BleDisable( shell_cmd_args *args )
{
	if(args->count != 0) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(SystemOperMode != SystemInProductVerification) return SHELL_PROCESS_ERR_CMD_UNKN;

	BlueTooth_DA14580Disable();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_GpsEnable( shell_cmd_args *args )
{
	if(args->count != 0) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(SystemOperMode != SystemInProductVerification) return SHELL_PROCESS_ERR_CMD_UNKN;

	GPS_MT3333Run();
	GPS_HandmadeOff = false;
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_GpsDisable( shell_cmd_args *args )
{
	if(args->count != 0) return SHELL_PROCESS_ERR_CMD_UNKN;
	if(SystemOperMode != SystemInProductVerification) return SHELL_PROCESS_ERR_CMD_UNKN;

	GPS_MT3333Stop();
	GPS_HandmadeOff = true;
	return SHELL_PROCESS_OK;
}
#endif


int	CLI_ShellCmd_LoraGetMyAddr( shell_cmd_args *args )
{
	uint8_t		*LoraAddress;

	if( args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	LoraAddress = AcsipProtocol_ReadMyAddress();

	Console_Output_String( "LoraMyAddr=" );
	CLI__Console_Output_Node_Address( LoraAddress );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetMyInterval( shell_cmd_args *args )
{
	int8_t		str[6];

	if( args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;
	if( ( SystemOperMode != SystemInNormal ) || (EnableMaster != false) ) return SHELL_PROCESS_ERR_CMD_UNKN;

	if(LoraGateWay == NULL) {
		Console_Output_String( "LoraGateWay=NULL " );
		Console_Output_String( "LoraMyInterval=0\r\n" );
	} else {
		snprintf( (char *)str, sizeof(str), "%u", LoraGateWay->Interval );
		Console_Output_String( "LoraMyInterval=" );
		Console_Output_String( (const char *)str );
		Console_Output_String( "\r\n" );
	}

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_GetUUID( shell_cmd_args *args )
{
	int8_t		str[9];
	int8_t		count;
	uint32_t	uuid[3];

	if( args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	UUID_Read( uuid );
	Console_Output_String( "UUID=" );
	for(count = 2 ; count >= 0 ; count--) {
		snprintf( (char *)str, sizeof(str), "%08x", uuid[count] );
		Console_Output_String( (const char *)str );
	}
	Console_Output_String( "\r\n" );

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_GetFWVersion( shell_cmd_args *args )
{
	if(args->count != 0) return SHELL_PROCESS_ERR_CMD_UNKN;

	Console_Output_String( "FirmwareVersion=" );
	Console_Output_String( FirmwareVersion );
	Console_Output_String( " for " );
	Console_Output_String( Target_Lora_Device );
	Console_Output_String( "\r\n" );

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetSystemMode( shell_cmd_args *args )
{
	if( args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	if( SystemOperMode == SystemInNormal ) {
		Console_Output_String( "SystemOperationMode=InNormal\r\n" );
		return SHELL_PROCESS_OK;
	}

	if( SystemOperMode == SystemInProductVerification ) {
		Console_Output_String( "SystemOperationMode=InProductVerification\r\n" );
		return SHELL_PROCESS_OK;
	}

	if( SystemOperMode == SystemInPingPognTest ) {
		Console_Output_String( "SystemOperationMode=InPingPognTest\r\n" );
		return SHELL_PROCESS_OK;
	}

	if( SystemOperMode == SystemInIdle ) {
		Console_Output_String( "SystemOperationMode=InIdle\r\n" );
		return SHELL_PROCESS_OK;
	}

	return SHELL_PROCESS_ERR_CMD_UNKN;
}


int	CLI_ShellCmd_LoraGetMode( shell_cmd_args *args )
{
	if( args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	if( EnableMaster ) {
		Console_Output_String( "LoraMode=MASTER\r\n" );
	} else {
		Console_Output_String( "LoraMode=SLAVE\r\n" );
	}
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetPara( shell_cmd_args *args )
{
	bool		hop, header, crc;
	uint8_t		bw, sf, ec, period, pre, pay;
	uint32_t	freq, time;
	int8_t		power, str[11];

	if(args->count != 0)	return SHELL_PROCESS_ERR_CMD_UNKN;

	freq = SX1276LoRaGetRFFrequency();
	snprintf( (char *)str, sizeof(str), "%d", freq );
	Console_Output_String( "RFFrequency=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "  " );

	bw = SX1276LoRaGetSignalBandwidth();
	snprintf( (char *)str, sizeof(str), "%d", bw );
	Console_Output_String( "BW=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "  " );

	sf = SX1276LoRaGetSpreadingFactor();
	snprintf( (char *)str, sizeof(str), "%d", sf );
	Console_Output_String( "SF=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "  ");

	ec = SX1276LoRaGetErrorCoding();
	snprintf( (char *)str, sizeof(str), "%d", ec );
	Console_Output_String( "ErrorCoding=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "  " );

	power = SX1276LoRaGetRFPower();
	snprintf( (char *)str, sizeof(str), "%d", power );
	Console_Output_String( "RFPower=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "  " );

	hop = LoRaSettings.FreqHopOn;
	Console_Output_String( "FreqHopOn=" );
	if( hop ) {
		period = SX1276LoRaGetHopPeriod();
		snprintf( (char *)str, sizeof(str), "%d", period );
		Console_Output_String( "true" );
		Console_Output_String( (const char *)str );
	} else {
		Console_Output_String( "false" );
	}
	Console_Output_String( "  " );

	header = SX1276LoRaGetImplicitHeaderOn();
	Console_Output_String( "HeaderStatus=" );
	if(header == true) {
		Console_Output_String( "Implicit" );
	} else {
		Console_Output_String( "Explicit" );
	}
	Console_Output_String( "  " );

	crc = SX1276LoRaGetPacketCrcOn();
	Console_Output_String( "CRC=" );
	if(crc == true) {
		Console_Output_String( "ON" );
	} else {
		Console_Output_String( "OFF" );
	}
	Console_Output_String( "  " );

	pre = SX1276LoRaGetPreambleLength();
	snprintf( (char *)str, sizeof(str), "%d", pre );
	Console_Output_String( "PreambleLength=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "  " );

	pay = SX1276LoRaGetPayloadLength();
	snprintf( (char *)str, sizeof(str), "%d", pay );
	Console_Output_String( "PayloadLength=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "  " );

	pay = SX1276LoRaGetMaxPayloadLength();
	snprintf( (char *)str, sizeof(str), "%d", pay );
	Console_Output_String( "MaxPayloadLength=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "  " );

	time = SX1276LoRaGetTxPacketTimeout();
	snprintf( (char *)str, sizeof(str), "%d", time );
	Console_Output_String( "TxPacketTimeout=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "  " );

	time = SX1276LoRaGetRxPacketTimeout();
	snprintf( (char *)str, sizeof(str), "%d", time );
	Console_Output_String( "RxPacketTimeout=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetRFOsc( shell_cmd_args *args )
{
	if(args->count != 0) return SHELL_PROCESS_ERR_CMD_UNKN;

	if( LoRaSettings.Oscillator == OscillatorCrystal ) {
		Console_Output_String( "RFOscillator=Crystal" );
	} else {
		if( LoRaSettings.Oscillator == OscillatorTCXO ) {
			Console_Output_String( "RFOscillator=TCXO" );
		} else {
			Console_Output_String( "RFOscillator=UnKnown" );
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	}
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetRFFrequency( shell_cmd_args *args )
{
	uint32_t	freq;
	int8_t		str[11];

	if( args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	freq = SX1276LoRaGetRFFrequency();
	snprintf( (char *)str, sizeof(str), "%d", freq );

	Console_Output_String( "RFFrequency=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetRxPacketTimeout( shell_cmd_args *args )
{
	uint32_t	time;
	int8_t		str[10];

	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	time = SX1276LoRaGetRxPacketTimeout();
	snprintf( (char *)str, sizeof(str), "%d", time );

	Console_Output_String( "RxPacketTimeout=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetTxPacketTimeout( shell_cmd_args *args )
{
	uint32_t	time;
	int8_t		str[10];

	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	time = SX1276LoRaGetTxPacketTimeout();
	snprintf( (char *)str, sizeof(str), "%d", time );

	Console_Output_String( "TxPacketTimeout=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetRFPower( shell_cmd_args *args )
{
	int8_t	power;
	int8_t	str[4];

	if( args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	power = SX1276LoRaGetRFPower();
	snprintf( (char *)str, sizeof(str), "%d", power );

	Console_Output_String( "RFPower=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetPreambleLength( shell_cmd_args *args )
{
	uint8_t		pre;
	int8_t		str[6];

	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	pre = SX1276LoRaGetPreambleLength();
	snprintf( (char *)str, sizeof(str), "%d", pre );

	Console_Output_String( "PreambleLength=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetPayloadLength( shell_cmd_args *args )
{
	uint8_t		pay;
	char		str[32];

	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	pay = SX1276LoRaGetPayloadLength();
	snprintf( str, sizeof(str), "PayloadLength= %d, ", pay );
	Console_Output_String( str );

	pay = SX1276LoRaGetMaxPayloadLength();
	snprintf( str, sizeof(str), "MaxPayloadLength= %d\r\n", pay );
	Console_Output_String( str );

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetCRC( shell_cmd_args *args )
{
	bool	crc;

	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	crc = SX1276LoRaGetPacketCrcOn();
	Console_Output_String( "CRC=" );

	if(crc == true) {
		Console_Output_String( "ON" );
	} else {
		Console_Output_String( "OFF" );
	}

	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetImplicitHeader( shell_cmd_args *args )
{
	bool	header;

	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	header = SX1276LoRaGetImplicitHeaderOn();
	Console_Output_String( "HeaderStatus=" );

	if(header == true) {
		Console_Output_String( "Implicit" );
	} else {
		Console_Output_String( "Explicit" );
	}

	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


//int	CLI_ShellCmd_LoraGetFreqHopOn( shell_cmd_args *args )
//{
//	bool		hop;
//	uint8_t		period;
//	int8_t		str[4];

//	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

//	hop = LoRaSettings.FreqHopOn;
//	Console_Output_String( "FreqHopOn=" );

//	if( hop ) {
//		period = SX1276LoRaGetHopPeriod();
//		snprintf( (char *)str, sizeof(str), "%d", period );
//		Console_Output_String( "true" );
//		Console_Output_String( (const char *)str );
//	} else {
//		Console_Output_String( "false" );
//	}

//	Console_Output_String( "\r\n" );
//	return SHELL_PROCESS_OK;
//}


int	CLI_ShellCmd_LoraGetErrorCoding( shell_cmd_args *args )
{
	uint8_t		ec;
	int8_t		str[6];

	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	ec = SX1276LoRaGetErrorCoding();
	snprintf( (char *)str, sizeof(str), "%d", ec );

	Console_Output_String( "ErrorCoding=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetSF( shell_cmd_args *args )
{
	uint8_t		sf;
	int8_t		str[6];

	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	sf = SX1276LoRaGetSpreadingFactor();
	snprintf( (char *)str, sizeof(str), "%d", sf );

	Console_Output_String( "SF=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetBW( shell_cmd_args *args )
{
	uint8_t		bw;
	int8_t		str[6];

	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	bw = SX1276LoRaGetSignalBandwidth();
	snprintf( (char *)str, sizeof(str), "%d", bw );

	Console_Output_String( "BW=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetRxGain( shell_cmd_args *args )
{
	uint8_t		gain;
	int8_t		str[6];

	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	gain = SX1276LoRaGetPacketRxGain();
	snprintf( (char *)str, sizeof(str), "%d", gain );

	Console_Output_String( "RxGain=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetSNR( shell_cmd_args *args )
{
	int8_t	snr;
	int8_t	str[5];

	if( args->count != 0 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	snr = SX1276LoRaGetPacketSnr();
	snprintf( (char *)str, sizeof(str), "%d", snr );

	Console_Output_String( "SNR=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraGetRSSI( shell_cmd_args *args )
{
	double	rssi;
	int8_t	str[8];

	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	rssi = SX1276LoRaGetPacketRssi();

	snprintf( (char *)str, sizeof(str), "%3.2f", rssi );

	Console_Output_String( "RSSI=" );
	Console_Output_String( (const char *)str );
	Console_Output_String( "\r\n" );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_FactoryReset( shell_cmd_args *args )
{
	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	CLI_FactoryReset();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSystemMode( shell_cmd_args *args )
{
	// uint8_t	count;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if(strcmp(args->args[0].val, "inNormal") == 0)  {
		SystemOperMode = SystemInNormal;
		LoRaSettings.PayloadLength = MaxPacketSize;
		if(EnableMaster == false) {		// for SLAVE
			SaveRecord_ReadOutLoraGateWayPara();
			SaveRecord_TraceDataSpaceInitialization();
#ifdef Board__A22_Tracker
			BlueTooth_DA14580Disable();
			GPS_MT3333Run();
			GPS_HandmadeOff = false;
#endif
			RandomHopStartChannel_SlaveDefaultHoppingChannel();
		} else {		// for MASTER
			SaveRecord_ReadOutLoraNodePara();
#ifdef Board__A22_Tracker
			GPS_MT3333Stop();
			BlueTooth_DA14580Run(ComPortBaudRate);
#endif
		}
		// goto ReadPara_PROCESS;
	} else {
		if(strcmp(args->args[0].val, "inPV") == 0) {
			CLI_DeleteAllLoraNodeMemorySpace();
			CLI_DeleteLoraGateWayMemorySpace();
			SystemOperMode = SystemInProductVerification;
			LoRaSettings.PayloadLength = 9;
			LoRaSettings.FreqHopOn = 0;		// [0: OFF, 1: ON]
			RandomHopStartChannel_SlaveDefaultHoppingChannel();
#ifdef Board__A22_Tracker
			GPS_MT3333Stop();
			BlueTooth_DA14580Disable();
#endif
			// goto Free_PROCESS;
		} else {
			if(strcmp(args->args[0].val, "inTD") == 0) {
				CLI_DeleteAllLoraNodeMemorySpace();
				CLI_DeleteLoraGateWayMemorySpace();
				SystemOperMode = SystemInPingPognTest;
				LoRaSettings.PayloadLength = 9;
				RandomHopStartChannel_SlaveDefaultHoppingChannel();
#ifdef Board__A22_Tracker
				GPS_MT3333Stop();
				BlueTooth_DA14580Disable();
#endif
				// goto Free_PROCESS;
			} else {
				if(strcmp(args->args[0].val, "inIdle") == 0) {
					CLI_DeleteAllLoraNodeMemorySpace();
					CLI_DeleteLoraGateWayMemorySpace();
					SystemOperMode = SystemInIdle;
					RandomHopStartChannel_SlaveDefaultHoppingChannel();
#ifdef Board__A22_Tracker
					GPS_MT3333Stop();
					BlueTooth_DA14580Disable();
#endif
					return SHELL_PROCESS_OK;
				} else {
					return SHELL_PROCESS_ERR_CMD_UNKN;
				}
			}
		}
	}

	CLI_ClearHoppingRandomChannelNumber();

/*
ReadPara_PROCESS:
	SaveRecord_ReadOutMyselfPara();
	SaveRecord_ReadOutLoraGateWayPara();
	SaveRecord_ReadOutLoraNodePara();
	SaveRecord_TraceDataSpaceInitialization();
	CLI_LoraReInit();
	goto Go_Out;

Free_PROCESS:
	for(count = 0 ; count < MAX_LoraNodeNum ; count++) {
		if(LoraNodeDevice[count] != NULL) {
			free(LoraNodeDevice[count]);
			free(DeviceNodeSensor[count]);
			LoraNodeDevice[count] = NULL;
			LoraNodeDevice[count] = NULL;
		}
	}
	if(LoraGateWay != NULL) {
		free(LoraGateWay);
		LoraGateWay = NULL;
	}
	if(MySensor !=NULL) {
		free(MySensor);
		MySensor = NULL;
	}
	goto Go_Out;

Go_Out:
*/
	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;

	return SHELL_PROCESS_OK;
}

void	CLI__Save_LoRa_Configure_into_Flash( void )
{
#ifdef STM32F401xx
	SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara();
#endif
#ifdef STM32F072
	SaveRecord_WriteInMyselfParaAndLoraGateWayPara();
#endif
	SaveRecord_WriteInLoraMode();
}

int	CLI_ShellCmd_LoraSF7BW250RS120( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 1, 7, 8, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF7BW125RS123( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 2, 7, 7, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF8BW500RS120( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 3, 8, 9, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF8BW250RS123( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 4, 8, 8, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF8BW125RS126( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 5, 8, 7, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF9BW500RS123( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 6, 9, 9, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF9BW250RS126( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 7, 9, 8, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF9BW125RS129( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 8, 9, 7, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF10BW500RS126( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 9, 10, 9, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF10BW250RS129( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 10, 10, 8, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF10BW125RS132( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 11, 10, 7, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF11BW250RS132( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 12, 11, 8, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF11BW125RS135( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 13, 11, 7, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF12BW250RS134( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 14, 12, 8, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF12BW125RS137( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! CLI__Lora_Operation_Mode__Configure( 15, 12, 7, args->args[0].val ) ) return( SHELL_PROCESS_ERR_CMD_UNKN );
	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraFreq( shell_cmd_args *args )
{
	uint32_t	freq;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	freq = (uint32_t)atoi(args->args[0].val);

	if( (freq < MIN_RF_Freq) || (freq > MAX_RF_Freq) ) return SHELL_PROCESS_ERR_CMD_UNKN;

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	SX1276LoRaSetRFFrequency(freq);
	LoRaSettings.RFFrequency = freq;
	Radio->StartRx();
	// 此處設定1276模式,設定好preamble後,重算timeout時間與設定,並回到原本狀態

	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraBW( shell_cmd_args *args )
{
	uint8_t		bw;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	bw = atoi(args->args[0].val);
	if( bw > 9 ) bw = 9;
	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	SX1276LoRaSetSignalBandwidth(bw);

	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;
	LoRaSettings.SignalBw = bw;
	Radio->StartRx();
	// 此處設定1276模式,設定好preamble後,重算timeout時間與設定,並回到原本狀態

	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraSF( shell_cmd_args *args )
{
	uint8_t		sf;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	sf = atoi(args->args[0].val);
	if( sf > 12 ) {
		sf = 12;
	} else {
		if(sf < 6) sf = 6;
	}

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	SX1276LoRaSetSpreadingFactor(sf);
	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;
	LoRaSettings.SpreadingFactor = sf;
	Radio->StartRx();
	// 此處設定1276模式,設定好preamble後,重算timeout時間與設定,並回到原本狀態

	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraErrorCoding( shell_cmd_args *args )
{
	uint8_t		coding;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	coding = atoi(args->args[0].val);
	if( coding > 4 ) {
		coding = 4;
	} else {
		if(coding < 1) coding = 1;
	}

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	SX1276LoRaSetErrorCoding(coding);
	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;
	LoRaSettings.ErrorCoding = coding;
	Radio->StartRx();
	// 此處設定1276模式,設定好preamble後,重算timeout時間與設定,並回到原本狀態

	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraFreqHop( shell_cmd_args *args )
{
	if( args->count != 1 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	if(strcmp(args->args[0].val, "ON") == 0) {
		LoRaSettings.FreqHopOn = true;
	} else {
		if(strcmp(args->args[0].val, "OFF") == 0) {
			LoRaSettings.FreqHopOn = false;
		} else {
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	}

	SX1276LoRaSetRFFrequency( Lora_RFFrequency );
	CLI_ClearHoppingRandomChannelNumber();

	CLI__Save_LoRa_Configure_into_Flash();

	Radio->StartRx();
	return SHELL_PROCESS_OK;
}


//int	CLI_ShellCmd_LoraFreqHopOn( shell_cmd_args *args )
//{
//	uint16_t		hopperiod;

//	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

//	hopperiod = atoi(args->args[0].val);
//	if(hopperiod >= 256) return SHELL_PROCESS_ERR_CMD_UNKN;
//	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);

//	switch(hopperiod) {
//	case 0:
//		LoRaSettings.FreqHopOn = false;
//		break;

//	default:
//		LoRaSettings.FreqHopOn = true;
//	}

//	SX1276LoRaSetHopPeriod(hopperiod);
//	SX1276LoRaSetRFFrequency(Lora_RFFrequency);
//	// LoRaSettings.RFFrequency = Lora_RFFrequency;

//	CLI_ClearHoppingRandomChannelNumber();
//	Radio->StartRx();

//	CLI__Save_LoRa_Configure_into_Flash();
//	return SHELL_PROCESS_OK;
//}


int	CLI_ShellCmd_LoraImplicitHeader( shell_cmd_args *args )
{
	if( args->count != 1 ) return SHELL_PROCESS_ERR_CMD_UNKN;

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	if(strcmp(args->args[0].val, "ENABLE") == 0) {
		SX1276LoRaSetImplicitHeaderOn(true);
		LoRaSettings.ImplicitHeaderOn = true;
	} else {
		if(strcmp(args->args[0].val, "DISABLE") == 0) {
			SX1276LoRaSetImplicitHeaderOn(false);
			LoRaSettings.ImplicitHeaderOn = false;
		} else {
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	}

	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;
	Radio->StartRx();
	// 此處設定1276模式,設定好preamble後,重算timeout時間與設定,並回到原本狀態

	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraCRC( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	if(strcmp(args->args[0].val, "ENABLE") == 0) {
		SX1276LoRaSetPacketCrcOn(true);
		LoRaSettings.CrcOn = true;
	} else {
		if(strcmp(args->args[0].val, "DISABLE") == 0) {
			SX1276LoRaSetPacketCrcOn(false);
			LoRaSettings.CrcOn = false;
		} else {
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	}

	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;
	Radio->StartRx();
	// 此處設定1276模式,設定好preamble後,重算timeout時間與設定,並回到原本狀態

	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraPayloadLength( shell_cmd_args *args )
{
	uint8_t		payload;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	payload = atoi(args->args[0].val);

	if( payload > MAX_LoraPayloadLength ) payload = MAX_LoraPayloadLength;
	if( payload < MIN_LoraPayloadLength ) payload = MIN_LoraPayloadLength;

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	SX1276LoRaSetPayloadLength(payload);
	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;
	LoRaSettings.PayloadLength = payload;

	CLI__Save_LoRa_Configure_into_Flash();
	Radio->StartRx();
	// 此處設定1276模式,設定好preamble後,重算timeout時間與設定,並回到原本狀態

	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd_MaxLoraPayloadLength( shell_cmd_args *args )
{
	uint8_t		payload;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	payload = atoi(args->args[0].val);

	if( payload > MAX_LoraPayloadLength ) payload = MAX_LoraPayloadLength;
	if( payload < MIN_LoraPayloadLength ) payload = MIN_LoraPayloadLength;

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	SX1276LoRaSetMaxPayloadLength( payload );
	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;
	LoRaSettings.MaxPayloadLength = payload;

	CLI__Save_LoRa_Configure_into_Flash();

	Radio->StartRx();
	// 此處設定1276模式,設定好preamble後,重算timeout時間與設定,並回到原本狀態

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraPreambleLength( shell_cmd_args *args )
{
	uint16_t	preamble;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	preamble = atoi(args->args[0].val);

	if(preamble > 65535) {
		preamble = 65535;
	} else {
		if(preamble < 6) preamble = 6;
	}

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	SX1276LoRaSetPreambleLength(preamble);
	LoRaSettings.RxPacketTimeout = CLI_LoraTimeOutCalculate(&LoRaSettings);
	LoRaSettings.TxPacketTimeout = LoRaSettings.RxPacketTimeout;
	LoRaSettings.PreambleLength = preamble;
	Radio->StartRx();
	// 此處設定1276模式,設定好preamble後,重算timeout時間與設定,並回到原本狀態

	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraMode( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;
	if( SystemOperMode != SystemInNormal ) return SHELL_PROCESS_ERR_CMD_UNKN;

	if((strcmp(args->args[0].val, "MASTER") == 0) && (EnableMaster == true)) return SHELL_PROCESS_OK;
	if((strcmp(args->args[0].val, "SLAVE") == 0) && (EnableMaster == false)) return SHELL_PROCESS_OK;

#ifdef STM32F401xx
	SaveRecord_WriteInMyselfParaAndLoraGateWayParaAndLoraNodePara();
#endif
#ifdef STM32F072
	SaveRecord_WriteInMyselfParaAndLoraGateWayPara();
	SaveRecord_WriteInLoraNodePara();
#endif

	if(strcmp(args->args[0].val, "MASTER") == 0)  {
		EnableMaster = true;
#ifdef Board__A22_Tracker
		GPS_MT3333Stop();
#endif
		CLI_DeleteLoraGateWayMemorySpace();
		SaveRecord_ReadOutLoraNodePara();
#ifdef Board__A22_Tracker
		BlueTooth_DA14580Run(ComPortBaudRate);
#endif
	} else {
		if(strcmp(args->args[0].val, "SLAVE") == 0) {
			EnableMaster = false;
#ifdef Board__A22_Tracker
			BlueTooth_DA14580Disable();
#endif
			CLI_DeleteAllLoraNodeMemorySpace();
			SaveRecord_ReadOutLoraGateWayPara();
			SaveRecord_TraceDataSpaceInitialization();
#ifdef Board__A22_Tracker
			GPS_MT3333Run();
			GPS_HandmadeOff = false;
#endif
			RandomHopStartChannel_SlaveDefaultHoppingChannel();
		} else {
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	}

	Running_TimeCount = 0;
	SaveRecord_WriteInLoraMode();
	CLI_ClearHoppingRandomChannelNumber();

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraStartWork( shell_cmd_args *args )
{
	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	if(strcmp(args->args[0].val, "ENABLE") == 0)  {
		LoraStartWork = true;
	} else {
		if(strcmp(args->args[0].val, "DISABLE") == 0) {
			LoraStartWork = false;
		} else {
			return SHELL_PROCESS_ERR_CMD_UNKN;
		}
	}

	CLI_ClearHoppingRandomChannelNumber();
	// 可能需要增加當"LoraStartWork = true",設定1276模式,當"LoraStartWork = false"進入睡眠模式
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_LoraPower( shell_cmd_args *args )
{
	uint8_t		power;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	power = atoi(args->args[0].val);

	if( (power > MAX_LoraOutputPower) || (power < MIN_LoraOutputPower) ) return SHELL_PROCESS_ERR_CMD_UNKN;

	SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
	if(power > 17) {
		SX1276LoRaSetPa20dBm( true );
	} else {
		SX1276LoRaSetPa20dBm( false );
	}

	SX1276LoRaSetRFPower( power );
	LoRaSettings.Power = power;
	Radio->StartRx();
	// 以上可能需加上 IC 工作模式切換後再去做暫存器的設定,再根據LoraStartWork狀態去進入相關狀態

	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_UartEchoOn( shell_cmd_args *args )
{
	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	Console_EchoOn = true;
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd_UartEchoOff( shell_cmd_args *args )
{
	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	Console_EchoOn = false;
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__List_Hop_Channel_List( shell_cmd_args *args )
{
	List_FHSS_Channel_List();
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__Clear_Hop_Channel_List( shell_cmd_args *args )
{
	if( args->count != 0 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	Clear_FHSS_Channel_List();
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__Append_Hop_Channel( shell_cmd_args *args )
{
	uint32_t	freq;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	freq = (uint32_t) atoi( args->args[0].val );
	if( freq < MIN_RF_Freq || freq > MAX_RF_Freq ) return SHELL_PROCESS_ERR_CMD_UNKN;

	if( ! Append_FHSS_Channel_into_List( freq ) ) return SHELL_PROCESS_ERR_CMD_UNKN;

	CLI__Save_LoRa_Configure_into_Flash();
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__Read_CW_Offset( shell_cmd_args *args )
{
	char		cs[32];

	RF__CW_Offset = Flash_Read__CW_Offset();
	snprintf( cs, sizeof(cs), "CW_Offset= %d\r\n", RF__CW_Offset );
	Console_Output_String( cs );

	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__Write_CW_Offset( shell_cmd_args *args )
{
	char		cs[32];

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	RF__CW_Offset = atoi( args->args[0].val );
	if( Flash_Write__CW_Offset( RF__CW_Offset ) ) return( SHELL_PROCESS_OK );

	snprintf( cs, sizeof(cs), "Save CW Offset= %d got fail!\r\n", RF__CW_Offset );
	Console_Output_String( cs );
	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_testPrint1( shell_cmd_args *args )
{
	int16_t		offset;
	char		cs[32];

	Console_Output_String( "\r\ntestPrint1\r\n" );

	offset = Flash_Read__CW_Offset();
	snprintf( cs, sizeof(cs), "XTAL Offset= %d", offset );
	Console_Output_String(cs);

	return SHELL_PROCESS_OK;
}


int	CLI_ShellCmd_testPrint2( shell_cmd_args *args )
{
	Console_Output_String( "\r\ntestPrint2\r\n" );
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__SX127x_NRESET( shell_cmd_args *args )
{
	uint8_t		hl;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	hl = atoi(args->args[0].val);
	if( hl ) {
		SX1276SetReset( RADIO_RESET_ON );
	} else {
		SX1276SetReset( RADIO_RESET_OFF );
	}
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__SX127x_OpMode( shell_cmd_args *args )
{
	uint8_t		hl;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	hl = atoi(args->args[0].val);
	SX1276Write( REG_LR_OPMODE, hl );
	Console_Output_LoraMode( hl );
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__System_Standby( shell_cmd_args *args )
{
	uint16_t		duration_sec;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	duration_sec = atoi( args->args[0].val );
	SLEEP_SlaveSleep_STANDBY_Mode( & duration_sec );
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__System_Stop( shell_cmd_args *args )
{
	uint16_t		duration_sec;

	if( args->count != 1 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	duration_sec = atoi( args->args[0].val );
// 	SLEEP_SlaveSleep_STOP_Mode( & duration_sec );
	SLEEP_SlaveSleep_Deep_STOP_Mode( & duration_sec );
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__Dump_SX127x_Regs( shell_cmd_args *args )
{
	uint8_t		buf[0x80];

	SX1276Read( 0, buf );
	SX1276ReadBuffer( 1, buf + 1, 0x70 - 1 );
	Console_Output_String( "\r\n Dumping Lora Regsister\r\n" );
	Console_Dump_Binary( buf, 0x70 );
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__Dump_STM32F401_OTP( shell_cmd_args *args )
{
	uint8_t		buf[ 16 * ( 16 + 2 ) ];

	FLASH_ReadByte( 0x1FFF7800, buf, 16 * 17 );
	Console_Output_String( "\r\n Dumping OTP Buffer\r\n" );
	Console_Dump_Binary( buf, 16*17 );
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__Write_OTP( shell_cmd_args *args )
{
	uint8_t	addr, val;

	if( args->count != 2 )	return SHELL_PROCESS_ERR_CMD_UNKN;

	addr = atoi( args->args[0].val );
	val = atoi( args->args[1].val );

	FLASH_WriteByte( 0x1FFF7800 + addr, & val, 1 );
	return SHELL_PROCESS_OK;
}

int	CLI_ShellCmd__Read_Device_Info( shell_cmd_args *args )
{
	char		buf[64];

	snprintf( buf, sizeof( buf ), "\r\nDevice ID:0x%08X\r\n", MCU__Read_Device_ID() );
	Console_Output_String( buf );
	snprintf( buf, sizeof( buf ), "Device RDP:0x%08X\r\n", MCU__Read_Device_RDP() );
	Console_Output_String( buf );
	snprintf( buf, sizeof( buf ), "Device WRP:0x%08X\r\n", MCU__Read_Device_WRP() );
	Console_Output_String( buf );
	return SHELL_PROCESS_OK;
}

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
