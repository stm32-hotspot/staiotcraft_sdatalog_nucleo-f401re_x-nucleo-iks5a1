/**
  ******************************************************************************
  * @file    App_model_Acquisition_Information.c
  * @author  SRA
  * @brief   Acquisition_Information PnPL Components APIs
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

#include "App_model.h"
#include "parson.h"

/* USER includes -------------------------------------------------------------*/

/* USER private function prototypes ------------------------------------------*/

/* USER defines --------------------------------------------------------------*/

/* Acquisition Information PnPL Component ------------------------------------*/
extern AppModel_t app_model;

uint8_t acquisition_information_comp_init(void)
{
  app_model.acquisition_information_model.comp_name = acquisition_information_get_key();
  
  acquisition_information_set_acquisition_name("NucleoF401RE-IKS5A1_acquisition");
  acquisition_information_set_acquisition_description("");
  app_model.acquisition_information_model.start_time[0] = '\0';
  app_model.acquisition_information_model.end_time[0] = '\0';
  
  /* USER Component initialization code */
  return PNPL_NO_ERROR_CODE;
}

char *acquisition_information_get_key(void)
{
  return "acquisition_information";
}


uint8_t acquisition_information_get_acquisition_name(char **value)
{
  /* USER Code */
  *value = app_model.acquisition_information_model.name;  
  return PNPL_NO_ERROR_CODE;
}

uint8_t acquisition_information_get_acquisition_description(char **value)
{
  /* USER Code */
  *value = app_model.acquisition_information_model.description;  
  return PNPL_NO_ERROR_CODE;
}

uint8_t acquisition_information_get_acquisition_uuid(char **value)
{
  /* USER Code */
  *value = app_model.acquisition_information_model.uuid;
  return PNPL_NO_ERROR_CODE;
}

uint8_t acquisition_information_get_start_time(char **value)
{
  /* USER Code */
  *value = app_model.acquisition_information_model.start_time;
  return PNPL_NO_ERROR_CODE;
}

uint8_t acquisition_information_get_end_time(char **value)
{
  /* USER Code */
  *value = app_model.acquisition_information_model.end_time;
  return PNPL_NO_ERROR_CODE;
}

uint8_t acquisition_information_get_exported_data_extension(char **value)
{
  /* USER Code */
  *value = ".dat";
  return PNPL_NO_ERROR_CODE;
}

uint8_t acquisition_information_get_exported_data_format(char **value)
{
  /* USER Code */
  *value = "CUSTOM";
  return PNPL_NO_ERROR_CODE;
}

uint8_t acquisition_information_get_tags(JSON_Value *value)
{
  /* USER Code */
  if (value == NULL)
  {
    return PNPL_BASE_ERROR_CODE;
  }

  JSON_Object *tags_object = json_value_get_object(value);
  if (tags_object == NULL)
  {
    return PNPL_BASE_ERROR_CODE;
  }

  JSON_Value *tags_value = json_value_init_array();
  if (tags_value == NULL)
  {
    return PNPL_BASE_ERROR_CODE;
  }

  if (json_object_set_value(tags_object, "tags", tags_value) != JSONSuccess)
  {
    json_value_free(tags_value);
    return PNPL_BASE_ERROR_CODE;
  }
  return PNPL_NO_ERROR_CODE;
}

uint8_t acquisition_information_get_interface(pnpl_acquisition_information_interface_t *enum_id)
{
//#error "select the acquisition_information_interface_t enum value corresponding to the extracted interface, then: *enum_id = <selected_enum_id>;"
  /* USER Code */
  /*
   * Only Serial interface is supported by this FW
   **/
  *enum_id = pnpl_acquisition_information_interface_serial;  
  return PNPL_NO_ERROR_CODE;
}

uint8_t acquisition_information_get_schema_version(char **value)
{
  /* USER Code */
  *value = "3.0.0";  
  return PNPL_NO_ERROR_CODE;
}


uint8_t acquisition_information_set_acquisition_name(const char *value)
{
  /* USER Code */
  strncpy(app_model.acquisition_information_model.name, value, ACQ_NAME_LENGTH);
  return PNPL_NO_ERROR_CODE;
}

uint8_t acquisition_information_set_acquisition_description(const char *value)
{
  /* USER Code */
  strncpy(app_model.acquisition_information_model.description, value, ACQ_DESC_LENGTH);
  return PNPL_NO_ERROR_CODE;
}



