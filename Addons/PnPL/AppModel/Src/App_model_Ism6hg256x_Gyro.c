/**
  ******************************************************************************
  * @file    App_model_Ism6hg256x_Gyro.c
  * @author  SRA
  * @brief   Ism6hg256x_Gyro PnPL Components APIs
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
  * dtmi:appconfig:nucleo_stm32f401re:STAIOTCRAFT_SDATALOG:sensors:ism6hg256x_gyro;1
  *
  * Created by: DTDL2PnPL_cGen version 3.1.0-alpha.2
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#include "App_model.h"

/* USER includes -------------------------------------------------------------*/
#include "custom_motion_sensors.h"
#include "custom_motion_sensors_ex.h"
#include "vdatalog_conf.h"
#include "timestamp.h"
#include "ism6hg256x_gyro.h"
#include <stdint.h>
#include <string.h>
/* USER private function prototypes ------------------------------------------*/

/* USER defines --------------------------------------------------------------*/

/* ISM6HG256X_GYRO PnPL Component --------------------------------------------*/
static SensorModel_t ism6hg256x_gyro_model;
static pnpl_ism6hg256x_gyro_odr_t_t odr_trgd = DEFAULT_GYRO_ODR_TRGD_HZ;
static char gyro_annotation[85];
extern bool g_odr_t_trgd_sync_in_progress;
extern AppModel_t app_model;
extern uint8_t    MlcSensorInUse; // variable to track which sensor is used by the MLC model (0: none, 1: acc, 2: gyro, 4 magneto)

uint8_t ism6hg256x_gyro_comp_init(void)
{
  ism6hg256x_gyro_model.comp_name = ism6hg256x_gyro_get_key();

  uint16_t id;
  if (getNextSensorId(&id) != 0)
  {
    /* Sensor slot not available */
    return PNPL_BASE_ERROR_CODE;
  }
  addSensorToAppModel(id, &ism6hg256x_gyro_model);

  CUSTOM_MOTION_SENSOR_Disable(SENSOR_0, MOTION_GYRO);  
  ism6hg256x_gyro_set_odr(DEFAULT_GYRO_ODR_HZ);
  ism6hg256x_gyro_set_fs(DEFAULT_GYRO_FS_DPS);
  ism6hg256x_gyro_set_odr_t(DEFAULT_GYRO_ODR_TRGD_HZ);
  /* USER Component initialization code */
  return PNPL_NO_ERROR_CODE;
}

char *ism6hg256x_gyro_get_key(void)
{
  return "ism6hg256x_gyro";
}


uint8_t ism6hg256x_gyro_get_enable(bool *value)
{
  /* USER Code */
  if (value == NULL)
  { 
    return PNPL_BASE_ERROR_CODE;
  }

  if (FsmCheckState(&GyroFsm, FSM_STATE_ENABLED)) {*value = true; return PNPL_NO_ERROR_CODE; }
  if (FsmCheckState(&GyroFsm, FSM_STATE_LOGGING_DATA)) {*value = true; return PNPL_NO_ERROR_CODE; }
  *value = false;

  return PNPL_NO_ERROR_CODE;
  #if 0
  float_t odr;
  CUSTOM_MOTION_SENSOR_GetOutputDataRate(SENSOR_0, MOTION_GYRO, &odr);
  if (odr != 0.0f)
  {
    *value = true;
  }
  else
  {
    *value = false;
  } 
  return PNPL_NO_ERROR_CODE;
  #endif
}

