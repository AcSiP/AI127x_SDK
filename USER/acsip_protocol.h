/**
  ******************************************************************************
  * @file    Project/ARM-Lora/acsip_protocol.h 
  * @author  JC
  * @version 
  * @date    
  * @brief   Header for acsip_protocol.c module
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ACSIP_PROTOCOL_H
#define __ACSIP_PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <time.h>
#ifdef STM32F072
  #include "stm32f0xx.h"
#endif
#ifdef STM32F401xx
  #include "stm32f4xx.h"
#endif

/* Exported types ------------------------------------------------------------*/
//Master Event identifier
#define Master_AcsipProtocol_Broadcast           (0x9A)
#define Master_AcsipProtocol_Idle                (0xA0)
#define Master_AcsipProtocol_Join                (0xA5)
#define Master_AcsipProtocol_Poll                (0xAA)
#define Master_AcsipProtocol_Data                (0xAF)
#define Master_AcsipProtocol_Leave               (0xB0)
#define Master_AcsipProtocol_Interval            (0xB5)
//General parameters definition
#define MAX_LoraNodeNum                          (7)
//Command parameters definition
#define CMD_JoinNetworkReq                       ("JReq")
#define CMD_JoinNetworkResOK                     ("JOK")
#define CMD_JoinNetworkResNOK                    ("JNOK")
#define CMD_LeaveNetworkReq                      ("LReq")
#define CMD_LeaveNetworkResOK                    ("LOK")
#define CMD_LeaveNetworkResNOK                   ("LNOK")
#define CMD_IntervalResOK                        ("InOK")
#define CMD_IntervalResNOK                       ("InNOK")
//Function status identifier
#define AcsipProtocol_NoAction                   (0x05)
#define AcsipProtocol_OK                         (0x10)
#define AcsipProtocol_FlagError                  (0x25)
#define AcsipProtocol_SnError                    (0x30)
#define AcsipProtocol_NodeAddressError           (0x35)
#define AcsipProtocol_CommandError               (0x40)
#define AcsipProtocol_LoraDeviceNodeFull         (0x45)
#define AcsipProtocol_UnknownLoraDeviceNode      (0x50)
#define AcsipProtocol_MemorySpaceError           (0x55)
//Max message data size
#define MaxPacketSize                            (255)
#define MaxMsgDataSize                           (244)     // 32(Total Size = MaxPacketSize) - 8(Length�BFlag�BSN�BNodeAdd�BCRC) - 3(MyAdd) = 21
#define AliasesSize                              (32)
#define AliasesDoubleSize                        (AliasesSize * 2)

//SX1276 LoRa General parameters definition
typedef struct sAcsipProtocolFrame {
  uint8_t FrameFlag;
  uint8_t FrameSN;
  uint8_t NodeAddress[3];
  uint8_t MyAddress[3];
  uint8_t MsgLength;
  uint8_t MsgData[MaxMsgDataSize];
  uint16_t FrameCRC;
}tAcsipProtocolFrame;

typedef enum {
  FrameFlag_Broadcast,
  FrameFlag_Join,
  FrameFlag_JoinResponse,
  FrameFlag_Poll,
  FrameFlag_PollResponse,
  FrameFlag_Data,
  FrameFlag_DataResponse,
  FrameFlag_Leave,
  FrameFlag_LeaveResponse,
  FrameFlag_Interval,
  FrameFlag_IntervalResponse,
}tFrameFlagType;

typedef struct sLoraDeviceNode {
	uint8_t   NodeAddress[3];
	uint8_t   PacketSN;
  uint8_t   Aliases[AliasesSize];
  uint16_t  Interval;
	double    RSSI;             //MASTER�PSLAVE���|�Ψ�
}tLoraDeviceNode;

