/**
  ******************************************************************************
  * @file  aspep.h
  * @author  Motor Control SDK Team, STMicroelectronics
  * @brief  This file provides aspep API that implement the aspep protocol
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef ASPEP_SLAVE_H
#define ASPEP_SLAVE_H

#include "aspep_slave_conf.h"

#include "aspep_slave_tl.h"
#include <stdbool.h>

#define ASPEP_CTRL                ((uint8_t)0) /* Beacon, Nack, or Ping*/

#define ASPEP_OK                  ((uint8_t)0U)
#define ASPEP_SYNC_NOT_EXPECTED   1U
#define ASPEP_NOT_CONNECTED       2U
#define ASPEP_BUFFER_ERROR        3U

#define ASPEP_BAD_PACKET_TYPE     1U
#define ASPEP_BAD_PACKET_SIZE     2U
#define ASPEP_BAD_CRC_HEADER      4U
#define ASPEP_BAD_CRC_DATA        5U

#define ASPEP_PING_RESET          0U
#define ASPEP_PING_CFG            1U

#define ASPEP_HEADER_SIZE         4U
#define ASPEP_CTRL_SIZE           4U
#define ASPEP_DATACRC_SIZE        2U

#define ID_MASK                   ((uint32_t)0xFU)
#define DATA_PACKET               ((uint32_t)0x9U)
#define PING                      ((uint32_t)0x6U)
#define BEACON                    ((uint32_t)0x5U)
#define NACK                      ((uint32_t)0xFU)
#define ACK                       ((uint32_t)0xAU)

/* Define buffer dimensions according to application settings */
#define ASPEP_TX_SYNC_BUFFER_SIZE     (ASPEP_TX_SYNC_PAYLOAD_MAX + ASPEP_HEADER_SIZE + ASPEP_DATACRC_SIZE)
#define ASPEP_RX_SYNC_BUFFER_SIZE     (ASPEP_RX_SYNC_PAYLOAD_MAX + ASPEP_DATACRC_SIZE) /* ASPEP_HEADER_SIZE is not stored in the RX buffer. */
#define ASPEP_TX_ASYNC_BUFFER_SIZE    (ASPEP_TX_ASYNC_PAYLOAD_MAX_A + ASPEP_HEADER_SIZE + ASPEP_DATACRC_SIZE)

/* Set to 1 in aspep_slave_conf.h to enable ASPEP callbacks */
#ifndef ASPEP_CALLBACKS_ENABLE
#define ASPEP_CALLBACKS_ENABLE             0
#endif

/* Set to 1 in aspep_slave_conf to disable internal ASYNC buffers */
#ifndef ASPEP_USE_EXTERNAL_ASYNC_BUFFERS
#define ASPEP_USE_EXTERNAL_ASYNC_BUFFERS   0
#endif

/* Set to 0 in aspep_slave_conf to disable DMA */
#ifndef ASPEP_USE_DMA
#define ASPEP_USE_DMA                      1
#endif

#if ASPEP_USE_EXTERNAL_ASYNC_BUFFERS == 1
#if ASPEP_CALLBACKS_ENABLE == 0
#error "ASPEP_USE_EXTERNAL_ASYNC_BUFFERS only works if callbacks are enabled. Set ASPEP_CALLBACKS_ENABLE to 1"
#endif
#endif

/* ASPEP Payload Type for Data packets (11 bit) --> [0x0 - 0xEFF] */
#define ASPEP_PAYLOAD_TYPE_MCP          0x0U
#define ASPEP_PAYLOAD_TYPE_SSTL         0x7U

/* ASPEP Payload Type, default is MCP (Motor Control Protocol) */
#ifndef ASPEP_PAYLOAD_TYPE
#define ASPEP_PAYLOAD_TYPE              ASPEP_PAYLOAD_TYPE_MCP
#endif

typedef uint32_t ASPEP_packetType;

typedef bool (*ASPEP_send_cb_t)(void *pHW_Handle, void *txbuffer, uint16_t length);
typedef void (*ASPEP_receive_cb_t)(void *pHW_Handle, void *rxbuffer, uint16_t length);
typedef void (*ASPEP_hwinit_cb_t)(void *pHW_Handle);
typedef void (*ASPEP_hwsync_cb_t)(void *pHW_Handle);

/* ASPEP Callback declaration */
typedef void (*ASPEP_packet_received_cb_t)(void);
typedef void (*ASPEP_tx_complete_cb_t)(void);

