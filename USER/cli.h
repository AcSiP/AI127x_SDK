
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_CLI_H_
#define USER_CLI_H_

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "shell.h"
#include "sx1276-LoRaMisc.h"
#include "sx1276-LoRa.h"
#include "sx1276.h"

#ifdef STM32F072
	#include "stm32f0xx.h"
#endif

#ifdef STM32F401xx
	#include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int		CLI_process(char *);
uint32_t	CLI_LoraTimeOutCalculate(tLoRaSettings *);
void		CLI_FactoryReset(void);
void		CLI_ClearHoppingRandomChannelNumber(void);
void		CLI_DeleteAllLoraNodeMemorySpace(void);
void		CLI_DeleteLoraGateWayMemorySpace(void);
void		CLI_LoraPV_TXresult(void);
void		CLI_LoraOP_SF7BW250RS120(bool);
void		CLI_LoraOP_SF7BW125RS123(bool);
void		CLI_LoraOP_SF8BW500RS120(bool);
void		CLI_LoraOP_SF8BW250RS123(bool);
void		CLI_LoraOP_SF8BW125RS126(bool);
void		CLI_LoraOP_SF9BW500RS123(bool);
void		CLI_LoraOP_SF9BW250RS126(bool);
void		CLI_LoraOP_SF9BW125RS129(bool);
void		CLI_LoraOP_SF10BW500RS126(bool);
void		CLI_LoraOP_SF10BW250RS129(bool);
void		CLI_LoraOP_SF10BW125RS132(bool);
void		CLI_LoraOP_SF11BW250RS132(bool);
void		CLI_LoraOP_SF11BW125RS135(bool);
void		CLI_LoraOP_SF12BW250RS134(bool);
void		CLI_LoraOP_SF12BW125RS137(bool);
int		CLI_GPIO_Examine(shell_cmd_args *, uint8_t *, uint16_t *);
int		CLI_ShellCmd_GetBatteryADC(shell_cmd_args *);
int		CLI_ShellCmd_SystemReset(shell_cmd_args *);
int		CLI_ShellCmd_GPIO_Set(shell_cmd_args *);
int		CLI_ShellCmd_GPIO_Write(shell_cmd_args *);
int		CLI_ShellCmd_GPIO_Read(shell_cmd_args *);
int		CLI_ShellCmd_CmdPortOff(shell_cmd_args *);
int		CLI_ShellCmd_FskPV_TXstart(shell_cmd_args *);
int		CLI_ShellCmd_FskPV_TXstop(shell_cmd_args *);
int		CLI_ShellCmd_FskPV_Freq(shell_cmd_args *);
int		CLI_ShellCmd_LoraPV_TXresult(shell_cmd_args *);
int		CLI_ShellCmd_LoraPV_TXstart(shell_cmd_args *);
int		CLI_ShellCmd_LoraPV_RXresult(shell_cmd_args *);
int		CLI_ShellCmd_LoraPV_RXstart(shell_cmd_args *);
int		CLI_ShellCmd_LoraJoinNode(shell_cmd_args *);
int		CLI_ShellCmd_LoraJoinNode_in_Force( shell_cmd_args *args );
int		CLI_ShellCmd_LoraLeaveNode(shell_cmd_args *);
int		CLI_ShellCmd_LoraLeaveNode_in_Force( shell_cmd_args *args );
int		CLI_ShellCmd_LoraSetNodePara(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetNodePara(shell_cmd_args *);
int		CLI_ShellCmd_LoraNodeData(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetAllNodeAddr(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetGateWayAddr(shell_cmd_args *);
int		CLI_ShellCmd_OutputTraceData(shell_cmd_args *);
int		CLI_ShellCmd_FormatTraceDataSpace(shell_cmd_args *);
int		CLI_ShellCmd_BleEnable(shell_cmd_args *);
int		CLI_ShellCmd_BleDisable(shell_cmd_args *);
int		CLI_ShellCmd_GpsEnable(shell_cmd_args *);
int		CLI_ShellCmd_GpsDisable(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetMyAddr(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetMyInterval(shell_cmd_args *);
int		CLI_ShellCmd_GetUUID(shell_cmd_args *);
int		CLI_ShellCmd_GetFWVersion(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetSystemMode( shell_cmd_args *);
int		CLI_ShellCmd_LoraGetMode( shell_cmd_args *);
int		CLI_ShellCmd_LoraGetPara(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetRFOsc(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetRFFrequency(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetRxPacketTimeout(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetTxPacketTimeout(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetRFPower(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetPreambleLength(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetPayloadLength(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetCRC(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetImplicitHeader(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetFreqHopOn(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetErrorCoding(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetSF(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetBW(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetRxGain(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetSNR(shell_cmd_args *);
int		CLI_ShellCmd_LoraGetRSSI(shell_cmd_args *);
int		CLI_ShellCmd_FactoryReset( shell_cmd_args *);
int		CLI_ShellCmd_LoraSystemMode(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF7BW250RS120(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF7BW125RS123(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF8BW500RS120(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF8BW250RS123(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF8BW125RS126(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF9BW500RS123(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF9BW250RS126(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF9BW125RS129(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF10BW500RS126(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF10BW250RS129(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF10BW125RS132(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF11BW250RS132(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF11BW125RS135(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF12BW250RS134(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF12BW125RS137(shell_cmd_args *);
int		CLI_ShellCmd_LoraFreq(shell_cmd_args *);
int		CLI_ShellCmd_LoraBW(shell_cmd_args *);
int		CLI_ShellCmd_LoraSF(shell_cmd_args *);
int		CLI_ShellCmd_LoraErrorCoding(shell_cmd_args *);
int		CLI_ShellCmd_LoraFreqHop(shell_cmd_args *);
int		CLI_ShellCmd_LoraFreqHopOn(shell_cmd_args *);
int		CLI_ShellCmd_LoraImplicitHeader(shell_cmd_args *);
int		CLI_ShellCmd_LoraCRC(shell_cmd_args *);
int		CLI_ShellCmd_LoraPayloadLength(shell_cmd_args *);
int		CLI_ShellCmd_MaxLoraPayloadLength( shell_cmd_args *args );
int		CLI_ShellCmd_LoraPreambleLength(shell_cmd_args *);
int		CLI_ShellCmd_LoraMode(shell_cmd_args *);
int		CLI_ShellCmd_LoraStartWork(shell_cmd_args *);
int		CLI_ShellCmd_LoraPower(shell_cmd_args *);
int		CLI_ShellCmd_UartEchoOn(shell_cmd_args *);
int		CLI_ShellCmd_UartEchoOff(shell_cmd_args *);
int		CLI_ShellCmd__SX127x_NRESET( shell_cmd_args *args );
int		CLI_ShellCmd__SX127x_OpMode( shell_cmd_args *args );
int		CLI_ShellCmd__System_Standby( shell_cmd_args *args );
int		CLI_ShellCmd__System_Stop( shell_cmd_args *args );
int		CLI_LoraReInit( shell_cmd_args *args );
int		CLI_ShellCmd__Dump_SX127x_Regs( shell_cmd_args *args );
int		CLI_ShellCmd__Read_Device_Info( shell_cmd_args *args );
int		CLI_ShellCmd_testPrint1(shell_cmd_args *);
int		CLI_ShellCmd_testPrint2(shell_cmd_args *);

#endif		// USER_CLI_H_

/************************ Copyright 2016(C) AcSiP Technology Inc. *****END OF FILE****/
