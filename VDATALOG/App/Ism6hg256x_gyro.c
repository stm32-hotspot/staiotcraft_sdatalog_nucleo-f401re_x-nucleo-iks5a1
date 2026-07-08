/**
  ******************************************************************************
  * @file           : ism6hg256x_gyro.c
  * @brief          : Application code for ism6hg256x_gyro
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
#include "main.h"
#include "Ism6hg256x_reg.h"
#include "App_model_Ism6hg256x_Gyro.h"
#include "App_model_Ism6hg256x_Mlc.h"
#include "Ism6hg256x_gyro.h"
#include "Ism6hg256x_acc.h"
#include "custom_motion_sensors_ex.h"
#include "simple_serial_tl.h"
#include "vdatalog_conf.h"
#include "vdatalog.h"
#include "timestamp.h"
#include "fsm.h"
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VD_BUFFER_SIZE_GYRO   (ASPEP_HEADER_SIZE + SSTL_HEADER_SIZE + TIMESTAMP_SIZE + SAMPLES_PER_TIME_STAMP_GYRO*BYTES_PER_SAMPLE_GYRO)
extern void *MotionCompObj[CUSTOM_MOTION_INSTANCES_NBR];
extern SimpleSerialTL_t *pSstl;
extern volatile double GYROTimeStamp;
extern uint8_t ism6hg256x_gyro_get_output_st_stream__channel_specification_stream_id(int32_t *value);
extern uint32_t led10_timer;
void GyroStartLoggingODRtrgdMode(pnpl_ism6hg256x_gyro_odr_t_t enum_odr);

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t txbuffer_s2[VD_BUFFER_SIZE_GYRO] __attribute__((aligned(4)));
/* allocate the FSM obj */
FSM GyroFsm;

/** Define the FSM state transitions LUT
     current FSM state,             Input event,        next FSM state,            FSM state action func **/
Transition GyroTransitions[] = {
    {FSM_STATE_DISABLED,       FSM_EVT_ENABLE,      FSM_STATE_ENABLED,         GyroDisabledStateActionEnable},
    {FSM_STATE_DISABLED,       FSM_EVT_ENABLE_ODR_TRGD, FSM_STATE_ENABLED_ODR_TRGD, GyroDisabledStateActionEnableOdrTrgd},

    {FSM_STATE_ENABLED,        FSM_EVT_DISABLE,     FSM_STATE_DISABLED,        GyroEnabledStateActionDisable},
    {FSM_STATE_ENABLED,        FSM_EVT_START_LOG,   FSM_STATE_LOGGING_DATA,    GyroEnabledStateActionStartLog},    

    {FSM_STATE_ENABLED_ODR_TRGD, FSM_EVT_START_LOG, FSM_STATE_LOGGING_DATA_ODR_TRGD, GyroEnabledOdrTrgdStateActionStartLog},
    {FSM_STATE_ENABLED_ODR_TRGD, FSM_EVT_DISABLE,   FSM_STATE_DISABLED,             GyroEnabledOdrTrgdStateActionDisable},

    {FSM_STATE_LOGGING_DATA,   FSM_EVT_INT,         FSM_STATE_LOGGING_DATA,    GyroLoggingDataStateActionInt},
    {FSM_STATE_LOGGING_DATA,   FSM_EVT_STOP_LOG,    FSM_STATE_ENABLED,         GyroLoggingDataStateActionStopLog},

    {FSM_STATE_LOGGING_DATA_ODR_TRGD, FSM_EVT_INT,      FSM_STATE_LOGGING_DATA_ODR_TRGD, GyroLoggingDataStateActionInt},
    {FSM_STATE_LOGGING_DATA_ODR_TRGD, FSM_EVT_STOP_LOG, FSM_STATE_ENABLED_ODR_TRGD,      GyroLoggingDataOdrTrgdStateActionStopLog},
};

unsigned int sizeof_Gyro_transitions = sizeof(GyroTransitions) / sizeof(Transition);

#if (VD_DUMMY_DATA_QVAR == 1)
static int16_t gyro_dummy_counter = 0;
#endif

/* Private function prototypes -----------------------------------------------*/

void ReadGyro()
{
    uint8_t Status=0;
    ISM6HG256X_AxesRaw_t Value;

    /* if gyro is logging it needs to check if the INT2 is from acc or gyro data ready */
    if (FsmCheckState(&AccFsm, FSM_STATE_LOGGING_DATA)) {
      ISM6HG256X_GYRO_Get_DRDY_Status((ISM6HG256X_Object_t *)MotionCompObj[SENSOR_0], &Status);
      if (Status == 0) return; /* if gyro data is not ready, it means is an acc data INT: return and wait for next INT */
    }
   
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);      
    /* Get time stamp*/
    GYROTimeStamp = TS_VD_GetTimeStamp();  

    /* read the Gyro axes */
    ISM6HG256X_GYRO_GetAxesRaw((ISM6HG256X_Object_t *)MotionCompObj[SENSOR_0], &Value);

