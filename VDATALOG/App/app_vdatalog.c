/**
 ******************************************************************************
 * File Name          : app_vdatalog.c
 * Description        : This file provides code for the configuration
 *                      of the STMicroelectronics.FP-SNS-STAIOTCFT.1.1.0 instances.
 ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
 ******************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "app_vdatalog.h"
#include "vdatalog.h"

void MX_VDATALOG_Init(void)
{
    vdatalog_init((void*)&hi2c1);

}

void MX_VDATALOG_Process(void)
{
	vdatalog_main();
}

#ifdef __cplusplus
}
#endif

/* ----------------    End VDL app    ----------------------------------------*/

