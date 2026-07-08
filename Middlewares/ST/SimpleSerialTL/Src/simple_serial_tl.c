/**
  ******************************************************************************
  * @file    simple_serial_tl.c
  * @author  STMicroelectronics
  * @brief   Simple Serial Transport Layer
  *
  * @description
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
  ******************************************************************************
  */

#include "simple_serial_tl.h"
#include "string.h"

/* Private member function declaration */
/***************************************/

/* Callbacks macro definition, if enabled */
#if SSTL_CALLBACKS_ENABLE == 1

#define FRAME_TRANSMITTING_CALLBACK(A) \
  if((A)->FrameTransmitting_cb != NULL) \
    (A)->FrameTransmitting_cb(A);

#define FRAME_TX_COMPLETE_CALLBACK(A) \
  if((A)->FrameTxComplete_cb != NULL) \
    (A)->FrameTxComplete_cb(A);

#define FRAME_RX_COMPLETE_CALLBACK(A) \
  if((A)->FrameReceived_cb != NULL) \
    (A)->FrameReceived_cb(A);

#define PACKET_RECEIVED_CALLBACK(A) \
  if((A)->PacketReceived_cb != NULL) \
    (A)->PacketReceived_cb(A);

#else

#define FRAME_TRANSMITTING_CALLBACK(A)
#define FRAME_TX_COMPLETE_CALLBACK(A)
#define FRAME_RX_COMPLETE_CALLBACK(A)
#define PACKET_RECEIVED_CALLBACK(A)

#endif /* SSTL_CALLBACKS_ENABLE */

/**
  * SSTL object declaration (Singleton design pattern)
  */
static SimpleSerialTL_t sObj;

/**
  * ASPEP driver handler
  */
extern ASPEP_Handle_t aspepOverUartA;

/**
  * Transmission complete callback to be registered to ASPEP layer
  */
static void Aspep_tx_complete(void);

/**
  * New packet received callback to be registered to ASPEP layer
  */
static void Aspep_new_packet(void);


/* Public API definition */
/*************************/

SimpleSerialTL_t *SSTL_Alloc(void)
{
  return &sObj;
}


uint32_t SSTL_Init(SimpleSerialTL_t *_this)
{
  assert_param(_this != NULL);
  uint32_t res = SSTL_OK;
  ASPEP_Handle_t *p_aspep = &aspepOverUartA;

  /* Internal variables initialization */
  _this->p_aspeptl = &p_aspep->_Super;
  _this->status = SSTL_STATUS_IDLE;
  _this->new_packet_received = false;
  _this->tx_payload_size = 0;
  _this->tx_payload_sequence = 0;

  /* Set the maximum packet size according to ASPEP settings */
  _this->max_tx_sync_payload_size = _this->p_aspeptl->txSyncMaxPayload - SSTL_HEADER_SIZE;
  _this->max_tx_async_payload_size = _this->p_aspeptl->txAsyncMaxPayload - SSTL_HEADER_SIZE;

  ASPEP_start(p_aspep);

  /* Register the callbacks for the ASPEP layer */
  ASPEP_RegisterTxComplete_cb(p_aspep, Aspep_tx_complete);
  ASPEP_RegisterNewPacket_cb(p_aspep, Aspep_new_packet);

  return res;
}


uint32_t SSTL_SetResponse(SimpleSerialTL_t *_this, uint8_t *p_payload, uint32_t size)
{
  assert_param(_this != NULL);
  uint32_t res = SSTL_OK;

  /* Fill the object parameters with the given arguments */
  if (_this->status == SSTL_STATUS_IDLE)
  {
    _this->tx_payload_size = size;
    _this->p_tx_payload = p_payload;
  }
  else
  {
    res = SSTL_BUSY;
  }

  return res;
}

