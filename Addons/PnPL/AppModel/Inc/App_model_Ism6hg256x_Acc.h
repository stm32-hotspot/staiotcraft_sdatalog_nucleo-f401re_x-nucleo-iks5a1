/**
  ******************************************************************************
  * @file    App_model_Ism6hg256x_Acc.h
  * @author  SRA
  * @brief   Ism6hg256x_Acc PnPL Components APIs
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
  * dtmi:appconfig:nucleo_stm32f401re:STAIOTCRAFT_SDATALOG:sensors:ism6hg256x_acc;1
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

#ifndef APP_MODEL_ISM6HG256X_ACC_H_
#define APP_MODEL_ISM6HG256X_ACC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "Ism6hg256x_Acc_PnPL.h"

/* USER includes -------------------------------------------------------------*/
#define DEFAULT_ACC_ODR_HZ         pnpl_ism6hg256x_acc_odr_hz240
#define DEFAULT_ACC_ODR_HZ_FLOAT   240.0f
#define DEFAULT_ACC_ODR_TRGD_HZ    pnpl_ism6hg256x_acc_odr_t_hz400
#define DEFAULT_ACC_FS_G           pnpl_ism6hg256x_acc_fs_g4
#define DEFAULT_ACC_FS_G_INT       4

/* ISM6HG256X_ACC PnPL Component ---------------------------------------------- */
uint8_t ism6hg256x_acc_comp_init(void);
char *ism6hg256x_acc_get_key(void);
uint8_t ism6hg256x_acc_get_enable(bool *value);
uint8_t ism6hg256x_acc_get_data_type(char **value);
uint8_t ism6hg256x_acc_get_dimensions(int32_t *value);
uint8_t ism6hg256x_acc_get_unit(char **value);
uint8_t ism6hg256x_acc_get_short_unit(char **value);
uint8_t ism6hg256x_acc_get_sensor_category(int32_t *value);
uint8_t ism6hg256x_acc_get_odr_triggered(bool *value);
uint8_t ism6hg256x_acc_get_odr(pnpl_ism6hg256x_acc_odr_t *enum_id);
uint8_t ism6hg256x_acc_get_odr_t(pnpl_ism6hg256x_acc_odr_t_t *enum_id);
uint8_t ism6hg256x_acc_get_fs(pnpl_ism6hg256x_acc_fs_t *enum_id);
uint8_t ism6hg256x_acc_get_initial_timestamp(float_t *value);
uint8_t ism6hg256x_acc_get_measured_odr(float_t *value);
uint8_t ism6hg256x_acc_get_sensitivity(float_t *value);
uint8_t ism6hg256x_acc_get_annotation(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__channel_specification_interface(
  pnpl_ism6hg256x_acc_output_st_stream__channel_specification_interface_t *enum_id);
uint8_t ism6hg256x_acc_get_output_st_stream__channel_specification_protocol_type(
  pnpl_ism6hg256x_acc_output_st_stream__channel_specification_protocol_type_t *enum_id);
uint8_t ism6hg256x_acc_get_output_st_stream__channel_specification_protocol_version(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__channel_specification_payload_type(
  pnpl_ism6hg256x_acc_output_st_stream__channel_specification_payload_type_t *enum_id);
uint8_t ism6hg256x_acc_get_output_st_stream__channel_specification_payload_version(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__channel_specification_stream_id(int32_t *value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_timestamp_data_type(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_timestamp_dimensions(int32_t *value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_timestamp_unit(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_timestamp_short_unit(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_timestamp_labels(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_x_data_type(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_x_dimensions(int32_t *value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_x_unit(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_x_short_unit(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_x_labels(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_y_data_type(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_y_dimensions(int32_t *value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_y_unit(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_y_short_unit(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_y_labels(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_z_data_type(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_z_dimensions(int32_t *value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_z_unit(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_z_short_unit(char **value);
uint8_t ism6hg256x_acc_get_output_st_stream__payload_specification_z_labels(char **value);
uint8_t ism6hg256x_acc_set_enable(bool value);
uint8_t ism6hg256x_acc_set_odr_triggered(bool value);
uint8_t ism6hg256x_acc_set_odr(pnpl_ism6hg256x_acc_odr_t enum_id);
uint8_t ism6hg256x_acc_set_odr_t(pnpl_ism6hg256x_acc_odr_t_t enum_id);
uint8_t ism6hg256x_acc_set_fs(pnpl_ism6hg256x_acc_fs_t enum_id);
uint8_t ism6hg256x_acc_set_annotation(const char *value);


#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_ISM6HG256X_ACC_H_ */
