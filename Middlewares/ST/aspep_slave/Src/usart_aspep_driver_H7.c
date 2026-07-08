/**
  ******************************************************************************
  * @file    usart_aspep_driver_H7.c
  * @author  STMicroelectronics
  * @brief   UART ASPEP driver implementation for STM32H7
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "usart_aspep_driver.h"

#define __DMA_PTR_REG_OFFSET(__REG__, __REG_OFFFSET__)                         \
  ((__IO uint32_t *)((uint32_t) ((uint32_t)(&(__REG__)) + ((__REG_OFFFSET__) << 2U))))

#define __LL_DMA_IT_TC_BIT(__STREAM_NB__) \
  (((__STREAM_NB__&0x3) == 0U) ? 5 : \
   ((__STREAM_NB__&0x3) == 1U) ? 11 :\
   ((__STREAM_NB__&0x3) == 2U) ? 21 : 27)

#define __LL_DMA_IT_HT_BIT(__STREAM_NB__) \
  (((__STREAM_NB__&0x3) == 0U) ? 4 : \
   ((__STREAM_NB__&0x3) == 1U) ? 10 :\
   ((__STREAM_NB__&0x3) == 2U) ? 20 : 26)

#define __LL_DMA_IT_TE_BIT(__STREAM_NB__) \
  (((__STREAM_NB__&0x3) == 0U) ? 3 : \
   ((__STREAM_NB__&0x3) == 1U) ? 9 :\
   ((__STREAM_NB__&0x3) == 2U) ? 19 : 25)


void UASPEP_DMACONFIG_TX(UASPEP_Handle_t *pHandle);
void UASPEP_DMACONFIG_RX(UASPEP_Handle_t *pHandle);

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup MCP
  * @{
  */

/**
  * @brief  Initialization of the Hardware used for data transmission and reception.
  *
  * @param  pHandle Handler of the current instance of the UASPEP component
  */
void UASPEP_INIT(void *pHWHandle)
{
  UASPEP_Handle_t *pHandle = (UASPEP_Handle_t *)pHWHandle; //cstat !MISRAC2012-Rule-11.5
  UASPEP_DMACONFIG_TX(pHandle);
  UASPEP_DMACONFIG_RX(pHandle);
}

/**
  * @brief  Configures the Hardware used for data transmission to controller.
  *
  * @param  pHandle Handler of the current instance of the UASPEP component
  */
void UASPEP_DMACONFIG_TX(UASPEP_Handle_t *pHandle)
{
#ifdef NULL_PTR_CHECK_USA_ASP_DRV
  if (NULL == pHandle)
  {
    /* Nothing to do */
  }
  else
  {
#endif
    /* Enable DMA UART */
    LL_USART_ClearFlag_TC(pHandle->USARTx);
    LL_USART_EnableIT_TC(pHandle->USARTx);

    /* Enable DMA UART to start the TX request */
    LL_USART_EnableDMAReq_TX(pHandle->USARTx);

    /* Write the USART_TDR register address in the DMA control register to configure it as
     * the destination of the transfer */
    //cstat !MISRAC2012-Rule-11.4
    LL_DMA_SetPeriphAddress(pHandle->txDMA, pHandle->txChannel, (uint32_t)&pHandle->USARTx->TDR);

    /* Clear UART ISR */
    LL_USART_ClearFlag_TC(pHandle->USARTx);

    /* DMA end of transfer on UART TX channel completion is not activated */
    /* We prefer to activate UART TC itself to avoid to trig IT while queued data are still to be transmitted */
#ifdef NULL_PTR_CHECK_USA_ASP_DRV
  }
#endif
}

/**
  * @brief  Configures the Hardware used for data reception from controller.
  *
  * @param  pHandle Handler of the current instance of the UASPEP component
  */
