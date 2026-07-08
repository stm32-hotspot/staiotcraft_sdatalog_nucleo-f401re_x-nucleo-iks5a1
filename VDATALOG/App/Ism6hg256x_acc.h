/**
  ******************************************************************************
  * @file           : ism6hg256x_acc.h
  * @brief          : Application code for ism6hg256x_acc
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ISM6HG256X_ACC_H
#define __ISM6HG256X_ACC_H

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

extern FSM             AccFsm;
extern FSM             GyroFsm;
extern Event           AccEvent;
extern Event           AccEventInt;
extern Transition      AccTransitions[];
extern unsigned int    sizeof_Acc_transitions;

/* User Defined Acc specific FSM event handling functions */
extern void AccDisabledStateActionEnable(Event event, void* args);

extern void AccDisabledStateActionEnableOdrTrgd(Event event, void* args);

extern void AccEnabledStateActionDisable(Event event, void* args);

extern void AccEnabledOdrTrgdStateActionDisable(Event event, void* args);

extern void AccEnabledStateActionLogRun(Event event, void* args);

extern void AccEnabledOdrTrgdStateActionLogRun(Event event, void* args);

extern void AccLoggingDataStateActionLogStop(Event event, void* args);

extern void AccLoggingDataOdrTrgdStateActionLogStop(Event event, void* args);

extern void AccLoggingDataStateActionInt(Event event, void* args);

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __ISM6HG256X_ACC_H */