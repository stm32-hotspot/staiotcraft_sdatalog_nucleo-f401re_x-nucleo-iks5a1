/**
  ******************************************************************************
  * @file    simple_serial_tl.h
  * @author  STMicroelectronics
  * @brief   Simple Serial Transport Layer include file
  *
  * Simple Serial Transport Layer (SSTL)
  *
  * This Simple point-to-point protocol allows to transfer packets on a serial
  * link, like a UART peripheral. It's a master/slave protocol that supports
  * both synchronous and asynchronous packets. It's tailored specifically to
  * multi-channel data streaming applications.
  * Synchronous packets always start with a request from the master, followed by
  * a response from the slave.
  * Asynchronous packets can be sent by the slave (data source) to the master.
  * The protocol takes care of the segmentation and reassembly of synchronous
  * packets that are too big to fit in lower level payload (ASPEP protocol).
  * To maintain a certain level of efficiency, the segmentation and reassembly
  * feature is not supported for asynchronous packets. Maximum ASPEP payload for
  * async packet is 8192 Bytes, but it may vary depending on the device
  * capabilities, and it's negotiated during the handshake phase at the
  * beginning.
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef SIMPLE_SERIAL_TL_H
#define SIMPLE_SERIAL_TL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "simple_serial_tl_conf.h"
#include "aspep_slave.h"


#define SSTL_HEADER_SIZE               4U
#define SSTL_MAX_TX_SYNC_PAYLOAD       (ASPEP_TX_SYNC_PAYLOAD_MAX - SSTL_HEADER_SIZE)
#define SSTL_MAX_ASYNC_PAYLOAD         (ASPEP_TX_ASYNC_PAYLOAD_MAX_A - SSTL_HEADER_SIZE)

#define SSTL_STATUS_IDLE               0U
/* #define SSTL_STATUS_RECEIVING          1U */
#define SSTL_STATUS_SYNC_TX            2U
#define SSTL_STATUS_SYNC_TX_LAST       3U
/* #define SSTL_STATUS_ASYNC_TX           4U */
#define SSTL_STATUS_ASYNC_TX_LAST      5U

#define SSTL_RX_STATUS_IDLE            0U


/* CR Values */
#define SSTL_HEADER_CR_REQUEST         0x00000020U
#define SSTL_HEADER_CR_RESPONSE        0x00000040U
#define SSTL_HEADER_CR_CUSTOM          0x00000080U
#define SSTL_HEADER_CR_TEXTUAL         0x000000E0U

#define SSTL_HEADER_FINAL_FRAME        0x00000100U

#define SSTL_HEADER_VERSION            0x0U

#define SSTL_HEADER_CHANNEL_MASK       0x0000FE00U
#define SSTL_HEADER_CHANNEL(A)         (((uint32_t)(A) << 9) & SSTL_HEADER_CHANNEL_MASK)

#define SSTL_HEADER_TOTAL_SIZE_MASK    0xFFFFFE00U
#define SSTL_HEADER_TOTAL_SIZE(A)      (((uint32_t)(A) << 9) & SSTL_HEADER_TOTAL_SIZE_MASK)

#define SSTL_HEADER_SEQUENCE_MASK      0xFFFF0000U
#define SSTL_HEADER_SEQUENCE(A)        (((uint32_t)(A) << 16) & SSTL_HEADER_SEQUENCE_MASK)

#define SSTL_SINGLE_FRAME_RESPONSE     (SSTL_HEADER_VERSION|SSTL_HEADER_FINAL_FRAME|SSTL_HEADER_CR_RESPONSE)


#define SSTL_CR_FILL()  (a)

