/**
  ******************************************************************************
  * @file    App_model_Log_Controller.c
  * @author  SRA
  * @brief   Log_Controller PnPL Components APIs
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
  * dtmi:appconfig:nucleo_stm32f401re:STAIOTCRAFT_SDATALOG:applications:log_controller;1
  *
  * Created by: DTDL2PnPL_cGen version 3.1.0-alpha.2
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#include "App_model.h"

/* USER includes -------------------------------------------------------------*/
#include "fsm.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "rtc.h"
#include "time.h"
#include "timestamp.h"
#include "ism6hg256x_acc.h"
#include "ism6hg256x_gyro.h"
#include "ism6hg256x_mlc_app.h"
#include <stdbool.h>
#include <sys/types.h>

/* USER private function prototypes ------------------------------------------*/
static uint8_t getCurrentTime(char *time_string);
static bool log_status = false;
static pnpl_ism6hg256x_gyro_odr_t_t gyro_odr;
static pnpl_ism6hg256x_acc_odr_t_t acc_odr;
uint32_t log_interface = 0;

/* USER defines --------------------------------------------------------------*/

/* Controller PnPL Component -------------------------------------------------*/
extern AppModel_t app_model;
extern void jump_to_bootloader(void);
extern void ToggleFlashBank(void);
extern void jump_to_bootloader(void);

uint8_t log_controller_comp_init(void)
{
  app_model.log_controller_model.comp_name = log_controller_get_key();

  /* USER Component initialization code */
  return PNPL_NO_ERROR_CODE;
}

char *log_controller_get_key(void)
{
  return "log_controller";
}


uint8_t log_controller_get_log_status(bool *value)
{
  /* USER Code */
  *value = log_status;
  return PNPL_NO_ERROR_CODE;
}

uint8_t log_controller_get_sd_mounted(bool *value)
{
  *value = false;
  return PNPL_NO_ERROR_CODE;
}

uint8_t log_controller_get_sd_failed(bool *value)
{
  *value = false;
  return PNPL_NO_ERROR_CODE;
}

uint8_t log_controller_get_controller_type(int32_t *value)
{
  return PNPL_NO_ERROR_CODE;
}

uint8_t log_controller_save_configuration(void)
{
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}

uint8_t log_controller_start_log(int32_t interface)
{
  bool acc_enabled, gyro_enabled, mlc_enabled;
  bool acc_odr_trgd_enabled, gyro_odr_trgd_enabled;

  ism6hg256x_acc_get_enable(&acc_enabled);
  ism6hg256x_gyro_get_enable(&gyro_enabled);
  ism6hg256x_mlc_get_enable(&mlc_enabled);
  ism6hg256x_acc_get_odr_triggered(&acc_odr_trgd_enabled);
  ism6hg256x_gyro_get_odr_triggered(&gyro_odr_trgd_enabled);

   /* if log is active or if no sensor is enabled return error */
   if (log_status || (!acc_enabled && !gyro_enabled && !mlc_enabled && !acc_odr_trgd_enabled && !gyro_odr_trgd_enabled)) return PNPL_BASE_ERROR_CODE;

  if (acc_enabled)
  {
//    FsmSetEventEx(&AccFsm, FSM_EVT_START_LOG, &acc_odr);
    FsmSetEvent(&AccFsm, FSM_EVT_START_LOG);
    log_status = true;
  }

  if (gyro_enabled)
  {
    FsmSetEvent(&GyroFsm, FSM_EVT_START_LOG);
    log_status = true;
  }

  if (mlc_enabled)
  {
    FsmSetEvent(&MlcFsm, FSM_EVT_START_LOG);
    TS_TIM_MLC_Start();
    log_status = true;
  }

  if (acc_odr_trgd_enabled)
  {
    ism6hg256x_acc_get_odr_t(&acc_odr);
    FsmSetEventEx(&AccFsm, FSM_EVT_START_LOG, &acc_odr);
    log_status = true;
  }

  if (gyro_odr_trgd_enabled)
  {
    ism6hg256x_gyro_get_odr_t(&gyro_odr);
    FsmSetEventEx(&GyroFsm, FSM_EVT_START_LOG, &gyro_odr);
    log_status = true;
  }

  if (log_status && !TS_TIM_VD_IsStarted())
  {
    log_interface = (uint32_t)interface;
    TS_TIM_VD_Start();
    getCurrentTime(app_model.acquisition_information_model.start_time);
  }
  return PNPL_NO_ERROR_CODE;
}

uint8_t log_controller_stop_log(void)
{
  bool acc_enabled, gyro_enabled, mlc_enabled;
  bool acc_odr_trgd_enabled, gyro_odr_trgd_enabled;

  ism6hg256x_acc_get_enable(&acc_enabled);
  ism6hg256x_gyro_get_enable(&gyro_enabled);
  ism6hg256x_mlc_get_enable(&mlc_enabled);
  ism6hg256x_acc_get_odr_triggered(&acc_odr_trgd_enabled);
  ism6hg256x_gyro_get_odr_triggered(&gyro_odr_trgd_enabled);

  if (!log_status || (!acc_enabled && !gyro_enabled && !mlc_enabled && !acc_odr_trgd_enabled && !gyro_odr_trgd_enabled)) return PNPL_BASE_ERROR_CODE;

  getCurrentTime(app_model.acquisition_information_model.end_time);
  PnPLGenerateAcquisitionUUID(app_model.acquisition_information_model.uuid);

  if (acc_enabled)
  {
    FsmSetEvent(&AccFsm, FSM_EVT_STOP_LOG);
    log_status = false;
  }

  if (gyro_enabled)
  {
    FsmSetEvent(&GyroFsm, FSM_EVT_STOP_LOG);
    log_status = false;
  }

  if (mlc_enabled)
  {
    FsmSetEvent(&MlcFsm, FSM_EVT_STOP_LOG);
    log_status = false;
  }

  if (acc_odr_trgd_enabled)
  {
    ism6hg256x_acc_get_odr_t(&acc_odr);
    FsmSetEventEx(&AccFsm, FSM_EVT_STOP_LOG, &acc_odr);
    log_status = false;
  }

  if (gyro_odr_trgd_enabled)
  {
    ism6hg256x_gyro_get_odr_t(&gyro_odr);
    FsmSetEventEx(&GyroFsm, FSM_EVT_STOP_LOG, &gyro_odr);
    log_status = false;
  }

  if (!log_status && TS_TIM_VD_IsStarted())
  {
    getCurrentTime(app_model.acquisition_information_model.end_time);
    TS_TIM_VD_Stop();
  }
  return PNPL_NO_ERROR_CODE;
}