/* ASPEP Sate Machine Type*/
typedef enum
{
  ASPEP_IDLE,
  ASPEP_CONFIGURED,
  ASPEP_CONNECTED,
} ASPEP_sm_type;

/* ASPEP Transport Layer Sate Machine Type*/
typedef enum
{
  WAITING_PACKET,
  WAITING_PAYLOAD,
} ASPEP_TL_sm_type;

typedef struct
{
  uint8_t buffer[ASPEP_HEADER_SIZE];
  volatile buff_access_t state;
} ASPEP_ctrlBuff_t;

typedef struct
{
  uint8_t DATA_CRC;
  uint8_t RX_maxSize;
  uint8_t TXS_maxSize;
  uint8_t TXA_maxSize;
  uint8_t version;
} ASPEP_Capabilities_def;

typedef struct
{
  /** ASPEP transport layer handler **/
  ASPEPTL_Handle_t _Super;

  /**  **/
  void *HWIp;

  /** ASPEP Data payload **/
  uint8_t *rxBuffer;

  /** Contains the ASPEP 32 bits header **/
  uint8_t rxHeader[4];

  ASPEP_ctrlBuff_t ctrlBuffer;
  ASPEPTL_Buff_t syncBuffer;
#if ASPEP_USE_EXTERNAL_ASYNC_BUFFERS == 0
  ASPEPTL_Buff_t asyncBufferA;
  ASPEPTL_Buff_t asyncBufferB;
  ASPEPTL_Buff_t *lastRequestedAsyncBuff;
  ASPEPTL_Buff_t *asyncNextBuffer;
#endif
  void *lockBuffer;
  ASPEP_hwinit_cb_t fASPEP_HWInit;
  ASPEP_hwsync_cb_t fASPEP_HWSync;
  ASPEP_receive_cb_t fASPEP_receive;
  ASPEP_send_cb_t fASPEP_send;
  uint16_t rxLength;
  uint16_t maxRXPayload;
  uint8_t syncPacketCount;
  bool NewPacketAvailable;
  uint8_t badPacketFlag; /* Contains the error code in case of ASPEP decoding issue */
  uint8_t liid;
  ASPEP_sm_type ASPEP_State;
  ASPEP_TL_sm_type ASPEP_TL_State;
  ASPEP_packetType rxPacketType;
  ASPEP_Capabilities_def Capabilities;

#if ASPEP_CALLBACKS_ENABLE == 1
  ASPEP_packet_received_cb_t ASPEP_PacketReceived_cb;
  ASPEP_tx_complete_cb_t ASPEP_tx_complete_cb;
#endif

#if ASPEP_USE_EXTERNAL_ASYNC_BUFFERS == 1
  uint8_t packet_type;
#endif
} ASPEP_Handle_t;


void ASPEP_start(ASPEP_Handle_t *pHandle);
/* ASPEPTL (ASPEP Transport Layer) API */
bool ASPEP_getBuffer(ASPEPTL_Handle_t *pHandle, void **buffer, uint8_t syncAsync);
uint8_t ASPEP_sendPacket(ASPEPTL_Handle_t *pHandle, void *txBuffer, uint16_t txDataLength, uint8_t syncAsync);
uint8_t *ASPEP_RXframeProcess(ASPEPTL_Handle_t *pHandle, uint16_t *packetLength);
/*   */
void ASPEP_HWDataReceivedIT(ASPEP_Handle_t *pHandle);
void ASPEP_HWDataTransmittedIT(ASPEP_Handle_t *pHandle);
/* Debugging */
void ASPEP_HWDMAReset(ASPEP_Handle_t *pHandle);

/* Application Callbacks */
#if ASPEP_CALLBACKS_ENABLE == 1
void ASPEP_RegisterNewPacket_cb(ASPEP_Handle_t *pHandle, ASPEP_packet_received_cb_t ASPEP_receive_new_packet_cb);
void ASPEP_RegisterTxComplete_cb(ASPEP_Handle_t *pHandle, ASPEP_tx_complete_cb_t ASPEP_tx_complete_cb);
#endif

/* IRQ handlers when DMA is used*/
void UASPEP_DMA_Rx_Handler(void *pHWHandle);
void UASPEP_Usart_Handler(void *pHWHandle);

/* IRQ handlers when DMA is not used */
void UASPEP_UART_Rx_Handler(void *pHWHandle);
void UASPEP_UART_Tx_Handler(void *pHWHandle);

#endif /* ASPEP_SLAVE_H */

/************************ (C) COPYRIGHT 2022 STMicroelectronics *****END OF FILE****/
