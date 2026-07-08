
/**
  ******************************************************************************
  * @file    aspep_slave_conf.h
  * @author  STMicroelectronics
  * @brief   User space configuration file for ASPEP slave protocol
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef ASPEP_SLAVE_CONF_H
#define ASPEP_SLAVE_CONF_H

#include "aspep_slave_hw_conf.h"
extern  UART_HandleTypeDef huart2;

#define ASPEP_USART_A                 USART2
#define ASPEP_DMA_RX_A                GPDMA1
#define ASPEP_DMA_TX_A                GPDMA1
#define ASPEP_DMA_RX_CH_A             LL_DMA_CHANNEL_0
#define ASPEP_DMA_TX_CH_A             LL_DMA_CHANNEL_1

#define ASPEP_USART_HANDLE	          &huart2
#define ASPEP_USE_DMA		              0

/* TXS_MAX -> Ranges from 32 to 4096 Bytes */
#define ASPEP_TX_SYNC_PAYLOAD_MAX     4096U

/* RXS_MAX -> Ranges from 32 to 2048 Bytes */
#define ASPEP_RX_SYNC_PAYLOAD_MAX     2048U

/* TXA_MAX -> Ranges from 0 to 8192 Bytes */
#define ASPEP_TX_ASYNC_PAYLOAD_MAX_A  8128U

/* Define payload type */
#define ASPEP_PAYLOAD_TYPE            ASPEP_PAYLOAD_TYPE_SSTL

#define ASPEP_CALLBACKS_ENABLE             1

#define ASPEP_USE_EXTERNAL_ASYNC_BUFFERS   1

#endif /* ASPEP_SLAVE_CONF_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
