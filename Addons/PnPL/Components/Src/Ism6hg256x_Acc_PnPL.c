/**
  ******************************************************************************
  * @file    Ism6hg256x_Acc_PnPL.c
  * @author  SRA
  * @brief   Ism6hg256x_Acc PnPL Component Manager
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "App_model.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"

#include "Ism6hg256x_Acc_PnPL.h"

static const IPnPLComponent_vtbl sIsm6hg256x_Acc_PnPL_CompIF_vtbl =
{
  Ism6hg256x_Acc_PnPL_vtblGetKey,
  Ism6hg256x_Acc_PnPL_vtblGetNCommands,
  Ism6hg256x_Acc_PnPL_vtblGetCommandKey,
  Ism6hg256x_Acc_PnPL_vtblGetStatus,
  Ism6hg256x_Acc_PnPL_vtblSetProperty,
  Ism6hg256x_Acc_PnPL_vtblExecuteFunction
};

/**
  *  Ism6hg256x_Acc_PnPL internal structure.
  */
struct _Ism6hg256x_Acc_PnPL
{
  /* Implements the IPnPLComponent interface. */
  IPnPLComponent_t component_if;
};

/* Objects instance ----------------------------------------------------------*/
static Ism6hg256x_Acc_PnPL sIsm6hg256x_Acc_PnPL;

/* Public API definition -----------------------------------------------------*/
IPnPLComponent_t *Ism6hg256x_Acc_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sIsm6hg256x_Acc_PnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sIsm6hg256x_Acc_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t Ism6hg256x_Acc_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  ism6hg256x_acc_comp_init();
  return PNPL_NO_ERROR_CODE;
}


/* IPnPLComponent virtual functions definition -------------------------------*/
char *Ism6hg256x_Acc_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return ism6hg256x_acc_get_key();
}