#ifndef NO_PRINTF_USB
    printf("G: %d %d %d\r\n", Value.x, Value.y, Value.z);
#endif
    /* prepare the ptr to msg data field */
    ISM6HG256X_AxesRaw_t *p_tx_datard_sensorGYRO = (ISM6HG256X_AxesRaw_t *)&txbuffer_s2[ASPEP_HEADER_SIZE + SSTL_HEADER_SIZE + TIMESTAMP_SIZE];    

    /* fill msg data field */
    memcpy (p_tx_datard_sensorGYRO, &Value, sizeof(ISM6HG256X_AxesRaw_t));

    /* because of alignement constraint timestamp must be copied byte by byte */
    /*  fill the msg time stamp data field */
    for (size_t i = 0; i < sizeof(double); i++) {
        txbuffer_s2[ASPEP_HEADER_SIZE + SSTL_HEADER_SIZE  + i] = ((uint8_t *)&GYROTimeStamp)[i];
    }
    /* send "Datalog Payload" (ByteCounter(4) | QvarData(2) | TimeStamp(8)) to GUI */
    int32_t id;  
    ism6hg256x_gyro_get_output_st_stream__channel_specification_stream_id(&id); 

    if (SSTL_TxAsync(pSstl, (uint8_t)id, SSTL_ASYNC_PAYLOAD_TYPE_CUSTOM, &txbuffer_s2[ASPEP_HEADER_SIZE + SSTL_HEADER_SIZE], TIMESTAMP_SIZE + SAMPLES_PER_TIME_STAMP_GYRO*BYTES_PER_SAMPLE_GYRO, SSTL_NON_BLOCKING) != SSTL_OK )
    {
        return;
    }
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
}

/* FSM State Action Functions */
void GyroDisabledStateActionEnable(Event event, void* args)
{
    (void)event;
    (void)args;
    CUSTOM_MOTION_SENSOR_Enable(SENSOR_0, MOTION_GYRO);
}

void GyroDisabledStateActionEnableOdrTrgd(Event event, void* args)
{
    (void)event;
    (void)args;
}

void GyroEnabledStateActionDisable(Event event, void* args)
{
  (void)event;
  (void)args;
  bool value;
  ism6hg256x_mlc_get_enable(&value);
  if(!value) {
    /* if MLC is NOT enabled, set Gyro EN, ODR, FS to their default value */      
    CUSTOM_MOTION_SENSOR_Disable(SENSOR_0, MOTION_GYRO);
    ism6hg256x_gyro_set_odr(DEFAULT_GYRO_ODR_HZ);
    ism6hg256x_gyro_set_fs(DEFAULT_GYRO_FS_DPS);
  } /* if MLC is enabled, keep the ODR and FS as they are because they can be shared between Gyro and MLC */
}

void GyroEnabledOdrTrgdStateActionDisable(Event event, void* args)
{
    (void)event;
    (void)args;
    MY_CUSTOM_MOTION_SENSOR_Reset (SENSOR_0);
    CUSTOM_MOTION_SENSOR_Disable(SENSOR_0, MOTION_GYRO);
    MY_CUSTOM_MOTION_SENSOR_GYRO_Disable_DRDY_On_INT2(SENSOR_0);
    ism6hg256x_gyro_set_odr_t(pnpl_ism6hg256x_gyro_odr_t_hz400);
    ism6hg256x_gyro_set_fs(DEFAULT_GYRO_FS_DPS);
    ISM6HG256X_GYRO_SetOutputDataRate_With_Mode((ISM6HG256X_Object_t *)MotionCompObj[SENSOR_0], DEFAULT_GYRO_ODR_HZ_FLOAT, ISM6HG256X_GYRO_HIGH_PERFORMANCE_MODE);
}

void GyroEnabledStateActionStartLog(Event event, void* args)
{
    (void)event;
    (void)args;
    led10_timer = LED10_FAST_TIMER; /* change blue led timer to fast */

    MY_CUSTOM_MOTION_SENSOR_GYRO_Enable_DRDY_On_INT2(SENSOR_0);

    /* first dummy read to unlatch INT */
    ISM6HG256X_AxesRaw_t Value;  
    ISM6HG256X_GYRO_GetAxesRaw((ISM6HG256X_Object_t *)MotionCompObj[SENSOR_0], &Value);
}