uint8_t log_controller_set_time(const char *datetime)
{
  /* USER Code */
  char datetimeStr[3];

  //internal input format: yyyyMMdd_hh_mm_ss

  RTC_DateTypeDef sdate;
  RTC_TimeTypeDef stime;

  /** extract year string (only the last two digit). It will be necessary to add 2000*/
  datetimeStr[0] = datetime[2];
  datetimeStr[1] = datetime[3];
  datetimeStr[2] = '\0';
  sdate.Year = atoi(datetimeStr);

  /** extract month string */
  datetimeStr[0] = datetime[4];
  datetimeStr[1] = datetime[5];
  sdate.Month = atoi(datetimeStr);

  /** extract day string */
  datetimeStr[0] = datetime[6];
  datetimeStr[1] = datetime[7];
  sdate.Date = atoi(datetimeStr);

  /** Week day initialization (not used)*/
  sdate.WeekDay = RTC_WEEKDAY_MONDAY; //Not used

  /** extract hour string */
  datetimeStr[0] = datetime[9];
  datetimeStr[1] = datetime[10];
  stime.Hours = atoi(datetimeStr);

  /** extract minute string */
  datetimeStr[0] = datetime[12];
  datetimeStr[1] = datetime[13];
  stime.Minutes = atoi(datetimeStr);

  /** extract second string */
  datetimeStr[0] = datetime[15];
  datetimeStr[1] = datetime[16];
  stime.Seconds = atoi(datetimeStr);

  /** not used */
  //stime.TimeFormat = RTC_HOURFORMAT12_AM;
  stime.SecondFraction = 0;
  stime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stime.StoreOperation = RTC_STOREOPERATION_RESET;

  if(HAL_RTC_SetTime(&hrtc, &stime, RTC_FORMAT_BIN) != HAL_OK)
  {
    return PNPL_NO_ERROR_CODE;
  }
  if(HAL_RTC_SetDate(&hrtc, &sdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    return PNPL_NO_ERROR_CODE;
  }

  return PNPL_NO_ERROR_CODE;
}

uint8_t log_controller_switch_bank(void)
{
  /* USER Code */
  jump_to_bootloader();
  return PNPL_NO_ERROR_CODE;
}

uint8_t log_controller_set_dfu_mode(void)
{
  /* USER Code */

  return PNPL_NO_ERROR_CODE;
}

uint8_t log_controller_enable_all(bool status)
{
  /* USER Code */
  (void)status;
  ism6hg256x_acc_set_enable(status);
  ism6hg256x_gyro_set_enable(status);
  ism6hg256x_mlc_set_enable(status);
  return PNPL_NO_ERROR_CODE;
}

/* User defined functions */

static uint8_t getCurrentTime(char *time_string)
{
  RTC_DateTypeDef sdate;
  RTC_TimeTypeDef stime;
  char tmp[12];

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdate, RTC_FORMAT_BIN);

  struct tm t =
  {
      .tm_year = sdate.Year + 2000,
      .tm_mon = sdate.Month - 1,
      .tm_mday = sdate.Date,
      .tm_hour = stime.Hours,
      .tm_min = stime.Minutes,
      .tm_sec = stime.Seconds };

  // WHY THIS -1 (in months) ???
  //  struct tm {
  //     int tm_sec;         /* seconds,  range 0 to 59          */
  //     int tm_min;         /* minutes, range 0 to 59           */
  //     int tm_hour;        /* hours, range 0 to 23             */
  //     int tm_mday;        /* day of the month, range 1 to 31  */
  //     int tm_mon;         /* month, range 0 to 11             */ <------ (-1) months here (0..11), months from RTC (1..12)
  //     int tm_year;        /* The number of years since 1900   */
  //     int tm_wday;        /* day of the week, range 0 to 6    */
  //     int tm_yday;        /* day in the year, range 0 to 365  */
  //     int tm_isdst;       /* daylight saving time             */
  //  };

  snprintf(time_string, ACQ_TIMESTAMP_LENGTH, "%04d-%02d-%02dT", t.tm_year, t.tm_mon + 1, t.tm_mday);
  snprintf(tmp, 12, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
  strncat(time_string, (const char *)tmp, ACQ_TIMESTAMP_LENGTH);

  /* last part not done in sprintf to avoid a warning  */
  time_string[19] = '.';
  time_string[20] = '0';
  time_string[21] = '0';
  time_string[22] = '0';
  time_string[23] = 'Z';
  time_string[24] = '\0';

  return PNPL_NO_ERROR_CODE;
}