/* Return values */
#define SSTL_OK                       0U  /* rx/tx API completed */
#define SSTL_GENERIC_ERROR            1U
#define SSTL_BUSY                     2U  /* rx/tx API busy */
#define SSTL_LL_PROTOCOL_ERROR        3U
#define SSTL_NO_DATA_AVAILABLE        4U
#define SSTL_PARTIAL_PACKET           5U
#define SSTL_NEW_PACKET_AVAILABLE     6U
#define SSTL_ACK_RECEIVED             7U
#define SSTL_TIMEOUT                  8U   /* rx/tx API timeout expired */
#define SSTL_TX_IN_PROGRESS           9U   /* tx proc started but not yet ended */
/* SSTL API timeout argument */
#define SSTL_BLOCKING                 0xFFFFFFFF  /* blocking mode call */
#define SSTL_NON_BLOCKING             0x0         /* non blocking mode call */

#ifndef SSTL_CALLBACKS_ENABLE
#define SSTL_CALLBACKS_ENABLE                   0
#endif /* SSTL_CALLBACKS_ENABLE */

#ifndef SSTL_USE_EXTERNAL_ASYNC_BUFFERS
#define SSTL_USE_EXTERNAL_ASYNC_BUFFERS         0
#endif /* SSTL_USE_EXTERNAL_ASYNC_BUFFERS */

#if SSTL_USE_EXTERNAL_ASYNC_BUFFERS == 1
#if ASPEP_USE_EXTERNAL_ASYNC_BUFFERS == 0
#error "You should enable External ASYNC buffers in ASPEP protocol as well: ASPEP_USE_EXTERNAL_ASYNC_BUFFERS = 1"
#endif /* ASPEP_USE_EXTERNAL_ASYNC_BUFFERS */
#endif /* SSTL_USE_EXTERNAL_ASYNC_BUFFERS */


/**
  * Async Payload type enumeration.
  */
typedef enum
{
  SSTL_ASYNC_PAYLOAD_TYPE_HSD = 0,
  SSTL_ASYNC_PAYLOAD_TYPE_CUSTOM,
  SSTL_ASYNC_PAYLOAD_TYPE_TEXTUAL
} SSTL_AsyncPayloadType_t;


/**
  * Create  type name for struct _SimpleSerialTL_t.
  */
typedef struct _SimpleSerialTL_t SimpleSerialTL_t;

/* Simple Serial TL Callbacks declaration */
typedef void (*SSTL_FrameTransmitting_cb_t)(SimpleSerialTL_t *);
typedef void (*SSTL_FrameTxComplete_cb_t)(SimpleSerialTL_t *);
typedef void (*SSTL_FrameReceived_cb_t)(SimpleSerialTL_t *);
typedef void (*SSTL_PacketReceived_cb_t)(SimpleSerialTL_t *);

/**
  * ::SimpleSerialTL_t internal structure.
  */
struct _SimpleSerialTL_t
{
  /**
    * Pointer to lower level transport layer (ASPEP)
    */
  ASPEPTL_Handle_t *p_aspeptl;

  /**
    * Instance status
    */
  volatile uint8_t status;  /* must be volatile because of optimization */

  /**
    * Flag to notify when a new packet is received
    */
  bool new_packet_received;

  /**
    * Pointer to rx packet buffer to be allocated on new packet reception and
    * filled with received payload
    */
  uint8_t *p_rx_packet;

  /**
    * Pointer to the allocated rx packet buffer
    * This is used when the packet is segmented in multiple frames
    */
  uint8_t p_total_rx_packet[ASPEP_RX_SYNC_PAYLOAD_MAX];

  /**
    * Total byte counter for receiving packet (entire message)
    */
  uint32_t rx_packet_size;

  /**
    * Byte counter for receiving packet
    */
  uint32_t rx_bytes_received;

  /**
    * Frame counter for receiving packet
    */
  uint32_t rx_frame_counter;

  /**
    * Pointer to the payload to be sent
    */
  uint8_t *p_tx_payload;

  /**
    * Size of the payload to be sent
    */
  uint32_t tx_payload_size;

  /**
    * Sequence number of the frame when payload segmentation is used
    */
  uint16_t tx_payload_sequence;

  /*
    * Max frame size of SYNC tx frames
    */
  uint16_t max_tx_sync_payload_size;

  /*
    * Max frame size of ASYNC tx frames
    */
  uint16_t max_tx_async_payload_size;

#if SSTL_CALLBACKS_ENABLE == 1
  SSTL_FrameTransmitting_cb_t FrameTransmitting_cb;