void UASPEP_DMACONFIG_RX(UASPEP_Handle_t *pHandle)
{
#ifdef NULL_PTR_CHECK_USA_ASP_DRV
  if (NULL == pHandle)
  {
    /* Nothing to do */
  }
  else
  {
#endif
    /* DMA interrupt not used for all families */
    /* Enable DMA end of transfer on UART RX channel completion */
    LL_DMA_EnableIT_TC(pHandle->rxDMA, pHandle->rxChannel);
    /* Enable Error interrupt (EIE) to unmask Overrun interrupt */
    LL_USART_EnableIT_ERROR(pHandle->USARTx);

    /* Write the USART_RDR register address in the DMA control register to configure it as
     * the source of the transfer */
    //cstat !MISRAC2012-Rule-11.4
    LL_DMA_SetPeriphAddress(pHandle->rxDMA, pHandle->rxChannel, (uint32_t)&pHandle->USARTx->RDR);

    /* Clear UART ISR */
    LL_USART_ClearFlag_TC(pHandle->USARTx);

    LL_USART_EnableDMAReq_RX(pHandle->USARTx);
#ifdef NULL_PTR_CHECK_USA_ASP_DRV
  }
#endif
}

__STATIC_INLINE void LL_DMA_ClearFlag_TC(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if (NULL == DMAx)
  {
    /* Nothing to do */
  }
  else
  {
    /* Clear TC bits with bits position depending on parameter "Stream" */
    register __IO uint32_t *preg = __DMA_PTR_REG_OFFSET(DMAx->LIFCR, (Stream <= 3) ? 0 : 1);

    WRITE_REG(*preg, 1 << __LL_DMA_IT_TC_BIT(Stream));
  }
}

__STATIC_INLINE void LL_DMA_ClearFlag_TE(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if (NULL == DMAx)
  {
    /* Nothing to do */
  }
  else
  {
    /* Clear TE bits with bits position depending on parameter "Stream" */
    register __IO uint32_t *preg = __DMA_PTR_REG_OFFSET(DMAx->LIFCR, (Stream <= 3) ? 0 : 1);

    WRITE_REG(*preg, 1 << __LL_DMA_IT_TE_BIT(Stream));
  }
}

//cstat !MISRAC2012-Rule-8.13
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_TC(DMA_TypeDef *DMAx, uint32_t Stream)
{
  uint32_t retVal;
  if (NULL == DMAx)
  {
    retVal = 0;
  }
  else
  {
    register __IO uint32_t *preg = __DMA_PTR_REG_OFFSET(DMAx->LISR, (Stream <= 3) ? 0 : 1);
    retVal = ((READ_BIT(*preg, 1 << __LL_DMA_IT_TC_BIT(Stream)) == (1 << __LL_DMA_IT_TC_BIT(Stream))) ? 1UL : 0UL);
  }
  return (retVal);
}
//cstat !MISRAC2012-Rule-8.13
__STATIC_INLINE void LL_DMA_ClearFlag_HT(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if (NULL == DMAx)
  {
    /* Nothing to do */
  }
  else
  {
    /* Clear TC bits with bits position depending on parameter "Stream" */
    register __IO uint32_t *preg = __DMA_PTR_REG_OFFSET(DMAx->LIFCR, (Stream <= 3) ? 0 : 1);

    WRITE_REG(*preg, 1 << __LL_DMA_IT_HT_BIT(Stream));
  }
}
//cstat !MISRAC2012-Rule-8.13
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_HT(DMA_TypeDef *DMAx, uint32_t Stream)
{
  uint32_t retVal;
  if (NULL == DMAx)
  {
    retVal = 0;
  }
  else
  {
    register __IO uint32_t *preg = __DMA_PTR_REG_OFFSET(DMAx->LISR, (Stream <= 3) ? 0 : 1);
    retVal = ((READ_BIT(*preg, 1 << __LL_DMA_IT_HT_BIT(Stream)) == (1 << __LL_DMA_IT_HT_BIT(Stream))) ? 1UL : 0UL);
  }
  return (retVal);
}


