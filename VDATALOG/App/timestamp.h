/**
  ******************************************************************************
  * @file    timestamp.h
  *
  * @brief
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  ******************************************************************************
  */
#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tim.h"
#include <stdbool.h>

/* Timestamp size in Bytes */
#define TIMESTAMP_SIZE        (sizeof(double))

/* Public API declaration */
/**************************/
void TS_TIM_VD_Init(void);
extern void TS_TIM_VD_Start(void);
extern void TS_TIM_VD_Stop(void);
extern bool TS_TIM_VD_IsStarted(void);
double TS_VD_GetTimeStamp(void);
double TS_VD_GetTimeStamp_fromISR(void);

void TS_TIM_MLC_Init(void);
extern void TS_TIM_MLC_Start(void);
extern void TS_TIM_MLC_Stop(void);
extern bool TS_TIM_MLC_IsStarted(void);

#ifdef __cplusplus
}
#endif

#endif /* TIMESTAMP_H_ */
