/**
  ******************************************************************************
  * @file    aspep_config.c
  * @author  STMicroelectronics
  * @brief
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

#include "usart_aspep_driver.h"

/* Buffers for sync packets */
static uint8_t ASPEP_SyncTxBuff[ASPEP_TX_SYNC_BUFFER_SIZE] __attribute__((aligned(4)));
static uint8_t ASPEP_SyncRxBuff[ASPEP_RX_SYNC_BUFFER_SIZE] __attribute__((aligned(4)));

#if ASPEP_USE_EXTERNAL_ASYNC_BUFFERS == 0
/* Buffers for async packets */
static uint8_t ASPEP_AsyncBuff_A[ASPEP_TX_ASYNC_BUFFER_SIZE] __attribute__((aligned(4)));
static uint8_t ASPEP_AsyncBuff_B[ASPEP_TX_ASYNC_BUFFER_SIZE] __attribute__((aligned(4)));
#endif

static UASPEP_Handle_t UASPEP_A =
{
#if ASPEP_USE_DMA == 1
  .USARTx = ASPEP_USART_A,
  .rxDMA = ASPEP_DMA_RX_A,
  .txDMA = ASPEP_DMA_TX_A,
  .rxChannel = ASPEP_DMA_RX_CH_A,
  .txChannel = ASPEP_DMA_TX_CH_A,
#else
  .huartn = ASPEP_USART_HANDLE,
  .rxDMA = 0,
  .txDMA = 0,
  .rxChannel = 0,
  .txChannel = 0,
#endif
};

ASPEP_Handle_t aspepOverUartA =
{
  ._Super =
  {
    .fGetBuffer = &ASPEP_getBuffer,
    .fSendPacket = &ASPEP_sendPacket,
    .fRXPacketProcess = &ASPEP_RXframeProcess,
  },
  .HWIp = &UASPEP_A,
  .Capabilities = {
    .DATA_CRC = 0U,
    .RX_maxSize = (ASPEP_RX_SYNC_PAYLOAD_MAX >> 5U) - 1U,
    .TXS_maxSize = (ASPEP_TX_SYNC_PAYLOAD_MAX >> 5U) - 1U,
    .TXA_maxSize =  ASPEP_TX_ASYNC_PAYLOAD_MAX_A >> 6U,
                                                 .version = 0x0U,
  },
  .syncBuffer = {
    .buffer = ASPEP_SyncTxBuff,
  },
#if ASPEP_USE_EXTERNAL_ASYNC_BUFFERS == 0
  .asyncBufferA = {
    .buffer = ASPEP_AsyncBuff_A,
  },
  .asyncBufferB = {
    .buffer = ASPEP_AsyncBuff_B,
  },
#endif
  .rxBuffer = ASPEP_SyncRxBuff,
  .fASPEP_HWInit = &UASPEP_INIT,
#if ASPEP_USE_DMA == 1
  .fASPEP_HWSync = &UASPEP_IDLE_ENABLE,
#endif
  .fASPEP_receive = &UASPEP_RECEIVE_BUFFER,
  .fASPEP_send = &UASPEP_SEND_PACKET,
  .liid = 0,
};

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