  SSTL_FrameTxComplete_cb_t FrameTxComplete_cb;

  SSTL_FrameReceived_cb_t FrameReceived_cb;

  SSTL_PacketReceived_cb_t PacketReceived_cb;
#endif /* SSTL_CALLBACKS_ENABLE */

};


/* Public API declaration */
/**************************/

/**
  * Allocate an instance of ::SimpleSerialTL_t.
  * Singleton design pattern is used
  *
  * @return a pointer to the new object ::SimpleSerialTL_t if success, or NULL if out of memory error occurs.
  */
SimpleSerialTL_t *SSTL_Alloc(void);

/**
  * Initialize the protocol object. It must be called after the object allocation and before using it.
  *
  * @param _this [IN] pointer to the SSTL object.
  * @return SSTL_OK if success, an error code otherwise.
  */
uint32_t SSTL_Init(SimpleSerialTL_t *_this);

/**
  * Return true if tx is completed, false if not, or if timeout occurred
  */
bool SSTL_isTxComplete(SimpleSerialTL_t *_this, uint32_t timeout);

/**
  * Set the payload buffer that needs to be sent and its size
  *
  * @param _this [IN] pointer to the SSTL object.
  * @param p_payload [IN] pointer to payload to be sent as a response.
  * @param size [IN] size of the payload.
  * @return SSTL_OK if success, an error code otherwise.
  */
uint32_t SSTL_SetResponse(SimpleSerialTL_t *_this, uint8_t *p_payload, uint32_t size);

/**
  * Send the previously set payload (::SSTL_SetResponse)
  * This function sends the complete payload only if it fits in the maximum size, otherwise
  * it manages the segmentation of the packet in multiple frames.
  * In case of segmentation, the function needs to be called again when an ACK packet
  * is received from the master. The ::SSTL_SetResponse function is only called one time at
  * the beginning.
  *
  * @param _this [IN] pointer to the SSTL object.
  * @return SSTL_OK if the complete payload has been sent
  *         SSTL_PARTIAL_PACKET if the packet was segmented and only one part was sent
  *         an error code otherwise.
  */
uint32_t SSTL_TxResponse(SimpleSerialTL_t *_this, uint32_t timeout);

/**
  * Send a packet without any payload (Ack)
  *
  * @param _this [IN] pointer to the SSTL object.
  * @return SSTL_OK if success, an error code otherwise.
  */
uint32_t SSTL_TxAck(SimpleSerialTL_t *_this, uint32_t timeout);

#if SSTL_USE_EXTERNAL_ASYNC_BUFFERS == 0
/**
  * Get the buffer where the application can put the payload to be sent as ASYNC packet.
  * This is to improve efficiency and avoid doing a memcpy of the buffer ads done for the
  * SYNC packets.
  * The buffer also has room for the SSTL Header and the ASPEP Header (lower level protocol)
  * but the returned pointer already points to the payload, the management of headers is
  * transparent to the user.
  *
  * @param _this [IN] pointer to the SSTL object.
  * @param p_buffer [OUT] pointer to the buffer where the application can put the packet payload.
  * @param size [OUT] size in Bytes of the buffer
  * @return SSTL_OK if success, an error code otherwise.
  */
bool SSTL_GetAsyncBuffer(SimpleSerialTL_t *_this, uint8_t **p_buffer, uint32_t *size);
#endif /* SSTL_USE_EXTERNAL_ASYNC_BUFFERS == 0 */

