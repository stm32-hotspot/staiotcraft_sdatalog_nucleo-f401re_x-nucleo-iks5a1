/**
  ******************************************************************************
  * @file    Acquisition_Information_PnPL.c
  * @author  SRA
  * @brief   Acquisition_Information PnPL Component Manager
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
  * dtmi:appconfig:nucleo_stm32f401re:STAIOTCRAFT_SDATALOG:services:acquisition_information;1
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

#include "Acquisition_Information_PnPL.h"

static const IPnPLComponent_vtbl sAcquisition_Information_PnPL_CompIF_vtbl =
{
  Acquisition_Information_PnPL_vtblGetKey,
  Acquisition_Information_PnPL_vtblGetNCommands,
  Acquisition_Information_PnPL_vtblGetCommandKey,
  Acquisition_Information_PnPL_vtblGetStatus,
  Acquisition_Information_PnPL_vtblSetProperty,
  Acquisition_Information_PnPL_vtblExecuteFunction
};

/**
  *  Acquisition_Information_PnPL internal structure.
  */
struct _Acquisition_Information_PnPL
{
  /* Implements the IPnPLComponent interface. */
  IPnPLComponent_t component_if;
};

/* Objects instance ----------------------------------------------------------*/
static Acquisition_Information_PnPL sAcquisition_Information_PnPL;

/* Public API definition -----------------------------------------------------*/
IPnPLComponent_t *Acquisition_Information_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sAcquisition_Information_PnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sAcquisition_Information_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t Acquisition_Information_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  acquisition_information_comp_init();
  return PNPL_NO_ERROR_CODE;
}


/* IPnPLComponent virtual functions definition -------------------------------*/
char *Acquisition_Information_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return acquisition_information_get_key();
}

uint8_t Acquisition_Information_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char *Acquisition_Information_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t Acquisition_Information_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size,
                                                   uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  JSON_Value *temp_j = json_value_init_object();
  acquisition_information_get_tags(temp_j);
  json_object_set_value(JSON_Status, "acquisition_information", temp_j);
  char *temp_s = "";
  acquisition_information_get_acquisition_name(&temp_s);
  json_object_dotset_string(JSON_Status, "acquisition_information.acquisition_name", temp_s);
  acquisition_information_get_acquisition_description(&temp_s);
  json_object_dotset_string(JSON_Status, "acquisition_information.acquisition_description", temp_s);
  acquisition_information_get_acquisition_uuid(&temp_s);
  json_object_dotset_string(JSON_Status, "acquisition_information.acquisition_uuid", temp_s);
  acquisition_information_get_start_time(&temp_s);
  json_object_dotset_string(JSON_Status, "acquisition_information.start_time", temp_s);
  acquisition_information_get_end_time(&temp_s);
  json_object_dotset_string(JSON_Status, "acquisition_information.end_time", temp_s);
  acquisition_information_get_exported_data_extension(&temp_s);
  json_object_dotset_string(JSON_Status, "acquisition_information.exported_data_extension", temp_s);
  acquisition_information_get_exported_data_format(&temp_s);
  json_object_dotset_string(JSON_Status, "acquisition_information.exported_data_format", temp_s);
  pnpl_acquisition_information_interface_t temp_interface_e = (pnpl_acquisition_information_interface_t)0;
  acquisition_information_get_interface(&temp_interface_e);
  json_object_dotset_number(JSON_Status, "acquisition_information.interface", temp_interface_e);
  acquisition_information_get_schema_version(&temp_s);
  json_object_dotset_string(JSON_Status, "acquisition_information.schema_version", temp_s);
  json_object_dotset_number(JSON_Status, "acquisition_information.c_type", COMP_TYPE_SERVICE);

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

uint8_t Acquisition_Information_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = PNPL_NO_ERROR_CODE;
  if (json_object_dothas_value(tempJSONObject, "acquisition_information.acquisition_name"))
  {
    const char *acquisition_name = json_object_dotget_string(tempJSONObject, "acquisition_information.acquisition_name");
    ret = acquisition_information_set_acquisition_name(acquisition_name);
  }
  if (json_object_dothas_value(tempJSONObject, "acquisition_information.acquisition_description"))
  {
    const char *acquisition_description = json_object_dotget_string(tempJSONObject,
                                                                    "acquisition_information.acquisition_description");
    ret = acquisition_information_set_acquisition_description(acquisition_description);
  }
  json_value_free(tempJSON);
  return ret;
}


uint8_t Acquisition_Information_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return PNPL_NO_COMMANDS_ERROR_CODE;
}

