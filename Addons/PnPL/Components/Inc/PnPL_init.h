/**
  ******************************************************************************
  * @file    PnPL_init.h
  * @author  SRA
  * @brief   PnPL Components initialization functions
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

/**
  ******************************************************************************
  * This file has been auto generated from the following Device Template Model:
  * dtmi:appconfig:nucleo_stm32f401re:STAIOTCRAFT_SDATALOG;1
  *
  * Created by: DTDL2PnPL_cGen version 3.1.0-alpha.2
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#ifndef PNPL_INIT_H_
#define PNPL_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PnPLCompManager.h"
#include "Ism6hg256x_Acc_PnPL.h"
#include "Ism6hg256x_Gyro_PnPL.h"
#include "Ism6hg256x_Mlc_PnPL.h"
#include "Log_Controller_PnPL.h"
#include "Acquisition_Information_PnPL.h"
#include "Device_Information_PnPL.h"


uint8_t PnPL_Components_Alloc(void);
uint8_t PnPL_Components_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* PNPL_INIT_H_ */
