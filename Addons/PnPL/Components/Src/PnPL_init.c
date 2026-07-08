/**
  ******************************************************************************
  * @file    PnPL_init.c
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

#include "PnPL_init.h"

static IPnPLComponent_t *pIsm6hg256x_Acc_PnPLObj = NULL;
static IPnPLComponent_t *pIsm6hg256x_Gyro_PnPLObj = NULL;
static IPnPLComponent_t *pIsm6hg256x_Mlc_PnPLObj = NULL;
static IPnPLComponent_t *pLog_Controller_PnPLObj = NULL;
static IPnPLComponent_t *pAcquisition_Information_PnPLObj = NULL;
static IPnPLComponent_t *pDevice_Information_PnPLObj = NULL;

uint8_t PnPL_Components_Alloc(void)
{
  /* PnPL Components Allocation */
  pIsm6hg256x_Acc_PnPLObj = Ism6hg256x_Acc_PnPLAlloc();
  pIsm6hg256x_Gyro_PnPLObj = Ism6hg256x_Gyro_PnPLAlloc();
  pIsm6hg256x_Mlc_PnPLObj = Ism6hg256x_Mlc_PnPLAlloc();
  pLog_Controller_PnPLObj = Log_Controller_PnPLAlloc();
  pAcquisition_Information_PnPLObj = Acquisition_Information_PnPLAlloc();
  pDevice_Information_PnPLObj = Device_Information_PnPLAlloc();
  return PNPL_NO_ERROR_CODE;
}

uint8_t PnPL_Components_Init(void)
{
  /* Init&Add PnPL Components */
  Ism6hg256x_Acc_PnPLInit(pIsm6hg256x_Acc_PnPLObj);
  Ism6hg256x_Gyro_PnPLInit(pIsm6hg256x_Gyro_PnPLObj);
  Ism6hg256x_Mlc_PnPLInit(pIsm6hg256x_Mlc_PnPLObj);
  Log_Controller_PnPLInit(pLog_Controller_PnPLObj);
  Acquisition_Information_PnPLInit(pAcquisition_Information_PnPLObj);
  Device_Information_PnPLInit(pDevice_Information_PnPLObj);
  return PNPL_NO_ERROR_CODE;
}
