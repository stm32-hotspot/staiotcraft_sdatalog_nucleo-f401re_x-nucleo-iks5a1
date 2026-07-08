/**
******************************************************************************
* @file    MEMS_integration.c
* @author  Andrea Driutti
* @brief   This file contains an integration for the BSP Motion Sensors Extended interface for custom boards
******************************************************************************
* @attention
*
* Copyright (c) 2026 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "custom_motion_sensors_ex.h"
#include "MEMS_integration.h"

extern void *MotionCompObj[CUSTOM_MOTION_INSTANCES_NBR];


/**
  * @brief  Enable LSM6DSV16X accelero DRDY interrupt on INT2
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MY_LSM6DSV16X_ACC_Enable_DRDY_On_INT2(ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_pin_int_route_t pin_int2_route;

  /* Enable accelero DRDY Interrupts on INT2 */
  if (ism6hg256x_pin_int2_route_get(&(pObj->Ctx), &pin_int2_route) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }
  pin_int2_route.drdy_xl = 1;
  if (ism6hg256x_pin_int2_route_set(&(pObj->Ctx), &pin_int2_route) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Disable LSM6DSV16X accelero DRDY interrupt on INT2
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MY_ISM6HG256X_ACC_Disable_DRDY_On_INT2(ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_pin_int_route_t pin_int2_route;

  /* Disable accelero DRDY Interrupt on INT2 */
  if (ism6hg256x_pin_int2_route_get(&(pObj->Ctx), &pin_int2_route) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }
  pin_int2_route.drdy_xl = 0;
  if (ism6hg256x_pin_int2_route_set(&(pObj->Ctx), &pin_int2_route) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Enable LSM6DSV16X gyro DRDY interrupt on INT2
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MY_LSM6DSV16X_GYRO_Enable_DRDY_On_INT2(ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_pin_int_route_t pin_int2_route;

  /* Enable gyro DRDY Interrupts on INT2 */
  if (ism6hg256x_pin_int2_route_get(&(pObj->Ctx), &pin_int2_route) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }
  pin_int2_route.drdy_g = 1;
  if (ism6hg256x_pin_int2_route_set(&(pObj->Ctx), &pin_int2_route) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
  * @brief  Disable LSM6DSV16X gyro DRDY interrupt on INT2
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t MY_ISM6HG256X_GYRO_Disable_DRDY_On_INT2(ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_pin_int_route_t pin_int2_route;

  /* Disable accelero DRDY Interrupt on INT2 */
  if (ism6hg256x_pin_int2_route_get(&(pObj->Ctx), &pin_int2_route) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }
  pin_int2_route.drdy_g = 0;
  if (ism6hg256x_pin_int2_route_set(&(pObj->Ctx), &pin_int2_route) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

/**
 * @brief  Set FIFO BDR value
 * @param  Instance the device instance
 * @param  Function Motion sensor function
 * @param  Odr FIFO BDR value
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Set_BDR(uint32_t Instance, uint32_t Function, float Bdr)
{
	int32_t ret;

	switch(Instance)
	{
#if (USE_MOTION_SENSOR_IIS3DWB_0 == 1)
	case IIS3DWB_0:
		if(IIS3DWB_FIFO_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
	case CUSTOM_ISM330DHCX_0:
		if((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
		{
			if(ISM330DHCX_FIFO_ACC_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}
		}
		else if((Function & MOTION_GYRO) == MOTION_GYRO)
		{
			if(ISM330DHCX_FIFO_GYRO_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}
		}
		else
		{
			ret = BSP_ERROR_WRONG_PARAM;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
	case CUSTOM_LSM6DSO32X_0:
		if((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
		{
			if(LSM6DSO32X_FIFO_ACC_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}
		}
		else if((Function & MOTION_GYRO) == MOTION_GYRO)
		{
			if(LSM6DSO32X_FIFO_GYRO_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}
		}
		else
		{
			ret = BSP_ERROR_WRONG_PARAM;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
	case CUSTOM_LSM6DSV16X_0:
		if((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
		{
			if(LSM6DSV16X_FIFO_ACC_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}
		}
		else if((Function & MOTION_GYRO) == MOTION_GYRO)
		{
			if(LSM6DSV16X_FIFO_GYRO_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}
		}
		else
		{
			ret = BSP_ERROR_WRONG_PARAM;
		}
		break;
#endif

	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

/**
 * @brief  Set FIFO full interrupt on INT1 pin
 * @param  Instance the device instance
 * @param  Status FIFO full interrupt on INT1 pin
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(uint32_t Instance, uint8_t Status)
{
	int32_t ret;

	switch(Instance)
	{
#if (USE_MOTION_SENSOR_IIS3DWB_0 == 1)
	case IIS3DWB_0:
		if(IIS3DWB_INT1_Set_FIFO_Full(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
	case CUSTOM_ISM330DHCX_0:
		if(ISM330DHCX_FIFO_Set_INT1_FIFO_Full(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
	case CUSTOM_LSM6DSO32X_0:
		if(LSM6DSO32X_FIFO_Set_INT1_FIFO_Full(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
	case CUSTOM_LSM6DSV16X_0:
		if(LSM6DSV16X_FIFO_Set_INT1_FIFO_Full(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

/**
 * @brief  Set FIFO full interrupt on INT2 pin
 * @param  Instance the device instance
 * @param  Status FIFO full interrupt on INT2 pin
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Set_INT2_FIFO_Full(uint32_t Instance, uint8_t Status)
{
	int32_t ret;

	switch(Instance)
	{
#if (USE_MOTION_SENSOR_IIS3DWB_0 == 1)
	case IIS3DWB_0:
		if(IIS3DWB_INT2_Set_FIFO_Full(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
	case CUSTOM_ISM330DHCX_0:
		if(ISM330DHCX_FIFO_Set_INT2_FIFO_Full(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif
#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
	case CUSTOM_LSM6DSO32X_0:
		if(LSM6DSO32X_FIFO_Set_INT2_FIFO_Full(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
	case CUSTOM_LSM6DSV16X_0:
		if(LSM6DSV16X_FIFO_Set_INT2_FIFO_Full(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

/**
 * @brief  Set FIFO watermark level
 * @param  Instance the device instance
 * @param  Watermark FIFO watermark level
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Set_Watermark_Level(uint32_t Instance, uint16_t Watermark)
{
	int32_t ret;

	switch(Instance)
	{
#if (USE_MOTION_SENSOR_IIS3DWB_0 == 1)
	case IIS3DWB_0:
		if(IIS3DWB_FIFO_Set_Watermark_Level(MotionCompObj[Instance], Watermark) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
	case CUSTOM_ISM330DHCX_0:
		if(ISM330DHCX_FIFO_Set_Watermark_Level(MotionCompObj[Instance], Watermark) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
	case CUSTOM_LSM6DSO32X_0:
		if(LSM6DSO32X_FIFO_Set_Watermark_Level(MotionCompObj[Instance], Watermark) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
	case CUSTOM_LSM6DSV16X_0:
		if(LSM6DSV16X_FIFO_Set_Watermark_Level(MotionCompObj[Instance], Watermark) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

/**
 * @brief  Set FIFO stop on watermark
 * @param  Instance the device instance
 * @param  Status FIFO stop on watermark status
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(uint32_t Instance, uint8_t Status)
{
	int32_t ret;

	switch(Instance)
	{
#if (USE_MOTION_SENSOR_IIS3DWB_0 == 1)
	case IIS3DWB_0:
		if(IIS3DWB_FIFO_Set_Stop_On_Fth(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
	case CUSTOM_ISM330DHCX_0:
		if(ISM330DHCX_FIFO_Set_Stop_On_Fth(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
	case CUSTOM_LSM6DSO32X_0:
		if(LSM6DSO32X_FIFO_Set_Stop_On_Fth(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
	case CUSTOM_LSM6DSV16X_0:
		if(LSM6DSV16X_FIFO_Set_Stop_On_Fth(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

/**
 * @brief  Set FIFO mode
 * @param  Instance the device instance
 * @param  Mode FIFO mode
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Set_Mode(uint32_t Instance, uint8_t Mode)
{
	int32_t ret;

	switch(Instance)
	{

#if (USE_MOTION_SENSOR_IIS3DWB_0 == 1)
	case IIS3DWB_0:
		if(IIS3DWB_FIFO_Set_Mode(MotionCompObj[Instance], (iis3dwb_fifo_mode_t) Mode) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
	case CUSTOM_ISM330DHCX_0:
		if(ISM330DHCX_FIFO_Set_Mode(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
	case CUSTOM_LSM6DSO32X_0:
		if(LSM6DSO32X_FIFO_Set_Mode(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
	case CUSTOM_LSM6DSV16X_0:
		if(LSM6DSV16X_FIFO_Set_Mode(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

/**
  * @brief  Get the LSM6DSV16X FIFO accelero single word (16-bit data)
  * @param  pObj the device pObj
  * @param  Acceleration FIFO single data
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_FIFO_Get_Data_Word(ISM6HG256X_Object_t *pObj, int16_t *data_raw)
{
  uint8_t data[6];

  if (ISM6HG256X_FIFO_Get_Data(pObj, data) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }

  data_raw[0] = ((int16_t)data[1] << 8) | data[0];
  data_raw[1] = ((int16_t)data[3] << 8) | data[2];
  data_raw[2] = ((int16_t)data[5] << 8) | data[4];

  return ISM6HG256X_OK;
}
#if 0
/**
 * @brief  Get FIFO single axis data RAW
 * @param  Instance the device instance
 * @param  Function Motion sensor function. Could be:
 *         - MOTION_GYRO or MOTION_ACCELERO
 * @param  Data FIFO single axis data
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Get_Data_Word(uint32_t Instance, uint32_t Function, int16_t *Data)
{
	int32_t ret = 0;

	switch(Instance)
	{
#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
	case CUSTOM_ISM330DHCX_0:
		if((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
		{
			if(ISM330DHCX_FIFO_Get_Data_Word(MotionCompObj[Instance], Data) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}
		}
		else if((Function & MOTION_GYRO) == MOTION_GYRO)
		{
			/*if(ISM330DHCX_FIFO_GYRO_Get_Axes(MotionCompObj[Instance], Data) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}*/
		}
		else
		{
			ret = BSP_ERROR_WRONG_PARAM;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
	case CUSTOM_LSM6DSO32X_0:
		if((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
		{
			if(LSM6DSO32X_FIFO_Get_Data_Word(MotionCompObj[Instance], Data) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}
		}
		else if((Function & MOTION_GYRO) == MOTION_GYRO)
		{
			/*if(LSM6DSO32X_FIFO_GYRO_Get_Axes(MotionCompObj[Instance], Data) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}*/
		}
		else
		{
			ret = BSP_ERROR_WRONG_PARAM;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
	case CUSTOM_LSM6DSV16X_0:
		if((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
		{
			if(LSM6DSV16X_FIFO_Get_Data_Word(MotionCompObj[Instance], Data) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}
		}
		else if((Function & MOTION_GYRO) == MOTION_GYRO)
		{
			/*if(LSM6DSV16X_FIFO_GYRO_Get_Axes(MotionCompObj[Instance], Data) != BSP_ERROR_NONE)
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				ret = BSP_ERROR_NONE;
			}*/
		}
		else
		{
			ret = BSP_ERROR_WRONG_PARAM;
		}
		break;
#endif

	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Overrun_Set_INT1(uint32_t Instance, uint8_t Status)
{
	int32_t ret;

	switch(Instance)
	{
#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
	case CUSTOM_ISM330DHCX_0:
		if(ISM330DHCX_FIFO_Overrun_Set_INT1(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
	case CUSTOM_LSM6DSO32X_0:
		if(LSM6DSO32X_FIFO_Overrun_Set_INT1(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
	case CUSTOM_LSM6DSV16X_0:
		if(LSM6DSV16X_FIFO_Overrun_Set_INT1(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Overrun_Set_INT2(uint32_t Instance, uint8_t Status)
{
	int32_t ret;

	switch(Instance)
	{
#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
	case CUSTOM_ISM330DHCX_0:
		if(ISM330DHCX_FIFO_Overrun_Set_INT2(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
	case CUSTOM_LSM6DSO32X_0:
		if(LSM6DSO32X_FIFO_Overrun_Set_INT2(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
	case CUSTOM_LSM6DSV16X_0:
		if(LSM6DSV16X_FIFO_Overrun_Set_INT2(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

/**
* @brief  Set the LSM6DSV16X FIFO overrun interrupt on INT1 pin
* @param  pObj the device pObj
* @param  Status FIFO overrun interrupt on INT1 pin status
* @retval 0 in case of success, an error code otherwise
*/
int32_t LSM6DSV16X_FIFO_Overrun_Set_INT1(LSM6DSV16X_Object_t *pObj, uint8_t Status)
{
  lsm6dsv16x_reg_t reg;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_INT1_CTRL, &reg.byte, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  reg.int1_ctrl.int1_fifo_ovr = Status;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_INT1_CTRL, &reg.byte, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
* @brief  Set the LSM6DSV16X FIFO overrun interrupt on INT2 pin
* @param  pObj the device pObj
* @param  Status FIFO overrun interrupt on INT2 pin status
* @retval 0 in case of success, an error code otherwise
*/
int32_t LSM6DSV16X_FIFO_Overrun_Set_INT2(LSM6DSV16X_Object_t *pObj, uint8_t Status)
{
  lsm6dsv16x_reg_t reg;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_INT2_CTRL, &reg.byte, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  reg.int2_ctrl.int2_fifo_ovr = Status;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_INT2_CTRL, &reg.byte, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Watermark_Set_INT1(uint32_t Instance, uint8_t Status)
{
	int32_t ret;

	switch(Instance)
	{
#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
	case CUSTOM_ISM330DHCX_0:
		if(ISM330DHCX_FIFO_Watermark_Set_INT1(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
	case CUSTOM_LSM6DSO32X_0:
		if(LSM6DSO32X_FIFO_Watermark_Set_INT1(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
	case CUSTOM_LSM6DSV16X_0:
		if(LSM6DSV16X_FIFO_Watermark_Set_INT1(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Watermark_Set_INT2(uint32_t Instance, uint8_t Status)
{
	int32_t ret;

	switch(Instance)
	{
#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
	case CUSTOM_ISM330DHCX_0:
		if(ISM330DHCX_FIFO_Watermark_Set_INT2(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
	case CUSTOM_LSM6DSO32X_0:
		if(LSM6DSO32X_FIFO_Watermark_Set_INT2(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
	case CUSTOM_LSM6DSV16X_0:
		if(LSM6DSV16X_FIFO_Watermark_Set_INT2(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}
		break;
#endif

	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

/**
* @brief  Set the LSM6DSV16X FIFO watermark interrupt on INT1 pin
* @param  pObj the device pObj
* @param  Status FIFO watermark interrupt on INT1 pin status
* @retval 0 in case of success, an error code otherwise
*/
int32_t LSM6DSV16X_FIFO_Watermark_Set_INT1(LSM6DSV16X_Object_t *pObj, uint8_t Status)
{
  lsm6dsv16x_reg_t reg;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_INT1_CTRL, &reg.byte, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  reg.int1_ctrl.int1_fifo_th = Status;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_INT1_CTRL, &reg.byte, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
* @brief  Set the LSM6DSV16X FIFO watermark interrupt on INT2 pin
* @param  pObj the device pObj
* @param  Status FIFO watermark interrupt on INT2 pin status
* @retval 0 in case of success, an error code otherwise
*/
int32_t LSM6DSV16X_FIFO_Watermark_Set_INT2(LSM6DSV16X_Object_t *pObj, uint8_t Status)
{
  lsm6dsv16x_reg_t reg;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_INT2_CTRL, &reg.byte, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  reg.int2_ctrl.int2_fifo_th = Status;

  if (lsm6dsv16x_write_reg(&(pObj->Ctx), LSM6DSV16X_INT2_CTRL, &reg.byte, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  return LSM6DSV16X_OK;
}

/**
 * @brief  Get FIFO all status
 * @param  Instance the device instance
 * @param  Status FIFO all status
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Get_All_Status(uint32_t Instance, MY_LSM6DSV16X_Fifo_Status_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case CUSTOM_ISM330DHCX_0:
      if (ISM330DHCX_FIFO_Get_All_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
    case CUSTOM_LSM6DSO32X_0:
      if (LSM6DSO32X_FIFO_Get_All_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case CUSTOM_LSM6DSV16X_0:
      if (LSM6DSV16X_FIFO_Get_All_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get the LSM6DSV16X FIFO all status
  * @param  pObj the device pObj
  * @param  Status FIFO register content
  * @retval 0 in case of success, an error code otherwise
  */
int32_t LSM6DSV16X_FIFO_Get_All_Status(LSM6DSV16X_Object_t *pObj, MY_LSM6DSV16X_Fifo_Status_t *Status)
{
  lsm6dsv16x_reg_t reg;

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FIFO_STATUS1, &reg.byte, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  if (lsm6dsv16x_read_reg(&(pObj->Ctx), LSM6DSV16X_FIFO_STATUS2, &reg.byte, 1) != LSM6DSV16X_OK)
  {
    return LSM6DSV16X_ERROR;
  }

  Status->FifoWatermark = reg.fifo_status2.fifo_wtm_ia;
  Status->FifoFull = reg.fifo_status2.fifo_full_ia;
  Status->FifoOverrun = reg.fifo_status2.fifo_ovr_ia;
  Status->CounterBdr = reg.fifo_status2.counter_bdr_ia;
  Status->FifoOverrunLatched = reg.fifo_status2.fifo_ovr_latched;

  return LSM6DSV16X_OK;
}
#endif
/**
  * @brief  Get the LSM6DSV16X MLC INT status page
  * @param  pObj the device pObj
  * @param  Status MLC status register content
  * @retval 0 in case of success, an error code otherwise
  */
int32_t ISM6HG256X_MLC_Get_Status(ISM6HG256X_Object_t *pObj, uint8_t *Status)
{

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_MLC_STATUS_MAINPAGE, Status, 1) != ISM6HG256X_OK)
  {
    return ISM6HG256X_ERROR;
  }
  return ISM6HG256X_OK;
}
#if 0
/**
 * @brief  Get number of unread FIFO samples
 * @param  Instance the device instance
 * @param  NumSamples number of unread FIFO samples
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_FIFO_Get_Num_Samples(uint32_t Instance, uint16_t *NumSamples)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case CUSTOM_ISM330DHCX_0:
      if (ISM330DHCX_FIFO_Get_Num_Samples(MotionCompObj[Instance], NumSamples) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
    case CUSTOM_LSM6DSO32X_0:
      if (LSM6DSO32X_FIFO_Get_Num_Samples(MotionCompObj[Instance], NumSamples) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case CUSTOM_LSM6DSV16X_0:
      if (LSM6DSV16X_FIFO_Get_Num_Samples(MotionCompObj[Instance], NumSamples) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}
#endif

int32_t ISM6HG256X_ACC_Enable_DRDY_On_INT1 (ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_reg_t reg;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_INT1_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  reg.int1_ctrl.int1_drdy_xl = 1;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_INT1_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

int32_t ISM6HG256X_ACC_Disable_DRDY_On_INT1 (ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_reg_t reg;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_INT1_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  reg.int1_ctrl.int1_drdy_xl = 0;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_INT1_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

int32_t ISM6HG256X_ACC_Enable_DRDY_On_INT2 (ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_reg_t reg;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_INT2_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  reg.int2_ctrl.int2_drdy_xl = 1;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_INT2_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

int32_t ISM6HG256X_ACC_Disable_DRDY_On_INT2 (ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_reg_t reg;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_INT2_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  reg.int2_ctrl.int2_drdy_xl = 0;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_INT2_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}


/**
 * @brief  Enable INT1 on accelerometer data ready event on INT 1 pin
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_ACC_Enable_DRDY_On_INT1 (uint32_t Instance)
{
  int32_t ret= BSP_ERROR_NONE;

  switch (Instance)
  {
    case CUSTOM_ISM6HG256X_0:
      if (ISM6HG256X_ACC_Enable_DRDY_On_INT1(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
	  break;
	  
    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }
  
  return ret;
}

/**
 * @brief  Disable INT1 on accelerometer data ready event on INT 1 pin
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_ACC_Disable_DRDY_On_INT1 (uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
    case CUSTOM_ISM6HG256X_0:
      if (ISM6HG256X_ACC_Disable_DRDY_On_INT1(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
	  break;
	  
    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }
  
  return ret;
}


/**
 * @brief  Enable INT2 on accelerometer data ready event on INT 2 pin
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_ACC_Enable_DRDY_On_INT2 (uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
    case CUSTOM_ISM6HG256X_0:
      if (ISM6HG256X_ACC_Enable_DRDY_On_INT2(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
	  break;
	  
    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }
  
  return ret;
}

/**
 * @brief  Disable INT2 on accelerometer data ready event on INT 2 pin
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_ACC_Disable_DRDY_On_INT2 (uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
    case CUSTOM_ISM6HG256X_0:
      if (MY_ISM6HG256X_ACC_Disable_DRDY_On_INT2(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
	  break;
	  
    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }
  
  return ret;
}


int32_t MY_ISM6HG256X_ACC_GetOutputDataRate(ISM6HG256X_Object_t *pObj, float *Odr)
{
  if (pObj->acc_is_enabled == 1U)
  {
    return ISM6HG256X_ACC_GetOutputDataRate(pObj, Odr);
  } else {
				  
	*Odr = (pObj->acc_odr == ISM6HG256X_ODR_AT_1Hz875 ) ? 1.875f
         : (pObj->acc_odr == ISM6HG256X_ODR_AT_7Hz5 ) ? 7.5f
         : (pObj->acc_odr == ISM6HG256X_ODR_AT_15Hz ) ? 15.0f
         : (pObj->acc_odr == ISM6HG256X_ODR_AT_30Hz ) ? 30.0f
         : (pObj->acc_odr == ISM6HG256X_ODR_AT_60Hz ) ? 60.0f
         : (pObj->acc_odr == ISM6HG256X_ODR_AT_120Hz ) ? 120.0f
         : (pObj->acc_odr == ISM6HG256X_ODR_AT_240Hz ) ? 240.0f
         : (pObj->acc_odr == ISM6HG256X_ODR_AT_480Hz ) ? 480.0f
         : (pObj->acc_odr == ISM6HG256X_ODR_AT_960Hz ) ? 960.0f
         : (pObj->acc_odr == ISM6HG256X_ODR_AT_1920Hz ) ? 1920.0f
         : (pObj->acc_odr == ISM6HG256X_ODR_AT_3840Hz ) ? 3840.0f
         :  7680.0f;
  return ISM6HG256X_OK;
  }
}


int32_t MY_ISM6HG256X_GYRO_GetOutputDataRate(ISM6HG256X_Object_t *pObj, float *Odr)
{
  if (pObj->gyro_is_enabled == 1U)
  {
    return ISM6HG256X_GYRO_GetOutputDataRate(pObj, Odr);
  } else {
				  
	*Odr = (pObj->gyro_odr == ISM6HG256X_ODR_AT_1Hz875 ) ? 1.875f
         : (pObj->gyro_odr == ISM6HG256X_ODR_AT_7Hz5 ) ? 7.5f
         : (pObj->gyro_odr == ISM6HG256X_ODR_AT_15Hz ) ? 15.0f
         : (pObj->gyro_odr == ISM6HG256X_ODR_AT_30Hz ) ? 30.0f
         : (pObj->gyro_odr == ISM6HG256X_ODR_AT_60Hz ) ? 60.0f
         : (pObj->gyro_odr == ISM6HG256X_ODR_AT_120Hz ) ? 120.0f
         : (pObj->gyro_odr == ISM6HG256X_ODR_AT_240Hz ) ? 240.0f
         : (pObj->gyro_odr == ISM6HG256X_ODR_AT_480Hz ) ? 480.0f
         : (pObj->gyro_odr == ISM6HG256X_ODR_AT_960Hz ) ? 960.0f
         : (pObj->gyro_odr == ISM6HG256X_ODR_AT_1920Hz ) ? 1920.0f
         : (pObj->gyro_odr == ISM6HG256X_ODR_AT_3840Hz ) ? 3840.0f
         :  7680.0f;
  return ISM6HG256X_OK;
  }
}

int32_t MY_CUSTOM_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr)
{
  switch (Instance)
  {
    case CUSTOM_ISM6HG256X_0:
		if (Function == MOTION_ACCELERO)
		{
	   		if (MY_ISM6HG256X_ACC_GetOutputDataRate(MotionCompObj[Instance], Odr) != BSP_ERROR_NONE) return BSP_ERROR_COMPONENT_FAILURE;
	   	   		return BSP_ERROR_NONE;
	  	} else if (Function == MOTION_GYRO)
	  	{
	    	if (MY_ISM6HG256X_GYRO_GetOutputDataRate(MotionCompObj[Instance], Odr) != BSP_ERROR_NONE) return BSP_ERROR_COMPONENT_FAILURE;
				return BSP_ERROR_NONE;
	  	} 
		break;  

	default:
		return BSP_ERROR_WRONG_PARAM;
	 	break;
  }
  return BSP_ERROR_NONE;
}


int32_t ISM6HG256X_GYRO_Enable_DRDY_On_INT1 (ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_reg_t reg;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_INT1_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  reg.int1_ctrl.int1_drdy_g = 1;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_INT1_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

int32_t ISM6HG256X_GYRO_Disable_DRDY_On_INT1 (ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_reg_t reg;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_INT1_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  reg.int1_ctrl.int1_drdy_g = 0;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_INT1_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

int32_t ISM6HG256X_GYRO_Enable_DRDY_On_INT2 (ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_reg_t reg;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_INT2_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  reg.int2_ctrl.int2_drdy_g = 1;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_INT2_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}

int32_t ISM6HG256X_GYRO_Disable_DRDY_On_INT2 (ISM6HG256X_Object_t *pObj)
{
  ism6hg256x_reg_t reg;

  if (ism6hg256x_read_reg(&(pObj->Ctx), ISM6HG256X_INT2_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  reg.int2_ctrl.int2_drdy_g = 0;

  if (ism6hg256x_write_reg(&(pObj->Ctx), ISM6HG256X_INT2_CTRL, &reg.byte, 1) != ISM6HG256X_OK)
  {
	return ISM6HG256X_ERROR;
  }

  return ISM6HG256X_OK;
}


/**
 * @brief  Set INT2 on gyro data ready
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_GYRO_Enable_DRDY_On_INT2 (uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
    case CUSTOM_ISM6HG256X_0:
      if (ISM6HG256X_GYRO_Enable_DRDY_On_INT2(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
	  break;
	  
    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }
  
  return ret;
}

/**
 * @brief  Disable INT1 on gyro data ready event
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_GYRO_Disable_DRDY_On_INT2 (uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
    case CUSTOM_ISM6HG256X_0:
      if (ISM6HG256X_GYRO_Disable_DRDY_On_INT2(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
	  break;
	  
    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }
  
  return ret;
}

/**
 * @brief  Enable INT1 on accelerometer data ready event on INT 1 pin
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_GYRO_Enable_DRDY_On_INT1 (uint32_t Instance)
{
  int32_t ret= BSP_ERROR_NONE;

  switch (Instance)
  {
    case CUSTOM_ISM6HG256X_0:
      if (ISM6HG256X_GYRO_Enable_DRDY_On_INT1(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
	  break;
	  
    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }
  
  return ret;
}

/**
 * @brief  Disable INT1 on accelerometer data ready event on INT 1 pin
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_GYRO_Disable_DRDY_On_INT1 (uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
    case CUSTOM_ISM6HG256X_0:
      if (ISM6HG256X_GYRO_Disable_DRDY_On_INT1(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
	  break;
	  
    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }
  
  return ret;
}


/**
 * @brief  Get MLC INT status mainpage registr
 * @param  Instance the device instance
 * @param  NumSamples number of unread FIFO samples
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_MLC_Get_Status_Mainpage(uint32_t Instance, uint8_t *status)
{
  int32_t ret;

  switch (Instance)
  {

#if (USE_CUSTOM_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case CUSTOM_LSM6DSV16X_0:
      if (LSM6DSV16X_MLC_Get_Status(MotionCompObj[Instance], status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

int32_t ism6hg256x_sw_por(const stmdev_ctx_t *ctx)
{
  ism6hg256x_func_cfg_access_t func_cfg_access = {0};
  int32_t ret;

  if (ctx->mdelay == NULL)
  {
    ret = -1;
    goto exit;
  }

  /* 1. Set the SW_POR bit of the FUNC_CFG_ACCESS register to 1. */
  func_cfg_access.sw_por = 1;
  ret = ism6hg256x_write_reg(ctx, ISM6HG256X_FUNC_CFG_ACCESS, (uint8_t *)&func_cfg_access, 1);
  if (ret != 0)
  {
    goto exit;
  }

  /* 2. Wait 50 ms. */
  ctx->mdelay(50);

exit:
  return ret;
}

/**
 * @brief  Resets the ISM6HG256X ctrl registers to their default values
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t MY_CUSTOM_MOTION_SENSOR_Reset (uint32_t Instance)
{
  int32_t ret= BSP_ERROR_NONE;

  switch (Instance)
  {
    case CUSTOM_ISM6HG256X_0:
	  if (ism6hg256x_sw_por(&(((ISM6HG256X_Object_t *)MotionCompObj[Instance])->Ctx)) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
	  break;
	  
    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }  
  return ret;
}
