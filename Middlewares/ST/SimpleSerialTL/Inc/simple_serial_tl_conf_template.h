/**
  ******************************************************************************
  * @file    simple_serial_tl_conf.h
  * @author  STMicroelectronics
  * @brief   Template for Simple Serial Transport Layer user configuration file
  *          Copy to user space and rename to SimpleSerialTL_conf.h
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

#ifndef SIMPLE_SERIAL_TL_CONF_H_
#define SIMPLE_SERIAL_TL_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
  * HAL include dependent on the STM32 family
  */
#include "stm32u5xx_hal.h"

/**
  * Standard library include
  */
#include "stdlib.h"

/**
  * Enable user callbacks
  */
#define SSTL_CALLBACKS_ENABLE              1

/**
  * Use either internal or external ASYNC buffer for ASYNC messages
  * Default [0] is for internal buffer, managed directly by the protocol
  */
#define SSTL_USE_EXTERNAL_ASYNC_BUFFERS    0

#ifdef __cplusplus
}
#endif

#endif /* SIMPLE_SERIAL_TL_CONF_H_ */