/**
  * Transmit an async packet on the specified channel.
  * The payload buffer passed to this function can either be
  *  - the one returned by ::SSTL_GetAsyncBuffer if SSTL_USE_EXTERNAL_ASYNC_BUFFERS == 0
  *  - any other buffer if SSTL_USE_EXTERNAL_ASYNC_BUFFERS == 1
  *
  * The payload buffer passed to this function must also have room in the front for the protocol
  * headers. The buffer returned by the ::SSTL_GetAsyncBuffer already includes this additional space
  *
  * In case of custom buffer here is a code example to send 100 Bytes of payload on channel 0:
  *
  * \code
  *
  *
  * // buffer                      payload
  * //   |                            |
  * //   +--------------+-------------+------------------------------------+
  * //   | ASPEP Header | SSTL Header |           Payload buffer           |
  * //   +--------------+-------------+------------------------------------+
  * //   |    4 Bytes   |   4 Bytes   |               N Bytes              |
  * //   +--------------+-------------+------------------------------------+
  *
  * uint8_t buffer[100 + SSTL_HEADER_SIZE + ASPEP_HEADER_SIZE]; // 100 + 4 + 4
  *
  * // Pointer to the payload
  * uint8_t *payload = &buffer[SSTL_HEADER_SIZE + ASPEP_HEADER_SIZE];
  *
  * fill_payload(payload);
  *
  * // Pass the pointer to the payload, not the complete buffer
  * SSTL_TxAsync(p_sstl, 0, SSTL_PAYLOAD_TYPE_HSD, payload, 100);
  *
  * \endcode
  *
  * @param _this [IN] pointer to the SSTL object.
  * @return SSTL_OK if success, an error code otherwise.
  */
uint32_t SSTL_TxAsync(SimpleSerialTL_t *_this, uint8_t channel, SSTL_AsyncPayloadType_t payload_type,
                      uint8_t *p_payload, uint32_t size, uint32_t timeout);

/**
  * Low priority RX process function.
  * For bare metal application it's normally called from a low priority IRQ (e.g. SysTick)
  * For RTOS applications can be called from a task. The task execution can be triggered by
  * the ::SSTL_PacketReceived_cb_t callback.
  *
  * @param _this [IN] pointer to the SSTL object.
  * @param p_packet [OUT] pointer to the buffer with the received packet
  * @param size [OUT] size in Bytes of the received packet
  * @return SSTL_NEW_PACKET_AVAILABLE if a complete packet has been received
  *         SSTL_NO_DATA_AVAILABLE if no packet has been received
  *         SSTL_ACK_RECEIVED if a ack packet was received (no payload)
  *         SSTL_PARTIAL_PACKET if a partial packet has been received
  */
uint32_t SSTL_ProcessRxFrame(SimpleSerialTL_t *_this, uint32_t timeout);
/**
  * Get the received packet.
  *
  * @param _this [IN] pointer to the SSTL object.
  * @param p_packet [OUT] pointer to the buffer with the received packet
  * @param size [OUT] size in Bytes of the received packet
  * @return SSTL_OK if success, an error code otherwise.
  */
uint32_t SSTL_GetRxPacket(SimpleSerialTL_t *_this, uint8_t **p_packet, uint32_t *size);

/**
  * Serve the DMA Interrupt
  */
void SSTL_DMA_isr(void);

/**
  * Serve the incoming USART Interrupt when DMA is used
  */
void SSTL_USART_isr(void);

/**
  * Serve the USART RX Interrupt when DMA is not used
  */
void SSTL_UART_rx_isr(void);

/**
  * Serve the USART TX Interrupt when DMA is not used
  */
void SSTL_UART_tx_isr(void);

/* Application Callbacks */
#if SSTL_CALLBACKS_ENABLE == 1
void SSTL_RegisterFrameTransmitting_cb(SimpleSerialTL_t *_this, SSTL_FrameTransmitting_cb_t cb);
void SSTL_RegisterTxComplete_cb(SimpleSerialTL_t *_this, SSTL_FrameTxComplete_cb_t cb);
void SSTL_RegisterFrameReceived_cb(SimpleSerialTL_t *_this, SSTL_FrameReceived_cb_t cb);
void SSTL_RegisterPacketReceived_cb(SimpleSerialTL_t *_this, SSTL_PacketReceived_cb_t cb);
#endif /* SSTL_CALLBACKS_ENABLE == 1 */

#ifdef __cplusplus
}
#endif

#endif /* SIMPLE_SERIAL_TL_H */
