/**
  ******************************************************************************
  * @file    usart_aspep_driver_HAL.c
  * @author  STMicroelectronics
  * @brief   UART ASPEP driver e/o DMA implementation
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

#include "usart_aspep_driver.h"

/**
  * @brief  Initialization of the DMAs used for data transmission and reception.
  *
  * @param  pHandle Handler of the current instance of the UASPEP component
  */

void UASPEP_INIT(void *pHWHandle)
{
  (void)pHWHandle;
}

void UASPEP_IDLE_ENABLE(void *pHWHandle)
{
  (void)pHWHandle;
}


bool UASPEP_SEND_PACKET(void *pHWHandle, void *data, uint16_t length)
{
  UASPEP_Handle_t *pHandle = (UASPEP_Handle_t *) pHWHandle; //cstat !MISRAC2012-Rule-11.5
  bool result = true;

#ifdef NO_UART_DMA
  if (HAL_UART_Transmit_IT(pHandle->huartn, (uint8_t *)data, length) != HAL_OK)
#else  
  if (HAL_UART_Transmit_DMA(pHandle->huartn, (uint8_t *)data, length) != HAL_OK)
#endif    
  {
    result = false;
  }
  return (result);
}


void UASPEP_RECEIVE_BUFFER(void *pHWHandle, void *buffer, uint16_t length)
{
  UASPEP_Handle_t *pHandle = (UASPEP_Handle_t *) pHWHandle; //cstat !MISRAC2012-Rule-11.5

#ifdef NO_UART_DMA
  if (HAL_UART_Receive_IT(pHandle->huartn, (uint8_t *)buffer, length) != HAL_OK)
#else  
  if (HAL_UART_Receive_DMA(pHandle->huartn, (uint8_t *)buffer, length) != HAL_OK)
#endif    
  {

  }
}

/* empty stub function to be filled in case of DMA usage */
void UASPEP_DMA_Rx_Handler(void *pHandle)
{
  (void)pHandle;
}
/* empty stub function to be filled in case of DMA usage */
void UASPEP_Usart_Handler(void *pHandle)
{
  (void)pHandle;
}

void UASPEP_UART_Rx_Handler(void *pHandle)
{
  ASPEP_HWDataReceivedIT(pHandle);
}


void UASPEP_UART_Tx_Handler(void *pHandle)
{
  ASPEP_HWDataTransmittedIT(pHandle);
}

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