uint32_t SSTL_TxResponse(SimpleSerialTL_t *_this, uint32_t timeout)
{
  assert_param(_this != NULL);
  uint32_t res = SSTL_OK;
  uint8_t *p_aspep_buffer;
  ASPEPTL_Handle_t *p_aspeptl = _this->p_aspeptl;
  uint8_t *p_payload = _this->p_tx_payload + (_this->tx_payload_sequence * SSTL_MAX_TX_SYNC_PAYLOAD);
  uint32_t size = _this->tx_payload_size;
  uint32_t start_time = HAL_GetTick();
  uint32_t current_timeout = timeout;

  if ((size == 0U) || (p_payload == NULL)) /* No data to be sent */
  {
    res = SSTL_NO_DATA_AVAILABLE;
  }
  else if (size <= _this->p_aspeptl->txSyncMaxPayload - SSTL_HEADER_SIZE) /* The payload fits in a single frame */
  {
    /* Check if the previous transmission was completed.
     * In case there is an ongoing transmission and a timeout was specified, it waits here for it to be completed. */
    if (SSTL_isTxComplete(_this, timeout) == false)
    {
      return SSTL_BUSY;
    }

    /* Get an available SYNC buffer from the ASPEP protocol layer */
    if (p_aspeptl->fGetBuffer(p_aspeptl, (void **) &p_aspep_buffer, ASPEP_SYNC) == 0)
    {
      res = SSTL_LL_PROTOCOL_ERROR;
    }
    else
    {
      /* Copy the SSTL header into the ASPEP Sync buffer */
      uint32_t sstl_header = (SSTL_HEADER_VERSION | SSTL_HEADER_FINAL_FRAME | SSTL_HEADER_CR_RESPONSE | SSTL_HEADER_SEQUENCE(_this->tx_payload_sequence));
      (void)memcpy(p_aspep_buffer, &sstl_header, sizeof(sstl_header));  /* must be memcpy'ed because of TBAA optimization */

      /* Copy the payload into the ASPEP Sync buffer */
      (void)memcpy(&p_aspep_buffer[SSTL_HEADER_SIZE], p_payload, size);

      _this->status = SSTL_STATUS_SYNC_TX_LAST;

      _this->tx_payload_size = 0;
      _this->tx_payload_sequence = 0;

      /* Call the ASPEP function to send the frame */
      if (p_aspeptl->fSendPacket(p_aspeptl, (uint8_t *)p_aspep_buffer, size + SSTL_HEADER_SIZE, ASPEP_SYNC) != ASPEP_OK)
      {
        return SSTL_LL_PROTOCOL_ERROR;
      }

      /* Notify the application that a packet tx is in progress */
      FRAME_TRANSMITTING_CALLBACK(_this);

      if (timeout == SSTL_NON_BLOCKING)
      {
        /* TX in progress. Return now because the function was called in non-blocking mode */
        return SSTL_OK;
      }

      if (timeout != SSTL_BLOCKING)
      {
        if (timeout > (HAL_GetTick() - start_time))
        {
          /* Take into account the time already spent in this API */
          current_timeout = timeout - (HAL_GetTick() - start_time);
        }
        else
        {
          /* Elapsed time is greater than the timeout */
          return SSTL_TIMEOUT;
        }
      }

      if (SSTL_isTxComplete(_this, current_timeout))
      {
        return SSTL_OK;
      }
      return SSTL_TIMEOUT; /* tx timed out */
    }
  }
  else /* The payload needs to be segmented */
  {
    /* Check if the previous transmission was completed.
     * In case there is an ongoing transmission and a timeout was specified, it waits here for it to be completed. */
    if (SSTL_isTxComplete(_this, timeout) == false)
    {
      return SSTL_BUSY;
    }

    /* Get an available SYNC buffer from the ASPEP protocol layer */
    if (p_aspeptl->fGetBuffer(p_aspeptl, (void **) &p_aspep_buffer, ASPEP_SYNC) == 0)
    {
      res = SSTL_LL_PROTOCOL_ERROR;
    }
    else
    {
      uint32_t sstl_header;

      if (_this->tx_payload_sequence == 0U)
      {
        /* This is the first frame: the header includes the total size */
        sstl_header = (SSTL_HEADER_VERSION | SSTL_HEADER_CR_RESPONSE | SSTL_HEADER_TOTAL_SIZE(size));
      }
      else
      {
        /* Other frames: the header includes the sequence number */
        sstl_header = (SSTL_HEADER_VERSION | SSTL_HEADER_CR_RESPONSE | SSTL_HEADER_SEQUENCE(_this->tx_payload_sequence));
      }
      /* Copy the SSTL header into the ASPEP Sync buffer */
      (void)memcpy(p_aspep_buffer, &sstl_header, sizeof(sstl_header)); /* must be memcpy'ed because of TBAA optimization */

      /* Copy the payload into the ASPEP Sync buffer */
      (void)memcpy(&p_aspep_buffer[SSTL_HEADER_SIZE], p_payload, SSTL_MAX_TX_SYNC_PAYLOAD);

      _this->status = SSTL_STATUS_SYNC_TX;
      _this->tx_payload_size = size - SSTL_MAX_TX_SYNC_PAYLOAD;
      _this->tx_payload_sequence++;

      res = SSTL_PARTIAL_PACKET;

      /* Call the ASPEP function to send the packet */
      if (p_aspeptl->fSendPacket(p_aspeptl, (uint8_t *)p_aspep_buffer, SSTL_MAX_TX_SYNC_PAYLOAD + SSTL_HEADER_SIZE,
                                 ASPEP_SYNC) != ASPEP_OK)
      {
        return SSTL_LL_PROTOCOL_ERROR;
      }

      /* Notify the application that a packet tx is in progress */
      FRAME_TRANSMITTING_CALLBACK(_this);

      if (timeout == SSTL_NON_BLOCKING)
      {
        /* TX in progress. Return now because the function was called in non-blocking mode */
        return res + SSTL_OK; /* Keep track of SSTL_PARTIAL_PACKET return value */
      }

      if (timeout != SSTL_BLOCKING)
      {
        /* check if we already spent more than the timeout */
        if (timeout > (HAL_GetTick() - start_time))
        {
          /* Take into account the time already spent in this API */
          current_timeout = timeout - (HAL_GetTick() - start_time);
        }
        else
        {
          /* Elapsed time is greater than the timeout */
          return SSTL_TIMEOUT;
        }
      }

      if (SSTL_isTxComplete(_this, current_timeout))
      {
        return res + SSTL_OK; /* Keep track of SSTL_PARTIAL_PACKET return value */
      }

      /* TX timed out */
      return SSTL_TIMEOUT;
    }
  }
  return res;
}