void GyroEnabledOdrTrgdStateActionStartLog(Event event, void* args)
{
    (void)event;
    (void)args;

    led10_timer = LED10_FAST_TIMER;
    GyroStartLoggingODRtrgdMode(*(pnpl_ism6hg256x_gyro_odr_t_t*)args);
}

void GyroLoggingDataStateActionInt(Event event, void* args)
{
    (void)event;
    (void)args;
    ReadGyro();
}

void GyroLoggingDataStateActionStopLog(Event event, void* args)
{
    (void)event;
    (void)args;
    led10_timer = LED10_SLOW_TIMER; /* change blue led blink timer to slow */
    MY_CUSTOM_MOTION_SENSOR_GYRO_Disable_DRDY_On_INT2(SENSOR_0);
}

void GyroLoggingDataOdrTrgdStateActionStopLog(Event event, void* args)
{
    (void)event;
    (void)args;

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    led10_timer = LED10_SLOW_TIMER;
    MY_CUSTOM_MOTION_SENSOR_GYRO_Disable_DRDY_On_INT1(SENSOR_0);
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);

    GPIO_InitStruct.Pin = A_G_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(A_G_INT2_EXTI_IRQn);
}

void GyroStartLoggingODRtrgdMode(pnpl_ism6hg256x_gyro_odr_t_t enum_odr)
{
    uint32_t tim_pwm_period_us = 0;
    uint8_t nsamples_per_trg = 0;
    uint8_t odr = 0;

    switch (enum_odr)
    {
        case pnpl_ism6hg256x_gyro_odr_t_hz12_5:
            tim_pwm_period_us = 640000;
            nsamples_per_trg = 4;
            odr = ISM6HG256X_ODR_AT_15Hz;
            break;
        case pnpl_ism6hg256x_gyro_odr_t_hz25:
            tim_pwm_period_us = 320000;
            nsamples_per_trg = 4;
            odr = ISM6HG256X_ODR_AT_30Hz;
            break;
        case pnpl_ism6hg256x_gyro_odr_t_hz50:
            tim_pwm_period_us = 160000;
            nsamples_per_trg = 4;
            odr = ISM6HG256X_ODR_AT_60Hz;
            break;
        case pnpl_ism6hg256x_gyro_odr_t_hz100:
            tim_pwm_period_us = 80000;
            nsamples_per_trg = 4;
            odr = ISM6HG256X_ODR_AT_120Hz;
            break;
        case pnpl_ism6hg256x_gyro_odr_t_hz200:
            tim_pwm_period_us = 40000;
            nsamples_per_trg = 4;
            odr = ISM6HG256X_ODR_AT_240Hz;
            break;
        case pnpl_ism6hg256x_gyro_odr_t_hz400:
            tim_pwm_period_us = 40000;
            nsamples_per_trg = 8;
            odr = ISM6HG256X_ODR_AT_480Hz;
            break;
        case pnpl_ism6hg256x_gyro_odr_t_hz800:
            tim_pwm_period_us = 40000;
            nsamples_per_trg = 16;
            odr = ISM6HG256X_ODR_AT_960Hz;
            break;
        case pnpl_ism6hg256x_gyro_odr_t_hz1600:
            tim_pwm_period_us = 40000;
            nsamples_per_trg = 32;
            odr = ISM6HG256X_ODR_AT_1920Hz;
            break;
        case pnpl_ism6hg256x_gyro_odr_t_hz3200:
            tim_pwm_period_us = 40000;
            nsamples_per_trg = 64;
            odr = ISM6HG256X_ODR_AT_3840Hz;
            break;
        default:
            return;
    }

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = A_G_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(A_G_INT2_GPIO_Port, &GPIO_InitStruct);

    TIM2_PWM_Period_Init(tim_pwm_period_us);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

/* move sample ready INT from INT2 to INT1 */
    MY_CUSTOM_MOTION_SENSOR_GYRO_Disable_DRDY_On_INT2 (SENSOR_0);
    MY_CUSTOM_MOTION_SENSOR_GYRO_Enable_DRDY_On_INT1(SENSOR_0);

    CUSTOM_MOTION_SENSOR_Disable(SENSOR_0, MOTION_GYRO);

    CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_ODR_TRIG_CFG, nsamples_per_trg);

    CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_CTRL2, 0x30);

    uint8_t reg4_value;
    CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_CTRL4, &reg4_value);        
    reg4_value |= 0x01; 
    CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_CTRL4, reg4_value);

    CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_CTRL2, (0x30 | odr));

//    reg4_value |= 0x02;  // set INT pulsed
//    CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_CTRL4, reg4_value);

    HAL_NVIC_EnableIRQ(A_G_INT1_EXTI_IRQn);
    HAL_NVIC_DisableIRQ(A_G_INT2_EXTI_IRQn);

/* give time 4 * 80mSec to converge discarding first samples  */    

}