uint8_t Ism6hg256x_Acc_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char *Ism6hg256x_Acc_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t Ism6hg256x_Acc_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size,
                                          uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  bool temp_b = 0;
  ism6hg256x_acc_get_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "ism6hg256x_acc.enable", temp_b);
  char *temp_s = "";
  ism6hg256x_acc_get_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.data_type", temp_s);
  int32_t temp_i = 0;
  ism6hg256x_acc_get_dimensions(&temp_i);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.dimensions", temp_i);
  ism6hg256x_acc_get_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.unit", temp_s);
  ism6hg256x_acc_get_short_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.short_unit", temp_s);
  ism6hg256x_acc_get_sensor_category(&temp_i);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.sensor_category", temp_i);
  ism6hg256x_acc_get_odr_triggered(&temp_b);
  json_object_dotset_boolean(JSON_Status, "ism6hg256x_acc.odr_triggered", temp_b);
  pnpl_ism6hg256x_acc_odr_t temp_odr_e = (pnpl_ism6hg256x_acc_odr_t)0;
  ism6hg256x_acc_get_odr(&temp_odr_e);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.odr", temp_odr_e);
  pnpl_ism6hg256x_acc_odr_t_t temp_odr_t_e = (pnpl_ism6hg256x_acc_odr_t_t)0;
  ism6hg256x_acc_get_odr_t(&temp_odr_t_e);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.odr_t", temp_odr_t_e);
  pnpl_ism6hg256x_acc_fs_t temp_fs_e = (pnpl_ism6hg256x_acc_fs_t)0;
  ism6hg256x_acc_get_fs(&temp_fs_e);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.fs", temp_fs_e);
  float_t temp_f = 0;
  ism6hg256x_acc_get_initial_timestamp(&temp_f);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.initial_timestamp", temp_f);
  ism6hg256x_acc_get_measured_odr(&temp_f);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.measured_odr", temp_f);
  ism6hg256x_acc_get_sensitivity(&temp_f);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.sensitivity", temp_f);
  ism6hg256x_acc_get_annotation(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.annotation", temp_s);
  pnpl_ism6hg256x_acc_output_st_stream__channel_specification_interface_t
  temp_output_st_stream__channel_specification_interface_e =
    (pnpl_ism6hg256x_acc_output_st_stream__channel_specification_interface_t)0;
  ism6hg256x_acc_get_output_st_stream__channel_specification_interface(
    &temp_output_st_stream__channel_specification_interface_e);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.output_st_stream.channel_specification.interface",
                            temp_output_st_stream__channel_specification_interface_e);
  pnpl_ism6hg256x_acc_output_st_stream__channel_specification_protocol_type_t
  temp_output_st_stream__channel_specification_protocol_type_e =
    (pnpl_ism6hg256x_acc_output_st_stream__channel_specification_protocol_type_t)0;
  ism6hg256x_acc_get_output_st_stream__channel_specification_protocol_type(
    &temp_output_st_stream__channel_specification_protocol_type_e);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.output_st_stream.channel_specification.protocol_type",
                            temp_output_st_stream__channel_specification_protocol_type_e);
  ism6hg256x_acc_get_output_st_stream__channel_specification_protocol_version(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.channel_specification.protocol_version",
                            temp_s);
  pnpl_ism6hg256x_acc_output_st_stream__channel_specification_payload_type_t
  temp_output_st_stream__channel_specification_payload_type_e =
    (pnpl_ism6hg256x_acc_output_st_stream__channel_specification_payload_type_t)0;
  ism6hg256x_acc_get_output_st_stream__channel_specification_payload_type(
    &temp_output_st_stream__channel_specification_payload_type_e);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.output_st_stream.channel_specification.payload_type",
                            temp_output_st_stream__channel_specification_payload_type_e);
  ism6hg256x_acc_get_output_st_stream__channel_specification_payload_version(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.channel_specification.payload_version", temp_s);
  ism6hg256x_acc_get_output_st_stream__channel_specification_stream_id(&temp_i);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.output_st_stream.channel_specification.stream_id", temp_i);
  ism6hg256x_acc_get_output_st_stream__payload_specification_timestamp_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.timestamp.data_type",
                            temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_timestamp_dimensions(&temp_i);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.timestamp.dimensions",
                            temp_i);
  ism6hg256x_acc_get_output_st_stream__payload_specification_timestamp_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.timestamp.unit", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_timestamp_short_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.timestamp.short_unit",
                            temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_timestamp_labels(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.timestamp.labels",
                            temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_x_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.x.data_type", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_x_dimensions(&temp_i);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.x.dimensions", temp_i);
  ism6hg256x_acc_get_output_st_stream__payload_specification_x_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.x.unit", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_x_short_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.x.short_unit", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_x_labels(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.x.labels", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_y_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.y.data_type", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_y_dimensions(&temp_i);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.y.dimensions", temp_i);
  ism6hg256x_acc_get_output_st_stream__payload_specification_y_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.y.unit", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_y_short_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.y.short_unit", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_y_labels(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.y.labels", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_z_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.z.data_type", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_z_dimensions(&temp_i);
  json_object_dotset_number(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.z.dimensions", temp_i);
  ism6hg256x_acc_get_output_st_stream__payload_specification_z_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.z.unit", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_z_short_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.z.short_unit", temp_s);
  ism6hg256x_acc_get_output_st_stream__payload_specification_z_labels(&temp_s);
  json_object_dotset_string(JSON_Status, "ism6hg256x_acc.output_st_stream.payload_specification.z.labels", temp_s);

  if (pretty == 1)
  {
    *serializedJSON = json_serialize_to_string_pretty(tempJSON);
    *size = json_serialization_size_pretty(tempJSON);
  }
  else
  {
    *serializedJSON = json_serialize_to_string(tempJSON);
    *size = json_serialization_size(tempJSON);
  }

  /* No need to free temp_j as it is part of tempJSON */
  json_value_free(tempJSON);

  return PNPL_NO_ERROR_CODE;
}

uint8_t Ism6hg256x_Acc_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = PNPL_NO_ERROR_CODE;
  if (json_object_dothas_value(tempJSONObject, "ism6hg256x_acc.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "ism6hg256x_acc.enable");
    ret = ism6hg256x_acc_set_enable(enable);
  }
  if (json_object_dothas_value(tempJSONObject, "ism6hg256x_acc.odr_triggered"))
  {
    bool odr_triggered = json_object_dotget_boolean(tempJSONObject, "ism6hg256x_acc.odr_triggered");
    ret = ism6hg256x_acc_set_odr_triggered(odr_triggered);
  }
  if (json_object_dothas_value(tempJSONObject, "ism6hg256x_acc.odr"))
  {
    int32_t odr = (int32_t)json_object_dotget_number(tempJSONObject, "ism6hg256x_acc.odr");
    ret = ism6hg256x_acc_set_odr((pnpl_ism6hg256x_acc_odr_t)odr);
  }
  if (json_object_dothas_value(tempJSONObject, "ism6hg256x_acc.odr_t"))
  {
    int32_t odr_t = (int32_t)json_object_dotget_number(tempJSONObject, "ism6hg256x_acc.odr_t");
    ret = ism6hg256x_acc_set_odr_t((pnpl_ism6hg256x_acc_odr_t_t)odr_t);
  }
  if (json_object_dothas_value(tempJSONObject, "ism6hg256x_acc.fs"))
  {
    int32_t fs = (int32_t)json_object_dotget_number(tempJSONObject, "ism6hg256x_acc.fs");
    ret = ism6hg256x_acc_set_fs((pnpl_ism6hg256x_acc_fs_t)fs);
  }
  if (json_object_dothas_value(tempJSONObject, "ism6hg256x_acc.annotation"))
  {
    const char *annotation = json_object_dotget_string(tempJSONObject, "ism6hg256x_acc.annotation");
    ret = ism6hg256x_acc_set_annotation(annotation);
  }
  json_value_free(tempJSON);
  return ret;
}


uint8_t Ism6hg256x_Acc_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return PNPL_NO_COMMANDS_ERROR_CODE;
}