/*
__STATIC_INLINE void LL_DMA_ClearFlag_TC(DMA_TypeDef *DMAx, uint32_t Stream){

  switch (Stream)
  {
    case LL_DMA_STREAM_0:
      LL_DMA_ClearFlag_TC0(DMAx);
      break;
    case LL_DMA_STREAM_1:
      LL_DMA_ClearFlag_TC1(DMAx);
      break;
    case LL_DMA_STREAM_2:
      LL_DMA_ClearFlag_TC2(DMAx);
      break;
    case LL_DMA_STREAM_3:
      LL_DMA_ClearFlag_TC3(DMAx);
      break;
    case LL_DMA_STREAM_4:
      LL_DMA_ClearFlag_TC4(DMAx);
      break;
    case LL_DMA_STREAM_5:
      LL_DMA_ClearFlag_TC5(DMAx);
      break;
    case LL_DMA_STREAM_6:
      LL_DMA_ClearFlag_TC6(DMAx);
      break;
    case LL_DMA_STREAM_7:
      LL_DMA_ClearFlag_TC7(DMAx);
      break;
    default:
      LL_DMA_ClearFlag_TC0(DMAx);
      LL_DMA_ClearFlag_TC1(DMAx);
      LL_DMA_ClearFlag_TC2(DMAx);
      LL_DMA_ClearFlag_TC3(DMAx);
      LL_DMA_ClearFlag_TC4(DMAx);
      LL_DMA_ClearFlag_TC5(DMAx);
      LL_DMA_ClearFlag_TC6(DMAx);
      LL_DMA_ClearFlag_TC7(DMAx);
      break;
    }
}
*/

/**
  * @brief  Configures data transmission.
  *
  * @param  pHWHandle Hardware components chosen for communication
  * @param  data Data to be transmitted to controller
  * @param  length Length of the data to be transmitted
  */
bool UASPEP_SEND_PACKET(void *pHWHandle, void *data, uint16_t length)
{
  UASPEP_Handle_t *pHandle = (UASPEP_Handle_t *)pHWHandle; //cstat !MISRAC2012-Rule-11.5
  bool result;
  if (0U == LL_DMA_IsEnabledStream(pHandle->txDMA, pHandle->txChannel))
  {
    //cstat !MISRAC2012-Rule-11.4 !MISRAC2012-Rule-11.6
    LL_DMA_SetMemoryAddress(pHandle->txDMA, pHandle->txChannel, (uint32_t)data);
    LL_DMA_SetDataLength(pHandle->txDMA, pHandle->txChannel, length);
    LL_DMA_ClearFlag_TC(pHandle->txDMA, pHandle->txChannel);
    LL_DMA_EnableStream(pHandle->txDMA, pHandle->txChannel);
  }
  else
  {
    result = false;
  }
  return (result);
}

/**
  * @brief  Configures data reception.
  *
  * @param  pHWHandle Hardware components chosen for communication
  * @param  buffer Buffer which will receive the communicated data
  * @param  length Length of the received data
  */
void UASPEP_RECEIVE_BUFFER(void *pHWHandle, void *buffer, uint16_t length)
{
  UASPEP_Handle_t *pHandle = (UASPEP_Handle_t *)pHWHandle; //cstat !MISRAC2012-Rule-11.5
  //cstat !MISRAC2012-Rule-11.4 !MISRAC2012-Rule-11.6
  LL_DMA_SetMemoryAddress(pHandle->rxDMA, pHandle->rxChannel, (uint32_t)buffer);
  LL_DMA_SetDataLength(pHandle->rxDMA, pHandle->rxChannel, length);

  LL_DMA_EnableStream(pHandle->rxDMA, pHandle->rxChannel);
}

/**
  * @brief  Sets IDLE state : no transmission on going.
  *
  * @param  pHandle Handler of the current instance of the UASPEP component
  */
void UASPEP_IDLE_ENABLE(void *pHWHandle)
{
  UASPEP_Handle_t *pHandle = (UASPEP_Handle_t *)pHWHandle; //cstat !MISRAC2012-Rule-11.5
  LL_USART_ClearFlag_IDLE(pHandle->USARTx);
  LL_USART_EnableIT_IDLE(pHandle->USARTx);
}