uint32_t SSTL_TxAck(SimpleSerialTL_t *_this, uint32_t timeout)
{
  assert_param(_this != NULL);
  uint32_t res = SSTL_OK;
  uint8_t *p_aspep_buffer;
  ASPEPTL_Handle_t *p_aspeptl = _this->p_aspeptl;
  uint32_t start_time = HAL_GetTick();
  uint32_t current_timeout = timeout;

  /* Check if the previous transmission was completed.
   * In case there is an ongoing transmission and a timeout was specified, it waits here for it to be completed. */
  if (SSTL_isTxComplete(_this, timeout) == false)
  {
    return SSTL_BUSY;
  }

  /* Get an available SYNC buffer from the ASPEP protocol layer */
  if (p_aspeptl->fGetBuffer(p_aspeptl, (void **) &p_aspep_buffer, ASPEP_SYNC) == 0)
  {
    res = SSTL_LL_PROTOCOL_ERROR;
  }
  else
  {
    uint32_t *p_sstl_header = (uint32_t *) p_aspep_buffer;
    *p_sstl_header = SSTL_SINGLE_FRAME_RESPONSE;

    _this->status = SSTL_STATUS_SYNC_TX_LAST;

    /* Call the ASPEP function to send the packet */
    if (p_aspeptl->fSendPacket(p_aspeptl, p_aspep_buffer, SSTL_HEADER_SIZE, ASPEP_SYNC) != ASPEP_OK)
    {
      return SSTL_LL_PROTOCOL_ERROR;
    }

    /* Notify the application that a packet tx is in progress */
    FRAME_TRANSMITTING_CALLBACK(_this);

    if (timeout == SSTL_NON_BLOCKING)
    {
      /* TX in progress. Return now because the function was called in non-blocking mode */
      return SSTL_OK;
    }

    if (timeout != SSTL_BLOCKING)
    {
      /* check if we already spent more than the timeout */
      if (timeout > (HAL_GetTick() - start_time))
      {
        /* Take into account the time already spent in this API */
        current_timeout = timeout - (HAL_GetTick() - start_time);
      }
      else
      {
        /* Elapsed time is greater than the timeout */
        return SSTL_TIMEOUT;
      }
    }

    if (SSTL_isTxComplete(_this, current_timeout))
    {
      return SSTL_OK;
    }

    /* TX timed out */
    return SSTL_TIMEOUT;
  }
  return res;
}


