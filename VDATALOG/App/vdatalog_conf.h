
/**
  ******************************************************************************
  * @file    vdatalog_conf.h
  * @author  STMicroelectronics
  * @brief   Configuration file for Vanilla Datalog
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

#ifndef VDATALOG_CONF_H_
#define VDATALOG_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_ll_tim.h"
#include "ism6hg256x_reg.h"
#include "stm32f4xx_nucleo_bus.h"

/* Timestamp */
#define VD_TIMESTAMP_TIM          (htim5)
#define VD_TIMESTAMP_TIM_INIT     MX_TIM5_Init

#define MLC_POLLING_TIM           (htim3)

#define LED10_SLOW_TIMER 			    (1000)  /* Blue led blinking slow indicating fw is running */
#define LED10_FAST_TIMER 			    (200)   /* Blue led blinking fast indicating data logging is on */
#define LED1_TIMER    				    (2000)  /* Green led flashing once indicating GUI is connected */

#define VD_CS_Pin                  GPIO_PIN_5
#define VD_CS_GPIO_Port            GPIOI
#define VD_SENSOR_INT1_PIN         GPIO_PIN_4
#define VD_SENSOR_INT2_PIN         GPIO_PIN_5

#define SENSOR_0                   CUSTOM_ISM6HG256X_0                 /* Sensor used */

#define SENSOR_1                   CUSTOM_IIS2DULPX_0

#define VD_DUMMY_DATA              0

#define VD_COMBO_SENSOR            0

#define AXIS                       3

#define FIFO_WATERMARK             200

#define BYTE_COUNTER_SIZE          4U

#define BYTES_PER_SAMPLE            (AXIS * 2U)

#define BYTES_PER_SAMPLE_ACC        (6U)

#define BYTES_PER_SAMPLE_GYRO       (6U)

#define BYTES_PER_SAMPLE_MLC        (2U)

#define SAMPLES_PER_TIME_STAMP_ACC  (1U)

#define SAMPLES_PER_TIME_STAMP_GYRO (1U)

#define SAMPLES_PER_TIME_STAMP_MLC  (1U)

#define SSTL_ASYNC_CH_ACCELERO	    (0)

#define SSTL_ASYNC_CH_GYRO	        (1)

#define SSTL_ASYNC_CH_MLC	          (2)

#define APP_RX_DATA_SIZE  2048

#define APP_TX_DATA_SIZE  2048

#ifdef __cplusplus
}
#endif

#endif /* VDATALOG_CONF_H_ */
