/**
  ******************************************************************************
  * @file           : lsm6dsv16x_mlc.h
  * @brief          : Application code for lsm6dsv16x_mlc
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
// ...existing code...
#ifndef __ISM6HG256X_MLC_H
#define __ISM6HG256X_MLC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "fsm.h"
#include "custom_motion_sensors.h"
#include "custom_mems_conf.h"
#include "MEMS_integration.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

extern FSM 			    MlcFsm;
extern Event 		    MlcEvent;
extern Event 		    MlcEventInt;
extern Transition 	MlcTransitions[];
extern unsigned int sizeof_Mlc_transitions;

/* User Defined MLC specific FSM event handling functions */
void MlcDisabledStateActionEnable(Event event, void* args);

void MlcEnabledStateActionLoad(Event event, void* args);

void MlcEnabledStateActionDisable(Event event, void* args);

void MlcLoadedStateActionLoad(Event event, void* args);

void MlcLoadedStateActionDisable(Event event, void* args);

void MlcLoadedStateActionLogRun(Event event, void* args);

void MlcLoggingDataStateActionLogStop(Event event, void* args);

void MlcLoggingDataStateActionInt(Event event, void* args);

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* __ISM6HG256X_MLC_H */
typedef struct {
  const char *filename;
  int32_t size;
  const char *content;
} MlcModel_t;