uint32_t SSTL_GetRxPacket(SimpleSerialTL_t *_this, uint8_t **p_packet, uint32_t *size)
{
  assert_param(_this != NULL);
  assert_param(p_packet != NULL);
  assert_param(size != NULL);
  uint32_t res = SSTL_NO_DATA_AVAILABLE;

  /* Return the received buffer and it's size */
  if (_this->new_packet_received)
  {
    *p_packet = _this->p_total_rx_packet;
    *size = _this->rx_packet_size;
    res = SSTL_OK;
  }
  return res;
}


bool SSTL_isRxComplete(SimpleSerialTL_t *_this, uint32_t timeout, uint32_t start_time)
{
  assert_param(_this != NULL);
  (void)_this;

  if (timeout == SSTL_BLOCKING)
  {
    return false;
  }
  else
  {
    /* Non blocking call with timeout, return true if timedout */
    if (HAL_GetTick() > start_time + timeout)
    {
      /* Timeout */
      return true;
    }
    return false;
  }
}

uint32_t SSTL_ProcessRxFrame(SimpleSerialTL_t *_this, uint32_t timeout)
{
  assert_param(_this != NULL);
  uint8_t *rx_buffer;
  uint16_t size;
  uint32_t res = SSTL_NO_DATA_AVAILABLE;
  uint32_t *header;
  uint32_t start_time;

  start_time = HAL_GetTick();
  while (res == SSTL_NO_DATA_AVAILABLE)
  {
    /* Call the low priority packet process function of the ASPEP layer */
    rx_buffer = _this->p_aspeptl->fRXPacketProcess(_this->p_aspeptl, &size);
    if (rx_buffer != NULL)
    {
      header = (uint32_t *) rx_buffer;
      if (_this->rx_frame_counter == 0U) /* First frame */
      {
        if (header[0] & SSTL_HEADER_FINAL_FRAME) /* Final frame */
        {
          _this->rx_packet_size = size - (uint32_t) SSTL_HEADER_SIZE;
          if (_this->rx_packet_size > 0U) /* New packet */
          {
            _this->p_rx_packet = &rx_buffer[SSTL_HEADER_SIZE];
            res = SSTL_NEW_PACKET_AVAILABLE;
          }
          else /* Ack */
          {
            _this->p_rx_packet = 0;
            res = SSTL_ACK_RECEIVED;
          }
          memcpy(_this->p_total_rx_packet, _this->p_rx_packet, _this->rx_packet_size);
          _this->new_packet_received = true;
          /* Reset the internal variables for the next packet */
          _this->rx_frame_counter = 0;
          _this->rx_bytes_received = 0;
          _this->rx_packet_size = 0;
          /* Notify the application that new packet has been received */
          PACKET_RECEIVED_CALLBACK(_this);
          break;
        }
        else
        {
          /* ToDo: Check total packet size and allocate buffer */
          // Check total packet size and allocate buffer
          uint32_t total_size = (header[0] >> 9) & 0x7FFFFF; /* Extract total size from header */
          _this->rx_packet_size = total_size;
          memcpy(_this->p_total_rx_packet, &rx_buffer[SSTL_HEADER_SIZE], size - SSTL_HEADER_SIZE);
          _this->rx_frame_counter = 1;
          _this->rx_bytes_received = size - SSTL_HEADER_SIZE;
          res = SSTL_PARTIAL_PACKET;
          break;
        }
      }
      else
      {
        /* ToDo: Accumulate the frame in the previously allocated buffer */
        // Accumulate the frame in the previously allocated buffer
        uint32_t bytes_to_copy = size - SSTL_HEADER_SIZE;
        memcpy(_this->p_total_rx_packet + _this->rx_bytes_received, &rx_buffer[SSTL_HEADER_SIZE], bytes_to_copy);
        _this->rx_bytes_received += bytes_to_copy;
        _this->rx_frame_counter++;
        if (header[0] & SSTL_HEADER_FINAL_FRAME)
        {
          res = SSTL_NEW_PACKET_AVAILABLE;
          _this->new_packet_received = true;
          /* Reset the internal variables for the next packet */
          _this->rx_frame_counter = 0;
          _this->rx_bytes_received = 0;
          _this->rx_packet_size = 0;
          /* Notify the application that new packet has been received */
          PACKET_RECEIVED_CALLBACK(_this);
        }
        else
        {
          res = SSTL_PARTIAL_PACKET;
        }
        break;
      }
    }

    if (timeout == SSTL_NON_BLOCKING)
    {
      /* Return now because the function was called in non-blocking mode */
      return SSTL_OK;
    }

    if (SSTL_isRxComplete(_this, timeout, start_time))
    {
      return SSTL_TIMEOUT;
    }
  }
  return res;
}

