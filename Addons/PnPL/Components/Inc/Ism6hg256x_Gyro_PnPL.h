/**
  ******************************************************************************
  * @file    Ism6hg256x_Gyro_PnPL.h
  * @author  SRA
  * @brief   Ism6hg256x_Gyro PnPL Component Manager
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PNPL_ISM6HG256X_GYRO_H_
#define _PNPL_ISM6HG256X_GYRO_H_

typedef enum
{
  pnpl_ism6hg256x_gyro_odr_hz7_5 = 0,
  pnpl_ism6hg256x_gyro_odr_hz15 = 1,
  pnpl_ism6hg256x_gyro_odr_hz30 = 2,
  pnpl_ism6hg256x_gyro_odr_hz60 = 3,
  pnpl_ism6hg256x_gyro_odr_hz120 = 4,
  pnpl_ism6hg256x_gyro_odr_hz240 = 5,
  pnpl_ism6hg256x_gyro_odr_hz480 = 6,
  pnpl_ism6hg256x_gyro_odr_hz960 = 7,
  pnpl_ism6hg256x_gyro_odr_hz1920 = 8,
  pnpl_ism6hg256x_gyro_odr_hz3840 = 9,
  pnpl_ism6hg256x_gyro_odr_hz7680 = 10,
} pnpl_ism6hg256x_gyro_odr_t;
typedef enum
{
  pnpl_ism6hg256x_gyro_odr_t_hz12_5 = 0,
  pnpl_ism6hg256x_gyro_odr_t_hz25 = 1,
  pnpl_ism6hg256x_gyro_odr_t_hz50 = 2,
  pnpl_ism6hg256x_gyro_odr_t_hz100 = 3,
  pnpl_ism6hg256x_gyro_odr_t_hz200 = 4,
  pnpl_ism6hg256x_gyro_odr_t_hz400 = 5,
  pnpl_ism6hg256x_gyro_odr_t_hz800 = 6,
  pnpl_ism6hg256x_gyro_odr_t_hz1600 = 7,
  pnpl_ism6hg256x_gyro_odr_t_hz3200 = 8,
} pnpl_ism6hg256x_gyro_odr_t_t;
typedef enum
{
  pnpl_ism6hg256x_gyro_fs_dps250 = 1,
  pnpl_ism6hg256x_gyro_fs_dps500 = 2,
  pnpl_ism6hg256x_gyro_fs_dps1000 = 3,
  pnpl_ism6hg256x_gyro_fs_dps2000 = 4,
  pnpl_ism6hg256x_gyro_fs_dps4000 = 5,
} pnpl_ism6hg256x_gyro_fs_t;
typedef enum
{
  //String Enum --> enum value is translated to be an integer following the definition order in the Device Model.
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_interface_sd = 0,
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_interface_usb = 1,
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_interface_ble = 2,
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_interface_serial = 3,
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_interface_link = 4,
} pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_interface_t;
typedef enum
{
  //String Enum --> enum value is translated to be an integer following the definition order in the Device Model.
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_protocol_type_hsd = 0,
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_protocol_type_bluest = 1,
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_protocol_type_aspep_sstl = 2,
} pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_protocol_type_t;
typedef enum
{
  //String Enum --> enum value is translated to be an integer following the definition order in the Device Model.
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_payload_type_custom = 0,
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_payload_type_standard = 1,
  pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_payload_type_hsd = 2,
} pnpl_ism6hg256x_gyro_output_st_stream__channel_specification_payload_type_t;


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "parson.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"

/* VTBL Functions ------------------------------------------------------------*/
char *Ism6hg256x_Gyro_PnPL_vtblGetKey(IPnPLComponent_t *_this);
uint8_t Ism6hg256x_Gyro_PnPL_vtblGetNCommands(IPnPLComponent_t *_this);
char *Ism6hg256x_Gyro_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id);
uint8_t Ism6hg256x_Gyro_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size,
                                           uint8_t pretty);
uint8_t Ism6hg256x_Gyro_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON);
uint8_t Ism6hg256x_Gyro_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON);

/**
  * Create a type name for _Ism6hg256x_Gyro_PnPL.
 */
typedef struct _Ism6hg256x_Gyro_PnPL Ism6hg256x_Gyro_PnPL;

/* Public API declaration ----------------------------------------------------*/

IPnPLComponent_t *Ism6hg256x_Gyro_PnPLAlloc(void);

/**
  * Initialize the default parameters.
  *
 */
uint8_t Ism6hg256x_Gyro_PnPLInit(IPnPLComponent_t *_this);


/**
  * Get the IPnPLComponent interface for the component.
  * @param _this [IN] specifies a pointer to a PnPL component.
  * @return a pointer to the generic object ::IPnPLComponent if success,
  * or NULL if out of memory error occurs.
 */
IPnPLComponent_t *Ism6hg256x_Gyro_PnPLGetComponentIF(Ism6hg256x_Gyro_PnPL *_this);

#ifdef __cplusplus
}
#endif

#endif /* _PNPL_ISM6HG256X_GYRO_H_ */
