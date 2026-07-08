/**
  ******************************************************************************
  * @file    aspep_slave_tl.h
  * @author  STMicroelectronics
  * @brief   This file provides firmware definitions of the ASPEP protocol transport layer
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef ASPEP_SLAVE_TL_H
#define ASPEP_SLAVE_TL_H

#include <stdbool.h>
#include <stdint.h>

#define ASPEP_SYNC  ( uint8_t )0xAU
#define ASPEP_ASYNC ( uint8_t )0x9U

typedef struct ASPEPTL_Handle ASPEPTL_Handle_t; //cstat !MISRAC2012-Rule-2.4
typedef bool (* ASPEPTL_GetBuf)(ASPEPTL_Handle_t *pHandle, void **buffer, uint8_t syncAsync);
typedef uint8_t (* ASPEPTL_SendPacket)(ASPEPTL_Handle_t *pHandle, void *txBuffer, uint16_t txDataLength,
                                       uint8_t syncAsync);
typedef uint8_t *(* ASPEPTL_RXpacketProcess)(ASPEPTL_Handle_t *pHandle, uint16_t *packetLength);

typedef enum
{
  available = 0,
  writeLock = 1,
  pending = 2,
  readLock = 3,
} buff_access_t;

typedef struct
{
  uint8_t *buffer;
  uint16_t length;
  buff_access_t state;
#ifdef ASPEP_DEBUG_METRICS
  /* Debug metrics */
  uint16_t SentNumber;
  uint16_t PendingNumber;
  uint16_t RequestedNumber;
  /* End of Debug metrics */
#endif
} ASPEPTL_Buff_t;

struct ASPEPTL_Handle
{
  ASPEPTL_GetBuf fGetBuffer;
  ASPEPTL_SendPacket fSendPacket;
  ASPEPTL_RXpacketProcess fRXPacketProcess;
  uint16_t txSyncMaxPayload;
  uint16_t txAsyncMaxPayload;
  bool packetAvailable; /* Packet available for upper layer protocol*/
};

bool ASPEPTL_decodeCRCData(ASPEPTL_Handle_t *pHandle);

#endif /* ASPEP_SLAVE_TL_H */

/************************ (C) COPYRIGHT 2022 STMicroelectronics *****END OF FILE****/
