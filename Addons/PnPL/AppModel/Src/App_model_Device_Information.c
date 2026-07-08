/**
  ******************************************************************************
  * @file    App_model_Device_Information.c
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
  *
  * Created by: DTDL2PnPL_cGen version 3.1.0-alpha.2
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#include "App_model.h"

/* USER includes -------------------------------------------------------------*/

/* USER private function prototypes ------------------------------------------*/

/* USER defines --------------------------------------------------------------*/

/* Information PnPL Component ------------------------------------------------*/
extern AppModel_t app_model;

uint8_t device_information_comp_init(void)
{
  app_model.device_information_model.comp_name = device_information_get_key();

  /* USER Component initialization code */
  return PNPL_NO_ERROR_CODE;
}

char *device_information_get_key(void)
{
  return "device_information";
}


uint8_t device_information_get_device_alias(char **value)
{
  /* USER Code */
  (void)value;
  *value = "";
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_device_manufacturer(char **value)
{
  /* USER Code */
  *value = "STMicroelectronics";
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_device_name(char **value)
{
  /* USER Code */
  *value = "NUCLEO-STM32F401RE";
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_processor_manufacturer(char **value)
{
  /* USER Code */
  *value = "STMicroelectronics";
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_processor_architecture(char **value)
{
  /* USER Code */
  *value = "ARM Cortex-M4";
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_total_storage_megabytes(float_t *value)
{
  /* USER Code */
  *value = 0.093;
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_available_storage_megabytes(float_t *value)
{
  /* USER Code */
  (void)value;
  *value = 0.08;
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_total_rom_memory_megabytes(float_t *value)
{
  /* USER Code */
  *value = 0.512;
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_available_rom_memory_megabytes(float_t *value)
{
  /* USER Code */
  (void)value; 
  *value = 0.456;
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_total_ram_memory_megabytes(float_t *value)
{
  /* USER Code */
  *value = 0.096;
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_available_ram_memory_megabytes(float_t *value)
{
  /* USER Code */
  (void)value;
  *value = 0.078;
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_physical_address(char **value)
{
  /* USER Code */
  *value = "";
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_device_url(char **value)
{
  /* USER Code */
  *value = "https://www.st.com/en/evaluation-tools/nucleo-f401re.html";
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_os_name(char **value)
{
  /* USER Code */
  *value = "";
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_os_version(char **value)
{
  /* USER Code */
  *value = "";
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_firmware_part_number(char **value)
{
  /* USER Code */
  *value = "STAIOTCRAFT_SDATALOG"; 
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_firmware_name(char **value)
{
  /* USER Code */
  *value = "STAIOTCRAFT_SDATALOG";
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_firmware_version(char **value)
{
  /* USER Code */
  *value = "1.0.0";
  return PNPL_NO_ERROR_CODE;
}

uint8_t device_information_get_firmware_url(char **value)
{
  /* USER Code */
  *value = "https://github.com/stm32-hotspot";
  return PNPL_NO_ERROR_CODE;
}


uint8_t device_information_set_device_alias(const char *value)
{
  /* USER Code */
  (void)value;
  return PNPL_NO_ERROR_CODE;
}



