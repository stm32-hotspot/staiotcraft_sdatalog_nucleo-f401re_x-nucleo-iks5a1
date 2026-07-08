/**
  ******************************************************************************
  * @file    App_Model.c
  * @author  SRA
  * @brief   App Application Model and PnPL Components APIs
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

#include "App_model.h"
#include <string.h>
#include <stdio.h>
/* USER includes -------------------------------------------------------------*/

/* USER defines --------------------------------------------------------------*/

/* USER private function prototypes ------------------------------------------*/


AppModel_t app_model;
static uint16_t sensors_cnt;

AppModel_t *getAppModel(void)
{
  return &app_model;
}

uint8_t getNextSensorId(uint16_t *id)
{
  if (sensors_cnt < SENSOR_NUMBER)
  {
    *id = sensors_cnt;
    sensors_cnt++;
    return PNPL_NO_ERROR_CODE;
  }
  return PNPL_BASE_ERROR_CODE;
}

uint8_t addSensorToAppModel(uint16_t id, SensorModel_t *model)
{
  if (id < SENSOR_NUMBER)
  {
    app_model.s_models[id] = model;
    return PNPL_NO_ERROR_CODE;
  }
  return PNPL_BASE_ERROR_CODE;
}

