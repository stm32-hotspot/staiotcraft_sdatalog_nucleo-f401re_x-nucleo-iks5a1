/**
  ******************************************************************************
  * @file           : vdatalog.h
  * @brief          : Application code for Vanilla Datalog
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VDATALOG_H
#define __VDATALOG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "vdatalog_conf.h"
#include "stdbool.h"
#include "fsm.h"

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
//#define     SENSOR_0       CUSTOM_ISM6HG256X_0                 /* Sensor used */
/* When set to 1, real sensor data are overwritten with a rolling counter
 * (for debugging purposes)
 **/
#ifndef VD_DUMMY_DATA
#define VD_DUMMY_DATA        0
#endif

/*
 * Set to 1 to enable support for combo sensor
 **/
#ifndef VD_COMBO_SENSOR
#define VD_COMBO_SENSOR       0
#endif

/*
 * Select how many buffers you want to discard at the beginning of the acquisition
 **/
#ifndef VD_BUFFER_TO_DROP
#define VD_BUFFER_TO_DROP     1U
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

void vdatalog_init(void *sensor_hw_handle);
void vdatalog_main(void);
void VDatalog_main_combo(void *sensor_hw_handle);
void ProtocolScheduler(void);
void vdatalog_boot_resume_if_needed(void);
void jump_to_bootloader( void );

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __VDATALOG_H */
