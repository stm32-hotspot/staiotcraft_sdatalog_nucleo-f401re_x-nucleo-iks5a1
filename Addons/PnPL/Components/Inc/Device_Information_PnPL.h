/**
  ******************************************************************************
  * @file    Device_Information_PnPL.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PNPL_DEVICE_INFORMATION_H_
#define _PNPL_DEVICE_INFORMATION_H_



#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "parson.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"

/* VTBL Functions ------------------------------------------------------------*/
char *Device_Information_PnPL_vtblGetKey(IPnPLComponent_t *_this);
uint8_t Device_Information_PnPL_vtblGetNCommands(IPnPLComponent_t *_this);
char *Device_Information_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id);
uint8_t Device_Information_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size,
                                              uint8_t pretty);
uint8_t Device_Information_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON);
uint8_t Device_Information_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON);

/**
  * Create a type name for _Device_Information_PnPL.
 */
typedef struct _Device_Information_PnPL Device_Information_PnPL;

/* Public API declaration ----------------------------------------------------*/

IPnPLComponent_t *Device_Information_PnPLAlloc(void);

/**
  * Initialize the default parameters.
  *
 */
uint8_t Device_Information_PnPLInit(IPnPLComponent_t *_this);


/**
  * Get the IPnPLComponent interface for the component.
  * @param _this [IN] specifies a pointer to a PnPL component.
  * @return a pointer to the generic object ::IPnPLComponent if success,
  * or NULL if out of memory error occurs.
 */
IPnPLComponent_t *Device_Information_PnPLGetComponentIF(Device_Information_PnPL *_this);

#ifdef __cplusplus
}
#endif

#endif /* _PNPL_DEVICE_INFORMATION_H_ */
