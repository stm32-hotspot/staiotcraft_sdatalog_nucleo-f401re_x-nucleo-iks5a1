/**
  ******************************************************************************
  * @file    App_model_Device_Information.h
  * @author  SRA
  * @brief   Device_Information PnPL Components APIs
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

#ifndef APP_MODEL_DEVICE_INFORMATION_H_
#define APP_MODEL_DEVICE_INFORMATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
/* USER includes -------------------------------------------------------------*/

typedef struct _DeviceInformationModel_t
{
  char *comp_name;
  /* DeviceInformation Component Model USER code */
} DeviceInformationModel_t;

/* Information PnPL Component ------------------------------------------------- */
uint8_t device_information_comp_init(void);
char *device_information_get_key(void);
uint8_t device_information_get_device_alias(char **value);
uint8_t device_information_get_device_manufacturer(char **value);
uint8_t device_information_get_device_name(char **value);
uint8_t device_information_get_processor_manufacturer(char **value);
uint8_t device_information_get_processor_architecture(char **value);
uint8_t device_information_get_total_storage_megabytes(float_t *value);
uint8_t device_information_get_available_storage_megabytes(float_t *value);
uint8_t device_information_get_total_rom_memory_megabytes(float_t *value);
uint8_t device_information_get_available_rom_memory_megabytes(float_t *value);
uint8_t device_information_get_total_ram_memory_megabytes(float_t *value);
uint8_t device_information_get_available_ram_memory_megabytes(float_t *value);
uint8_t device_information_get_physical_address(char **value);
uint8_t device_information_get_device_url(char **value);
uint8_t device_information_get_os_name(char **value);
uint8_t device_information_get_os_version(char **value);
uint8_t device_information_get_firmware_part_number(char **value);
uint8_t device_information_get_firmware_name(char **value);
uint8_t device_information_get_firmware_version(char **value);
uint8_t device_information_get_firmware_url(char **value);
uint8_t device_information_set_device_alias(const char *value);


#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_DEVICE_INFORMATION_H_ */
