/**
  ******************************************************************************
  * @file    App_model_Ism6hg256x_Mlc.c
  * @author  SRA
  * @brief   Ism6hg256x_Mlc PnPL Components APIs
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
  * This file has been auto generated from the following DTDL Component:
  * dtmi:appconfig:nucleo_stm32f401re:STAIOTCRAFT_SDATALOG:sensors:ism6hg256x_mlc;1
  *
  * Created by: DTDL2PnPL_cGen version 3.1.0-alpha.2
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#include "App_model.h"
#include "fsm.h"
#include "Ism6hg256x_mlc_app.h"

/* USER includes -------------------------------------------------------------*/
#ifndef NO_FLASH_MEM
#include "flash_memory.h"
#endif
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

/* USER private function prototypes ------------------------------------------*/
#define MAX_MLC_FNAME_LENGTH 50
extern struct mems_conf_op* current_lsm6dsv16x_mlc_configuration;

extern bool is_MLC_configuration_in_flash(void);
extern void load_lsm6dsv16x_mlc_configuration(struct mems_conf_op* lsm6dsv16x_mlc_configuration, uint32_t ucf_number_of_lines);
extern uint8_t ism6hg256x_mlc_load_model_app(const char * fname, int32_t size, const char *content);
extern uint8_t hex_to_int(const char *hex);
uint8_t hex_char_to_int(char character);

/* USER defines --------------------------------------------------------------*/
#ifdef NO_FLASH_MEM
char mlc_fname[MAX_MLC_FNAME_LENGTH] = "none"; /* LSM6DSV16X_MLC model filename */
#endif

/* ISM6HG256X_MLC PnPL Component ---------------------------------------------*/
static SensorModel_t ism6hg256x_mlc_model;
extern AppModel_t app_model;

uint8_t ism6hg256x_mlc_comp_init(void)
{
  ism6hg256x_mlc_model.comp_name = ism6hg256x_mlc_get_key();

  uint16_t id;
  if (getNextSensorId(&id) != 0)
  {
    /* Sensor slot not available */
    return PNPL_BASE_ERROR_CODE;
  }
  addSensorToAppModel(id, &ism6hg256x_mlc_model);

  /* USER Component initialization code */
  return PNPL_NO_ERROR_CODE;
}

char *ism6hg256x_mlc_get_key(void)
{
  return "ism6hg256x_mlc";
}


uint8_t ism6hg256x_mlc_get_enable(bool *value)
{
  if (value == NULL)
  { 
    return PNPL_BASE_ERROR_CODE;
  }
  if (FsmCheckState(&MlcFsm, FSM_STATE_DISABLED)) *value = false; 
  else *value = true;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_mlc_get_model_filename(char **value)
{
  if (value == NULL)
  {
    return PNPL_BASE_ERROR_CODE;
  }
  #ifndef NO_FLASH_MEM
  if (is_MLC_configuration_in_flash())
  {
    *value = (char *)MLC_FILE_NAME_FLASH_ADDRESS;  //lsm6dsv16x mlc model filename stored in flash
    return PNPL_NO_ERROR_CODE;
  }
  #else
  *value = mlc_fname;
  #endif
  return PNPL_NO_ERROR_CODE; 
}


uint8_t ism6hg256x_mlc_set_enable(bool value)
{
  /* USER Code */
  bool log_status, en_status;
  log_controller_get_log_status(&log_status);  
  ism6hg256x_mlc_get_enable(&en_status);
  if (log_status) return PNPL_BASE_ERROR_CODE;  

  if (value)
  {  
    FsmSetEvent(&MlcFsm, FSM_EVT_ENABLE); 
  } else { 
    FsmSetEvent(&MlcFsm, FSM_EVT_DISABLE); 
  }
  return PNPL_NO_ERROR_CODE;
}


uint8_t ism6hg256x_mlc_load_model(const char *filename, int32_t size, const char *content)
{
  /* USER Code */
  bool value;
  if(filename == NULL || content == NULL || size <= 0 || size %4 !=0 || strlen(filename) >= MAX_MLC_FNAME_LENGTH) return PNPL_BASE_ERROR_CODE;
  if (size > 1400) return PNPL_BASE_ERROR_CODE; /* max model size is 1,4KB, can be changed based on the use case considering that min heap size is set to 2048 */  
    
  ism6hg256x_mlc_get_enable(&value);
  if (!value) return PNPL_BASE_ERROR_CODE;
 
  static MlcModel_t model_to_load;
  model_to_load.filename = filename;
  model_to_load.size = size;  
  model_to_load.content = content;

  FsmSetEventEx(&MlcFsm, FSM_EVT_LOAD, (void *)&model_to_load);

  return PNPL_NO_ERROR_CODE;
}


uint8_t ism6hg256x_mlc_create_telemetry(int32_t label_id, char **telemetry, uint32_t *size)
{
  PnPLTelemetry_t telemetries[1];
  strcpy(telemetries[0].telemetry_name, "label_id");
  telemetries[0].telemetry_value = (void *)& label_id;
  telemetries[0].telemetry_type = PNPL_INT;
  telemetries[0].n_sub_telemetries = 0;

  PnPLSerializeTelemetry("ism6hg256x_mlc", telemetries, 1, telemetry, size, 0);
  return PNPL_NO_ERROR_CODE;
}