uint8_t ism6hg256x_gyro_get_data_type(char **value)
{
  /* USER Code */
  *value = "int16_t"; 
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_dimensions(int32_t *value)
{
  /* USER Code */
  *value = 3;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_unit(char **value)
{
  /* USER Code */
  *value = "degrees/s";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_short_unit(char **value)
{
  /* USER Code */
  *value = "dps";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_sensor_category(int32_t *value)
{
  /* USER Code */
  *value = 0; 
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_odr_triggered(bool *value)
{
  /* USER Code */
  if (value == NULL)
  {
    return PNPL_BASE_ERROR_CODE;
  }

  if (FsmCheckState(&GyroFsm, FSM_STATE_ENABLED_ODR_TRGD)) {*value = true; return PNPL_NO_ERROR_CODE; }
  if (FsmCheckState(&GyroFsm, FSM_STATE_LOGGING_DATA_ODR_TRGD)) {*value = true; return PNPL_NO_ERROR_CODE; }
  *value = false;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_odr_t(pnpl_ism6hg256x_gyro_odr_t_t *enum_id)
{
//#error "select the ism6hg256x_gyro_odr_t_t enum value corresponding to the extracted odr_t, then: *enum_id = <selected_enum_id>;"
  /* USER Code */
  *enum_id = odr_trgd;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_odr(pnpl_ism6hg256x_gyro_odr_t *enum_id)
{
//#error "select the ism6hg256x_gyro_odr_t enum value corresponding to the extracted odr, then: *enum_id = <selected_enum_id>;"
  /* USER Code */
  float odr;

  MY_CUSTOM_MOTION_SENSOR_GetOutputDataRate(SENSOR_0, MOTION_GYRO, &odr);
  
  if (odr == 7.5)
  {
    *enum_id = pnpl_ism6hg256x_gyro_odr_hz7_5;
  } else if (odr == 15.0)
  {
    *enum_id = pnpl_ism6hg256x_gyro_odr_hz15;
  } else if (odr == 30.0)
  {
    *enum_id = pnpl_ism6hg256x_gyro_odr_hz30;
  } else if (odr == 60.0)
  {
    *enum_id = pnpl_ism6hg256x_gyro_odr_hz60;
  } else if (odr == 120.0)
  {
    *enum_id = pnpl_ism6hg256x_gyro_odr_hz120;
  } else if (odr == 240.0)
  {
    *enum_id = pnpl_ism6hg256x_gyro_odr_hz240;
  } else if (odr == 480.0)
  {
    *enum_id = pnpl_ism6hg256x_gyro_odr_hz480;
  } else if (odr == 960.0)
  {
    *enum_id = pnpl_ism6hg256x_gyro_odr_hz960;
  } else if (odr == 1920.0)
  {
    *enum_id = pnpl_ism6hg256x_gyro_odr_hz1920;
  } else if (odr == 3840.0)
  {
    *enum_id = pnpl_ism6hg256x_gyro_odr_hz3840;
  } else if (odr == 7680.0)
  {
    *enum_id = pnpl_ism6hg256x_gyro_odr_hz7680;
  } else 
  {
    /* not supported ODR */
    *enum_id =DEFAULT_GYRO_ODR_HZ;
    return PNPL_BASE_ERROR_CODE; 
  }
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_fs(pnpl_ism6hg256x_gyro_fs_t *enum_id)
{
//#error "select the ism6hg256x_gyro_fs_t enum value corresponding to the extracted fs, then: *enum_id = <selected_enum_id>;"
  /* USER Code */
  int32_t full_scale;
  CUSTOM_MOTION_SENSOR_GetFullScale(SENSOR_0, MOTION_GYRO, &full_scale);
  if (full_scale == 250)
  {
    *enum_id = pnpl_ism6hg256x_gyro_fs_dps250;
  } else if (full_scale == 500)
  {
    *enum_id = pnpl_ism6hg256x_gyro_fs_dps500;
  } else if (full_scale == 1000)
  {
    *enum_id = pnpl_ism6hg256x_gyro_fs_dps1000;
  } else if (full_scale == 2000)
  {
    *enum_id = pnpl_ism6hg256x_gyro_fs_dps2000;
  } else if (full_scale == 4000)
  {
    *enum_id = pnpl_ism6hg256x_gyro_fs_dps4000;
  } else 
  {
    /* not supported FS */
    *enum_id = DEFAULT_GYRO_FS_DPS;
    return PNPL_BASE_ERROR_CODE; 
  }

  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_initial_timestamp(float_t *value)
{
  /* USER Code */
  *value = TS_VD_GetTimeStamp();
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_measured_odr(float_t *value)
{
  /* USER Code */
  uint32_t odr_coeff;
  uint8_t freq_fine_raw;
  int8_t freq_fine;
  float odr;

  CUSTOM_MOTION_SENSOR_GetOutputDataRate(SENSOR_0, MOTION_GYRO, &odr);

  if (odr == 7.5)
  {
    odr_coeff = 1024;
  } else if (odr == 15.0)
  {
    odr_coeff = 512;
  } else if (odr == 30.0)
  {
    odr_coeff = 256;
  } else if (odr == 60.0)
  {
    odr_coeff = 128;
  } else if (odr == 120.0)
  {
    odr_coeff = 64;
  } else if (odr == 240.0)
  {
    odr_coeff = 32;
  } else if (odr == 480.0)
  {
    odr_coeff = 16;
  } else if (odr == 960.0)
  {
    odr_coeff = 8;
  } else if (odr == 1920.0)
  {
    odr_coeff = 4;
  } else if (odr == 3840.0)
  {
    odr_coeff = 2;
  } else if (odr == 7680.0)
  {
    odr_coeff = 1;
  } else
  {
    /* not supported ODR */
    *value = 0.0f;
    return PNPL_BASE_ERROR_CODE; 
  }

  CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_INTERNAL_FREQ, &freq_fine_raw);
  freq_fine = (int8_t)freq_fine_raw;
	*value = (7680 * (1 + 0.0013*(float )freq_fine))/odr_coeff;

  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_sensitivity(float_t *value)
{
  /* USER Code */
  // retrieve the actual sensitivity value from the sensor 
  CUSTOM_MOTION_SENSOR_GetSensitivity(SENSOR_0, MOTION_GYRO, value);      
  *value = *value/1000;  
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_annotation(char **value)
{
  /* USER Code */
  *value = gyro_annotation;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__channel_specification_interface(
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_interface_t *enum_id)
{
  /* USER Code */
  *enum_id = pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_interface_serial;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__channel_specification_protocol_type(
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_protocol_type_t *enum_id)
{
  /* USER Code */
  *enum_id = pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_protocol_type_aspep_sstl;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__channel_specification_protocol_version(char **value)
{
  /* USER Code */
  *value = "V1.0";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__channel_specification_payload_type(
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_payload_type_t *enum_id)
{
  /* USER Code */
  *enum_id = pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_payload_type_custom;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__channel_specification_payload_version(char **value)
{
  /* USER Code */
  *value = "V1.0";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__channel_specification_stream_id(int32_t *value)
{
  /* USER Code */
  *value = 1;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_timestamp_data_type(char **value)
{
  /* USER Code */
  *value = "double";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_timestamp_dimensions(int32_t *value)
{
  /* USER Code */
  *value = 1;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_timestamp_unit(char **value)
{
  /* USER Code */
  *value = "seconds";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_timestamp_short_unit(char **value)
{
  /* USER Code */
  *value = "s";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_timestamp_labels(char **value)
{
  /* USER Code */
  *value = "['t']";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_x_data_type(char **value)
{
  /* USER Code */
  *value ="int16_t";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_x_dimensions(int32_t *value)
{
  /* USER Code */
  *value = 1;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_x_unit(char **value)
{
  /* USER Code */
  *value = "degrees/s";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_x_short_unit(char **value)
{
  /* USER Code */
  *value = "dps";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_x_labels(char **value)
{
  /* USER Code */
  *value = "['x']";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_y_data_type(char **value)
{
  /* USER Code */
  *value = "int16_t";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_y_dimensions(int32_t *value)
{
  /* USER Code */
  *value = 1;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_y_unit(char **value)
{
  /* USER Code */
  *value = "degrees/s";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_y_short_unit(char **value)
{
  /* USER Code */
  *value = "dps";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_y_labels(char **value)
{
  /* USER Code */
  *value = "['y']";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_z_data_type(char **value)
{
  /* USER Code */
  *value = "int16_t";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_z_dimensions(int32_t *value)
{
  /* USER Code */
  *value = 1;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_z_unit(char **value)
{
  /* USER Code */
  *value = "degrees/s";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_z_short_unit(char **value)
{
  /* USER Code */
  *value = "dps";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_get_output_st_stream__payload_specification_z_labels(char **value)
{
  /* USER Code */ 
  *value = "['z']";
  return PNPL_NO_ERROR_CODE;
}


uint8_t ism6hg256x_gyro_set_enable(bool value)
{
  /* USER Code */
  bool log_status, gyro_status, acc_enabled;
  float odr_gyro;
  float odr_acc;
  log_controller_get_log_status(&log_status);  
  if (log_status) {
    ism6hg256x_gyro_set_annotation("Logging active: change not allowed");      
    return PNPL_BASE_ERROR_CODE;
  } 
  ism6hg256x_gyro_get_odr_triggered(&gyro_status);
  if (gyro_status) {
    ism6hg256x_gyro_set_annotation("ODR triggered mode enabled: change not allowed");      
    return PNPL_BASE_ERROR_CODE;
  }
  ism6hg256x_gyro_set_annotation(""); //

  if (value)
  {
    ism6hg256x_acc_get_enable(&acc_enabled);
    if (acc_enabled)
    {
      MY_CUSTOM_MOTION_SENSOR_GetOutputDataRate(SENSOR_0, MOTION_ACCELERO, &odr_acc);
      MY_CUSTOM_MOTION_SENSOR_GetOutputDataRate(SENSOR_0, MOTION_GYRO, &odr_gyro);
      if ((odr_acc > 960.0f) || (odr_gyro > 960.0f))
      {
        ism6hg256x_gyro_set_annotation("Insufficient bandwidth for two sensors at this ODR, use no more than 960Hz");
        return PNPL_BASE_ERROR_CODE;
      }
    }
    FsmSetEvent(&GyroFsm, FSM_EVT_ENABLE);     
  } else {
    FsmSetEvent(&GyroFsm, FSM_EVT_DISABLE);         
  }
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_set_odr_triggered(bool value)
{
  /* USER Code */
  bool log_status, gyro_status;
  bool acc_odr_trgd_status;
  pnpl_ism6hg256x_acc_odr_t_t acc_odr_t;
  log_controller_get_log_status(&log_status);
  if (log_status) return PNPL_BASE_ERROR_CODE;
  ism6hg256x_gyro_get_enable(&gyro_status);
  if (gyro_status) return PNPL_BASE_ERROR_CODE;

  if (value)
  {
    ism6hg256x_acc_get_odr_triggered(&acc_odr_trgd_status);
    if (acc_odr_trgd_status)
    {
      ism6hg256x_acc_get_odr_t(&acc_odr_t);
      if ((int32_t)acc_odr_t != (int32_t)odr_trgd)
      {
        return PNPL_BASE_ERROR_CODE;
      }
    }
    FsmSetEvent(&GyroFsm, FSM_EVT_ENABLE_ODR_TRGD);
  }
  else
  {
    FsmSetEvent(&GyroFsm, FSM_EVT_DISABLE);
  }
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_set_odr(pnpl_ism6hg256x_gyro_odr_t enum_id)
{
  /* USER Code */
  bool log_status, acc_status; //, en_status;
   log_controller_get_log_status(&log_status);
//   ism6hg256x_gyro_get_enable(&en_status); // disable ODR change when sensor is disabled (commented for DL2 backward compatibility)
   if (/*!en_status || */ log_status)
   {
     ism6hg256x_gyro_set_annotation("ODR change denied: logging active");
     return PNPL_BASE_ERROR_CODE;
   }
   if (MlcSensorInUse & MOTION_GYRO)
   {
     ism6hg256x_gyro_set_annotation("ODR change denied: gyro used by MLC");
     return PNPL_BASE_ERROR_CODE;
   }
   ism6hg256x_gyro_set_annotation("");

   switch (enum_id)
   {
     case pnpl_ism6hg256x_gyro_odr_hz7_5:
       CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, 7.5);
       break;
     case pnpl_ism6hg256x_gyro_odr_hz15:
       CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, 15);
       break;
     case pnpl_ism6hg256x_gyro_odr_hz30:
       CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, 30);
       break;
     case pnpl_ism6hg256x_gyro_odr_hz60:
       CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, 60);
       break;
     case pnpl_ism6hg256x_gyro_odr_hz120:
       CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, 120);
       break;
     case pnpl_ism6hg256x_gyro_odr_hz240:
       CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, 240);
       break;
     case pnpl_ism6hg256x_gyro_odr_hz480:
       CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, 480);
       break;
     case pnpl_ism6hg256x_gyro_odr_hz960:
       CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, 960);
       break;
     case pnpl_ism6hg256x_gyro_odr_hz1920:
       /* this ODR is not allowed with accelero also because of MCU performance limitation */
       ism6hg256x_acc_get_enable(&acc_status);
       if (acc_status)
       {
         ism6hg256x_gyro_set_annotation("Insufficient bandwidth for two sensors at this ODR, use no more than 960Hz");
         return PNPL_BASE_ERROR_CODE;
       }
       CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, 1920);
       break;
     case pnpl_ism6hg256x_gyro_odr_hz3840:
       /* this ODR is not allowed with accelero also because of MCU performance limitation */
       ism6hg256x_acc_get_enable(&acc_status);
       if (acc_status)
       {
         ism6hg256x_gyro_set_annotation("Insufficient bandwidth for two sensors at this ODR, use no more than 960Hz");
         return PNPL_BASE_ERROR_CODE;
       }
       CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, 3840);
       break;
     case pnpl_ism6hg256x_gyro_odr_hz7680:
       ism6hg256x_gyro_set_annotation("ODR not supported");
       return PNPL_BASE_ERROR_CODE; // 7680Hz ODR is not allowed because of MCU performance limitation
     default: 
       ism6hg256x_gyro_set_annotation("ODR not supported");
       return PNPL_BASE_ERROR_CODE;  
   }
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_set_odr_t(pnpl_ism6hg256x_gyro_odr_t_t enum_id)
{
  /* USER Code */
  bool log_status;
  uint8_t ret;

  if (!g_odr_t_trgd_sync_in_progress)
  {
    log_controller_get_log_status(&log_status);
    if (log_status) return PNPL_BASE_ERROR_CODE;
  }

  odr_trgd = enum_id;

  if (!g_odr_t_trgd_sync_in_progress)
  {
    g_odr_t_trgd_sync_in_progress = true;
    ret = ism6hg256x_acc_set_odr_t((pnpl_ism6hg256x_acc_odr_t_t)enum_id);
    g_odr_t_trgd_sync_in_progress = false;
    if (ret != PNPL_NO_ERROR_CODE)
    {
      return ret;
    }
  }

  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_set_fs(pnpl_ism6hg256x_gyro_fs_t enum_id)
{
  /* USER Code */
  bool log_status; //, en_status;
   log_controller_get_log_status(&log_status);
//   ism6hg256x_gyro_get_enable(&en_status);  // disable FS change when sensor is disabled (commented for DL2 backward compatibility)
   if (/*!en_status ||*/ log_status) return PNPL_BASE_ERROR_CODE;
   if (MlcSensorInUse & MOTION_GYRO) return PNPL_BASE_ERROR_CODE; 

  switch (enum_id)
  {
    case pnpl_ism6hg256x_gyro_fs_dps250:
      CUSTOM_MOTION_SENSOR_SetFullScale(SENSOR_0, MOTION_GYRO, 250);
      break;
    case pnpl_ism6hg256x_gyro_fs_dps500:
      CUSTOM_MOTION_SENSOR_SetFullScale(SENSOR_0, MOTION_GYRO, 500);
      break;
    case pnpl_ism6hg256x_gyro_fs_dps1000:
      CUSTOM_MOTION_SENSOR_SetFullScale(SENSOR_0, MOTION_GYRO, 1000);
      break;
    case pnpl_ism6hg256x_gyro_fs_dps2000:
      CUSTOM_MOTION_SENSOR_SetFullScale(SENSOR_0, MOTION_GYRO, 2000);
      break;
    case pnpl_ism6hg256x_gyro_fs_dps4000:
      CUSTOM_MOTION_SENSOR_SetFullScale(SENSOR_0, MOTION_GYRO, 4000);
      break;
    default:
  }
  return PNPL_NO_ERROR_CODE;
}

uint8_t ism6hg256x_gyro_set_annotation(const char *value)
{
  /* USER Code */
//   bool log_status; //, en_status;
//   log_controller_get_log_status(&log_status);
//   ism6hg256x_gyro_get_enable(&en_status);  // disable annotation change when sensor is disabled (commented for DL2 backward compatibility)
//   if (/*!en_status ||*/ log_status) return PNPL_BASE_ERROR_CODE;

  if (value == NULL)
  {
    gyro_annotation[0] = '\0';
    return PNPL_BASE_ERROR_CODE;
  }

  strncpy(gyro_annotation, value, sizeof(gyro_annotation) - 1U);
  gyro_annotation[sizeof(gyro_annotation) - 1U] = '\0';

  return PNPL_NO_ERROR_CODE;
}