typedef struct sDeviceNodeSensor {
	int32_t   GPS_Latitude;
	int32_t   GPS_Longitude;
	time_t    UTC;
	uint8_t   Battery;
  double    RSSI;             //��MASTER��,�u�ΨӰO�����쪺�P����ƫʥ]��RSSI��
}tDeviceNodeSensor;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
tLoraDeviceNode **AcsipProtocol_ReadLoraDeviceNodeStatus(void);
tDeviceNodeSensor **AcsipProtocol_ReadDeviceNodeSensorStatus(void);
uint8_t *AcsipProtocol_ReadDeviceNodeConnectedCounter(void);
tLoraDeviceNode *AcsipProtocol_ReadLoraGatewayStatus(void);
tDeviceNodeSensor *AcsipProtocol_ReadMySensorStatus(void);
void AcsipProtocol_SetMyAddress(void);
uint8_t *AcsipProtocol_ReadMyAddress(void);
void AcsipProtocol_FrameToPacketProcess(__IO tAcsipProtocolFrame *, __IO uint8_t *, uint8_t *);
void AcsipProtocol_PacketToFrame(__IO tAcsipProtocolFrame *, __IO uint8_t *);
bool AcsipProtocol_isMyPacket(uint8_t *);
bool AcsipProtocol_PacketCRCisCorrect(__IO uint8_t *, uint8_t);
void AcsipProtocol_MsgEncryption(uint8_t, __IO uint8_t *, uint8_t);
void AcsipProtocol_MsgDecryption(uint8_t, __IO uint8_t *, uint8_t);
bool AcsipProtocol_PacketToFrameProcess(__IO uint8_t *, uint8_t, __IO tAcsipProtocolFrame *);
void AcsipProtocol_LoraJoinNetworkRequest(__IO uint8_t *, __IO tAcsipProtocolFrame *, __IO uint8_t *, uint8_t *);
uint8_t AcsipProtocol_LoraJoinNetworkResponseCB(__IO tAcsipProtocolFrame *, __IO uint8_t *);
uint8_t AcsipProtocol_LoraJoinNetworkResponse(__IO tAcsipProtocolFrame *, const bool, __IO tAcsipProtocolFrame *, uint8_t *, uint8_t *);
void AcsipProtocol_LoraPollRequest(tLoraDeviceNode *, __IO tAcsipProtocolFrame *, __IO uint8_t *, uint8_t *);
uint8_t AcsipProtocol_LoraPollResponseCB(tLoraDeviceNode *, tDeviceNodeSensor *, __IO tAcsipProtocolFrame *);
uint8_t AcsipProtocol_LoraPollResponse(__IO tAcsipProtocolFrame *, __IO tAcsipProtocolFrame *, uint8_t *, uint8_t *);
void AcsipProtocol_LoraDataRequest(tLoraDeviceNode *, __IO uint8_t *, __IO uint8_t *, __IO tAcsipProtocolFrame *, __IO uint8_t *, uint8_t *);
uint8_t AcsipProtocol_LoraDataResponseCB(tLoraDeviceNode *, __IO tAcsipProtocolFrame *);
uint8_t AcsipProtocol_LoraDataResponse(__IO tAcsipProtocolFrame *, uint8_t *, uint8_t *, __IO tAcsipProtocolFrame *, uint8_t *, uint8_t *);
void AcsipProtocol_LoraLeaveNetworkRequest(tLoraDeviceNode *, __IO tAcsipProtocolFrame *, __IO uint8_t *, uint8_t *);
uint8_t AcsipProtocol_LoraLeaveNetworkResponseCB(tLoraDeviceNode *, __IO tAcsipProtocolFrame *);
uint8_t AcsipProtocol_LoraLeaveNetworkResponse(__IO tAcsipProtocolFrame *, const bool, __IO tAcsipProtocolFrame *, uint8_t *, uint8_t *);
void AcsipProtocol_LoraIntervalRequest(tLoraDeviceNode *, __IO tAcsipProtocolFrame *, __IO uint8_t *, uint8_t *);
uint8_t AcsipProtocol_LoraIntervalResponseCB(tLoraDeviceNode *, __IO tAcsipProtocolFrame *);
uint8_t AcsipProtocol_LoraIntervalResponse(__IO tAcsipProtocolFrame *, __IO tAcsipProtocolFrame *, uint8_t *, uint8_t *);



#endif /* __ACSIP_PROTOCOL_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


