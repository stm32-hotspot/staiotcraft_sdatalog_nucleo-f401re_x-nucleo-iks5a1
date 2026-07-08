/**
  ******************************************************************************
  * @file    Device_Information_PnPL.c
  * @author  SRA
  * @brief   Device_Information PnPL Component Manager
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
  * dtmi:appconfig:nucleo_stm32f401re:STAIOTCRAFT_SDATALOG:services:device_information;1
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

#include "Device_Information_PnPL.h"

static const IPnPLComponent_vtbl sDevice_Information_PnPL_CompIF_vtbl =
{
  Device_Information_PnPL_vtblGetKey,
  Device_Information_PnPL_vtblGetNCommands,
  Device_Information_PnPL_vtblGetCommandKey,
  Device_Information_PnPL_vtblGetStatus,
  Device_Information_PnPL_vtblSetProperty,
  Device_Information_PnPL_vtblExecuteFunction
};

/**
  *  Device_Information_PnPL internal structure.
  */
struct _Device_Information_PnPL
{
  /* Implements the IPnPLComponent interface. */
  IPnPLComponent_t component_if;
};

/* Objects instance ----------------------------------------------------------*/
static Device_Information_PnPL sDevice_Information_PnPL;

/* Public API definition -----------------------------------------------------*/
IPnPLComponent_t *Device_Information_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sDevice_Information_PnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sDevice_Information_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t Device_Information_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  device_information_comp_init();
  return PNPL_NO_ERROR_CODE;
}


/* IPnPLComponent virtual functions definition -------------------------------*/
char *Device_Information_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return device_information_get_key();
}

uint8_t Device_Information_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char *Device_Information_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t Device_Information_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size,
                                              uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  char *temp_s = "";
  device_information_get_device_alias(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.device_alias", temp_s);
  device_information_get_device_manufacturer(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.device_manufacturer", temp_s);
  device_information_get_device_name(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.device_name", temp_s);
  device_information_get_processor_manufacturer(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.processor_manufacturer", temp_s);
  device_information_get_processor_architecture(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.processor_architecture", temp_s);
  float_t temp_f = 0;
  device_information_get_total_storage_megabytes(&temp_f);
  json_object_dotset_number(JSON_Status, "device_information.total_storage_megabytes", temp_f);
  device_information_get_available_storage_megabytes(&temp_f);
  json_object_dotset_number(JSON_Status, "device_information.available_storage_megabytes", temp_f);
  device_information_get_total_rom_memory_megabytes(&temp_f);
  json_object_dotset_number(JSON_Status, "device_information.total_rom_memory_megabytes", temp_f);
  device_information_get_available_rom_memory_megabytes(&temp_f);
  json_object_dotset_number(JSON_Status, "device_information.available_rom_memory_megabytes", temp_f);
  device_information_get_total_ram_memory_megabytes(&temp_f);
  json_object_dotset_number(JSON_Status, "device_information.total_ram_memory_megabytes", temp_f);
  device_information_get_available_ram_memory_megabytes(&temp_f);
  json_object_dotset_number(JSON_Status, "device_information.available_ram_memory_megabytes", temp_f);
  device_information_get_physical_address(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.physical_address", temp_s);
  device_information_get_device_url(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.device_url", temp_s);
  device_information_get_os_name(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.os_name", temp_s);
  device_information_get_os_version(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.os_version", temp_s);
  device_information_get_firmware_part_number(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.firmware_part_number", temp_s);
  device_information_get_firmware_name(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.firmware_name", temp_s);
  device_information_get_firmware_version(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.firmware_version", temp_s);
  device_information_get_firmware_url(&temp_s);
  json_object_dotset_string(JSON_Status, "device_information.firmware_url", temp_s);
  json_object_dotset_number(JSON_Status, "device_information.c_type", COMP_TYPE_SERVICE);

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

uint8_t Device_Information_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);

  uint8_t ret = PNPL_NO_ERROR_CODE;
  if (json_object_dothas_value(tempJSONObject, "device_information.device_alias"))
  {
    const char *device_alias = json_object_dotget_string(tempJSONObject, "device_information.device_alias");
    ret = device_information_set_device_alias(device_alias);
  }
  json_value_free(tempJSON);
  return ret;
}


uint8_t Device_Information_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON)
{
  return PNPL_NO_COMMANDS_ERROR_CODE;
}

