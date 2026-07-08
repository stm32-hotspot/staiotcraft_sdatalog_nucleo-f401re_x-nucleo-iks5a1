/**
  ******************************************************************************
  * @file    App_model_Ism6hg256x_Mlc.h
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

/**
  ******************************************************************************
  * Component APIs *************************************************************
  * - Component init function
  *    <comp_name>_comp_init(void)
  * - Component get_key function
  *    <comp_name>_get_key(void)
  * - Component GET/SET Properties APIs ****************************************
  *  - GET Functions
  *    uint8_t <comp_name>_get_<prop_name>(prop_type *value)
  *      if prop_type == char --> (char **value)
  *  - SET Functions
  *    uint8_t <comp_name>_set_<prop_name>(prop_type value)
  *      if prop_type == char --> (char *value)
  *  - Component COMMAND Reaction Functions
  *      uint8_t <comp_name>_<command_name>(
  *                     field1_type field1_name, field2_type field2_name, ...,
  *                     fieldN_type fieldN_name);
  *  - Component TELEMETRY Send Functions
  *      uint8_t <comp_name>_create_telemetry(tel1_type tel1_name,
  *                     tel2_type tel2_name, ..., telN_type telN_name,
  *                     char **telemetry, uint32_t *size)
  ******************************************************************************
  */

#ifndef APP_MODEL_ISM6HG256X_MLC_H_
#define APP_MODEL_ISM6HG256X_MLC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* USER includes -------------------------------------------------------------*/

/* ISM6HG256X_MLC PnPL Component ---------------------------------------------- */
uint8_t ism6hg256x_mlc_comp_init(void);
char *ism6hg256x_mlc_get_key(void);
uint8_t ism6hg256x_mlc_get_enable(bool *value);
uint8_t ism6hg256x_mlc_get_model_filename(char **value);
uint8_t ism6hg256x_mlc_set_enable(bool value);
uint8_t ism6hg256x_mlc_load_model(const char *filename, int32_t size, const char *content);
uint8_t ism6hg256x_mlc_create_telemetry(int32_t label_id, char **telemetry, uint32_t *size);


#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_ISM6HG256X_MLC_H_ */