#if SSTL_USE_EXTERNAL_ASYNC_BUFFERS == 0
bool SSTL_GetAsyncBuffer(SimpleSerialTL_t *_this, uint8_t **p_buffer, uint32_t *size)
{
  /* Get async buffer from ASPEP layer */
  bool res = _this->p_aspeptl->fGetBuffer(_this->p_aspeptl, (void **)p_buffer, ASPEP_ASYNC);
  /* Return the correct position for the payload by adding the SSTL header size */
  *p_buffer += SSTL_HEADER_SIZE;
  /* Return the size of the buffer available for user payload */
  *size = SSTL_MAX_ASYNC_PAYLOAD;  /* ToDo: transform into a variable if max size is decided during handshake */
  return res;
}
#endif /* SSTL_USE_EXTERNAL_ASYNC_BUFFERS */

uint32_t SSTL_TxAsync(SimpleSerialTL_t *_this, uint8_t channel, SSTL_AsyncPayloadType_t payload_type,
                      uint8_t *p_payload, uint32_t size, uint32_t timeout)
{
  assert_param(_this != NULL);
  assert_param(channel < 128);
  assert_param(p_payload != NULL);
  assert_param(size > 0U);
  uint32_t res = SSTL_OK;
  uint32_t start_time = HAL_GetTick();
  uint32_t current_timeout = timeout;

  if (size <= SSTL_MAX_ASYNC_PAYLOAD)
  {
    ASPEPTL_Handle_t *p_aspeptl = _this->p_aspeptl;

    /* Check if the previous transmission was completed.
     * In case there is an ongoing transmission and a timeout was specified, it waits here for it to be completed. */
    if (SSTL_isTxComplete(_this, timeout) == false)
    {
      return SSTL_BUSY;
    }

    /* Copy the SSTL header before the payload */
    uint8_t *p_sstl_frame = p_payload - SSTL_HEADER_SIZE;
    uint32_t *p_sstl_header = (uint32_t *)p_sstl_frame;

    if (payload_type == SSTL_ASYNC_PAYLOAD_TYPE_HSD)
    {
      /* HSD payload */
      /* SSTL header: Version + Final Frame + Channel */
      *p_sstl_header = (SSTL_HEADER_VERSION | SSTL_HEADER_FINAL_FRAME | SSTL_HEADER_CHANNEL(channel));
    }
    else if (payload_type == SSTL_ASYNC_PAYLOAD_TYPE_CUSTOM)
    {
      /* Custom payload */
      /* SSTL header: Version + Custom Payload Flag + Final Frame + Channel */
      *p_sstl_header = (SSTL_HEADER_VERSION | SSTL_HEADER_CR_CUSTOM | SSTL_HEADER_FINAL_FRAME | SSTL_HEADER_CHANNEL(channel));
    }
    else /* Textual payload */
    {
      /* Textual payload */
      /* SSTL header: Version + Textual Payload Flag + Final Frame + Channel */
      *p_sstl_header = (SSTL_HEADER_VERSION | SSTL_HEADER_CR_TEXTUAL | SSTL_HEADER_FINAL_FRAME | SSTL_HEADER_CHANNEL(channel));
    }

    _this->status = SSTL_STATUS_ASYNC_TX_LAST;

    /* Call the ASPEP function to send the ASYNC packet */
    if (p_aspeptl->fSendPacket(p_aspeptl, p_sstl_frame, size + SSTL_HEADER_SIZE, ASPEP_ASYNC) != ASPEP_OK)
    {
      return SSTL_LL_PROTOCOL_ERROR;
    }

    /* Notify the application that tx is in progress */
    FRAME_TRANSMITTING_CALLBACK(_this);

    if (timeout == SSTL_NON_BLOCKING)
    {
      /* TX in progress. Return now because the function was called in non-blocking mode */
      return SSTL_OK;
    }

    if (timeout != SSTL_BLOCKING)
    {
      /* check if we already spent more than the timeout */
      if (timeout > (HAL_GetTick() - start_time))
      {
        /* Take into account the time already spent in this API */
        current_timeout = timeout - (HAL_GetTick() - start_time);
      }
      else
      {
        /* Elapsed time is greater than the timeout */
        return SSTL_TIMEOUT;
      }
    }

    if (SSTL_isTxComplete(_this, current_timeout))
    {
      return SSTL_OK;
    }
    /* tx timed out */
    return SSTL_TIMEOUT;
  }
  else
  {
    /* Wrong size provided by the application */
    res = SSTL_GENERIC_ERROR;
  }
  return res;
}

