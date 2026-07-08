
/**
  ******************************************************************************
  * @file    aspep_slave_hw_conf.h
  * @author  STMicroelectronics
  * @brief   User space HW configuration file for ASPEP slave protocol
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
#ifndef ASPEP_SLAVE_HW_CONF_H
#define ASPEP_SLAVE_HW_CONF_H

/*
 * Include LL header files for the correct platform
 **/

/*
 * STM32U5
 **/
#include "stm32f4xx_hal.h"
/*  Former version include with DMA support and related hw dependencies */
#ifndef NO_UART_DMA
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_dma.h"
#endif

/**
* @}
*/

#endif /* ASPEP_SLAVE_HW_CONF_H */
/******************* (C) COPYRIGHT 2022 STMicroelectronics *****END OF FILE****/
