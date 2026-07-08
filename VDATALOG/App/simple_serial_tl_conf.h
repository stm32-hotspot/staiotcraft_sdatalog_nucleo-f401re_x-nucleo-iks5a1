
/**
  ******************************************************************************
  * @file    simple_serial_tl_conf.h
  * @author  STMicroelectronics
  * @brief   User configuration for Simple Serial Transpor Layer
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef SIMPLESERIALTL_CONF_H_
#define SIMPLESERIALTL_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* HAL include dependent on the STM32 family */
#include "stm32f4xx_hal.h"
#include "stdlib.h"

/* #define SSTL_MALLOC   malloc */
/* #define SSTL_FREE     free */

#define SSTL_CALLBACKS_ENABLE 1

#define SSTL_USE_EXTERNAL_ASYNC_BUFFERS    1

#ifdef __cplusplus
}
#endif

#endif /* SIMPLESERIALTL_CONF_H_ */