/*__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_TC(DMA_TypeDef *DMAx, uint32_t Stream){

  switch (Stream)
  {
    case LL_DMA_STREAM_0:
      return LL_DMA_IsActiveFlag_TC0(DMAx);
    case LL_DMA_STREAM_1:
      return LL_DMA_IsActiveFlag_TC1(DMAx);
    case LL_DMA_STREAM_2:
      return LL_DMA_IsActiveFlag_TC2(DMAx);
    case LL_DMA_STREAM_3:
      return LL_DMA_IsActiveFlag_TC3(DMAx);
    case LL_DMA_STREAM_4:
      return LL_DMA_IsActiveFlag_TC4(DMAx);
    case LL_DMA_STREAM_5:
      return LL_DMA_IsActiveFlag_TC5(DMAx);
    case LL_DMA_STREAM_6:
      return LL_DMA_IsActiveFlag_TC6(DMAx);
    case LL_DMA_STREAM_7:
      return LL_DMA_IsActiveFlag_TC7(DMAx);
    default:
      return 0;
      break;
    }
}*/

/**
  * @}
  */
void UASPEP_DMA_Rx_Handler(void *pHandle)
{
  if (LL_DMA_IsActiveFlag_TC(ASPEP_DMA_RX_A, ASPEP_DMA_RX_CH_A))
  {
    // Clear full buffer transfer complete ISR
    LL_DMA_ClearFlag_TC(ASPEP_DMA_RX_A, ASPEP_DMA_RX_CH_A);
    ASPEP_HWDataReceivedIT(pHandle);
  }
}


/**
  * @}
  */
void UASPEP_Usart_Handler(void *pHandle)
{
  uint32_t flags;
  uint32_t activeIdleFlag;
  uint32_t isEnabledIdleFlag;

  if (0U == LL_USART_IsActiveFlag_TC(ASPEP_USART_A))
  {
    /* Nothing to do */
  }
  else
  {
    /* Disable the DMA channel to prepare the next chunck of data*/
    LL_DMA_DisableStream(ASPEP_DMA_TX_A, ASPEP_DMA_TX_CH_A);
    LL_USART_ClearFlag_TC(ASPEP_USART_A);
    /* Data Sent by UART*/
    /* Need to free the buffer, and to check pending transfer*/
    ASPEP_HWDataTransmittedIT(pHandle);
  }
  if ((LL_USART_IsActiveFlag_ORE(ASPEP_USART_A) || LL_USART_IsActiveFlag_FE(ASPEP_USART_A)
       || LL_USART_IsActiveFlag_NE(ASPEP_USART_A))
      && LL_USART_IsEnabledIT_ERROR(ASPEP_USART_A))
  {
    /* We disable ERROR interrupt to avoid to trig one Overrun IT per additional byte recevied*/
    LL_USART_DisableIT_ERROR(ASPEP_USART_A);
    LL_USART_EnableIT_IDLE(ASPEP_USART_A);
  }
  else
  {
    /* Nothing to do */
  }

  activeIdleFlag = LL_USART_IsActiveFlag_IDLE(ASPEP_USART_A);
  isEnabledIdleFlag = LL_USART_IsEnabledIT_IDLE(ASPEP_USART_A);

  flags = activeIdleFlag & isEnabledIdleFlag;
  if (0U == flags)
  {
    /* Nothing to do */
  }
  else
  {
    /* Stopping the debugger will generate an OverRun error*/
    LL_USART_ClearFlag_FE(ASPEP_USART_A);
    LL_USART_ClearFlag_ORE(ASPEP_USART_A);
    LL_USART_ClearFlag_NE(ASPEP_USART_A);
    LL_USART_DisableIT_IDLE(ASPEP_USART_A);
    /* Once the complete unexpected data are received, we enable back the error IT*/
    LL_USART_EnableIT_ERROR(ASPEP_USART_A);
    /* To be sure we fetch the potential pending data*/
    /* We disable the DMA request, Read the dummy data, endable back the DMA request */
    LL_USART_DisableDMAReq_RX(ASPEP_USART_A);
    (void)LL_USART_ReceiveData8(ASPEP_USART_A);
    LL_USART_EnableDMAReq_RX(ASPEP_USART_A);
    LL_DMA_ClearFlag_TE(ASPEP_DMA_RX_A, ASPEP_DMA_RX_CH_A);
    /* Clear pending DMA TC to process only new received packet */
    LL_DMA_ClearFlag_TC(ASPEP_DMA_RX_A, ASPEP_DMA_RX_CH_A);
    ASPEP_HWDMAReset(pHandle);
  }
}
/************************ (C) COPYRIGHT 2024 STMicroelectronics *****END OF FILE****/