bool SSTL_isTxComplete(SimpleSerialTL_t *_this, uint32_t timeout)
{
  if (timeout == SSTL_BLOCKING)
  {
    /* Blocking call, never return */
    while (_this->status != SSTL_STATUS_IDLE);
    return true;
  }
  else
  {
    /* Non blocking call with timeout, return false if timedout */
    uint32_t current_time, end_time;
    for (current_time = HAL_GetTick(), end_time = current_time + timeout; _this->status != SSTL_STATUS_IDLE;)
    {
      if (current_time > end_time)
      {
        /* Time out */
        return false;
      }
      current_time = HAL_GetTick();
    }
    /* TX ended */
    return true;
  }
}

static void Aspep_tx_complete(void)
{
  /* Go back to the IDLE state if the transmission is complete */
  if ((sObj.status == SSTL_STATUS_SYNC_TX_LAST) || (sObj.status == SSTL_STATUS_SYNC_TX)
      || (sObj.status == SSTL_STATUS_ASYNC_TX_LAST))
  {
    FRAME_TX_COMPLETE_CALLBACK(&sObj);
    sObj.status = SSTL_STATUS_IDLE;
  }
}

static void Aspep_new_packet(void)
{
  /* Application callback */
  FRAME_RX_COMPLETE_CALLBACK(&sObj);
}


/* Wrapper for ASPEP DMA IRQ Handler */
void SSTL_DMA_isr(void)
{
  UASPEP_DMA_Rx_Handler(&aspepOverUartA);
}

/* Wrapper for ASPEP UART IRQ Handler when DMA is used*/
void SSTL_USART_isr(void)
{
  UASPEP_Usart_Handler(&aspepOverUartA);
}


/* Wrapper for ASPEP UART RX IRQ Handler when DMA is not used */
void SSTL_UART_rx_isr(void)
{
  UASPEP_UART_Rx_Handler(&aspepOverUartA);
}

/* Wrapper for ASPEP UART TX IRQ Handler when DMA is not used */
void SSTL_UART_tx_isr(void)
{
  UASPEP_UART_Tx_Handler(&aspepOverUartA);
}


/*
 * SSTL callbacks implementation
 **/
#if (SSTL_CALLBACKS_ENABLE == 1)

void SSTL_RegisterFrameTransmitting_cb(SimpleSerialTL_t *_this, SSTL_FrameTransmitting_cb_t cb)
{
  assert_param(_this != NULL);
  assert_param(cb != NULL);

  if ((_this != NULL) && (cb != NULL))
  {
    _this->FrameTransmitting_cb = cb;
  }
}

void SSTL_RegisterTxComplete_cb(SimpleSerialTL_t *_this, SSTL_FrameTxComplete_cb_t cb)
{
  assert_param(_this != NULL);
  assert_param(cb != NULL);

  if ((_this != NULL) && (cb != NULL))
  {
    _this->FrameTxComplete_cb = cb;
  }
}

void SSTL_RegisterFrameReceived_cb(SimpleSerialTL_t *_this, SSTL_FrameReceived_cb_t cb)
{
  assert_param(_this != NULL);
  assert_param(cb != NULL);

  if ((_this != NULL) && (cb != NULL))
  {
    _this->FrameReceived_cb = cb;
  }
}

void SSTL_RegisterPacketReceived_cb(SimpleSerialTL_t *_this, SSTL_PacketReceived_cb_t cb)
{
  assert_param(_this != NULL);
  assert_param(cb != NULL);

  if ((_this != NULL) && (cb != NULL))
  {
    _this->PacketReceived_cb = cb;
  }
}
#endif /* SSTL_CALLBACKS_ENABLE */
