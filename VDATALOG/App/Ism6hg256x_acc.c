/**
  ******************************************************************************
  * @file           : ism6hg256x_acc.c
  * @brief          : Application code for ism6hg256x_acc
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
#include "App_model_Ism6hg256x_Acc.h"
#include "App_model_Ism6hg256x_Mlc.h"
#include "custom_motion_sensors_ex.h"
#include "Ism6hg256x_acc.h"
#include "Ism6hg256x_gyro.h"
#include "simple_serial_tl.h"
#include "vdatalog_conf.h"
#include "vdatalog.h"
#include "timestamp.h"
#include "fsm.h"
#include <stdint.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VD_BUFFER_SIZE_ACC   (ASPEP_HEADER_SIZE + SSTL_HEADER_SIZE + TIMESTAMP_SIZE + SAMPLES_PER_TIME_STAMP_ACC*BYTES_PER_SAMPLE_ACC)
extern void *MotionCompObj[CUSTOM_MOTION_INSTANCES_NBR];
extern SimpleSerialTL_t *pSstl;
extern volatile double ACCTimeStamp;
extern uint8_t ism6hg256x_acc_get_output_st_stream__channel_specification_stream_id(int32_t *value);
void AccStartLoggingODRtrgdMode(pnpl_ism6hg256x_acc_odr_t_t enum_odr);
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t txbuffer_s1[VD_BUFFER_SIZE_ACC] __attribute__((aligned(4)));
/* allocate the FSM obj */
FSM AccFsm;

/** Define the FSM state transitions LUT
     current FSM state,             Input event,           next FSM state,                 FSM state action func **/
Transition AccTransitions[] = {
    {FSM_STATE_DISABLED,         FSM_EVT_ENABLE,             FSM_STATE_ENABLED,            AccDisabledStateActionEnable},    
    {FSM_STATE_DISABLED,         FSM_EVT_ENABLE_ODR_TRGD,    FSM_STATE_ENABLED_ODR_TRGD,   AccDisabledStateActionEnableOdrTrgd},        

    {FSM_STATE_ENABLED,          FSM_EVT_START_LOG,          FSM_STATE_LOGGING_DATA,       AccEnabledStateActionLogRun},    
    {FSM_STATE_ENABLED,          FSM_EVT_DISABLE,            FSM_STATE_DISABLED,           AccEnabledStateActionDisable},
    
    {FSM_STATE_ENABLED_ODR_TRGD, FSM_EVT_START_LOG,          FSM_STATE_LOGGING_DATA_ODR_TRGD, AccEnabledOdrTrgdStateActionLogRun},    
    {FSM_STATE_ENABLED_ODR_TRGD, FSM_EVT_DISABLE,            FSM_STATE_DISABLED,           AccEnabledOdrTrgdStateActionDisable},    

    {FSM_STATE_LOGGING_DATA,     FSM_EVT_INT,                FSM_STATE_LOGGING_DATA,       AccLoggingDataStateActionInt},
    {FSM_STATE_LOGGING_DATA,     FSM_EVT_STOP_LOG,           FSM_STATE_ENABLED,            AccLoggingDataStateActionLogStop},

    {FSM_STATE_LOGGING_DATA_ODR_TRGD,     FSM_EVT_INT,       FSM_STATE_LOGGING_DATA_ODR_TRGD, AccLoggingDataStateActionInt},
    {FSM_STATE_LOGGING_DATA_ODR_TRGD,     FSM_EVT_STOP_LOG,  FSM_STATE_ENABLED_ODR_TRGD,      AccLoggingDataOdrTrgdStateActionLogStop},   
};
unsigned int sizeof_Acc_transitions = sizeof(AccTransitions) / sizeof(Transition);

/* Private function prototypes -----------------------------------------------*/

void ReadAcc()
{
  uint8_t Status=0;
  ISM6HG256X_AxesRaw_t Value;
  
    /* if gyro is logging it needs to check if the INT2 is from acc or gyro data ready */
    if (FsmCheckState(&GyroFsm, FSM_STATE_LOGGING_DATA)) {
      ISM6HG256X_ACC_Get_DRDY_Status((ISM6HG256X_Object_t *)MotionCompObj[SENSOR_0], &Status);
      if (Status == 0) return; /* if acc data is not ready, it means is a gyro data INT: return and wait for next INT */
    }

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);      
    /* get time stamp */ 
    ACCTimeStamp = TS_VD_GetTimeStamp();

    /* read the ACC axes */
    ISM6HG256X_ACC_GetAxesRaw((ISM6HG256X_Object_t *)MotionCompObj[SENSOR_0], &Value);
#ifndef NO_PRINTF_USB
      printf("A: %d %d %d\r\n", Value.x, Value.y, Value.z);
#endif
    /* prepare the ptr to msg data field */
    ISM6HG256X_AxesRaw_t *p_tx_datard_sensorACC = (ISM6HG256X_AxesRaw_t *)&txbuffer_s1[ASPEP_HEADER_SIZE + SSTL_HEADER_SIZE + TIMESTAMP_SIZE];    

    /* fill msg data field */
    memcpy (p_tx_datard_sensorACC, &Value, sizeof(ISM6HG256X_AxesRaw_t));
    /* this works only in case of SAMPLES_PER_TIME_STAMP_ACC = 1 otherwise data needs to be accumulated */
    /* because of alignement constraint timestamp must be copied byte by byte */
    /*  fill the msg time stamp data field */
    for (size_t i = 0; i < sizeof(double); i++) {
      txbuffer_s1[ASPEP_HEADER_SIZE + SSTL_HEADER_SIZE + i] = ((uint8_t *)&ACCTimeStamp)[i];
    }
    /* send "Datalog Payload" (TimeStamp(8) | AccData(2*3) | ) to GUI */
        
    int32_t id;
    ism6hg256x_acc_get_output_st_stream__channel_specification_stream_id(&id);

    if (SSTL_TxAsync(pSstl, (uint8_t)id, SSTL_ASYNC_PAYLOAD_TYPE_CUSTOM, &txbuffer_s1[ASPEP_HEADER_SIZE + SSTL_HEADER_SIZE], TIMESTAMP_SIZE + SAMPLES_PER_TIME_STAMP_ACC*BYTES_PER_SAMPLE_ACC, SSTL_NON_BLOCKING) != SSTL_OK )
    {
      return;
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
#ifndef NO_PRINTF_USB  
    // printf("ACC Data: X=%d, Y=%d, Z=%d", Value.x, Value.y, Value.z);
    // printf("\r\n");
#endif
}

extern uint32_t led10_timer;

void AccDisabledStateActionEnable(Event event, void* args)
{
  (void)event;
  (void)args;
  CUSTOM_MOTION_SENSOR_Enable(SENSOR_0, MOTION_ACCELERO);    
}

void AccEnabledStateActionDisable(Event event, void* args)
{
  (void)event;
  (void)args;
  bool value;
  ism6hg256x_mlc_get_enable(&value);
  if (!value) {
    /* if MLC is NOT enabled, set ODR and FS to their default value */    
    CUSTOM_MOTION_SENSOR_Disable(SENSOR_0, MOTION_ACCELERO);
    ism6hg256x_acc_set_odr(DEFAULT_ACC_ODR_HZ);
    ism6hg256x_acc_set_fs(DEFAULT_ACC_FS_G);
  } /* if MLC is enabled, keep the ODR and FS as they are because they can be shared between Acc and MLC */
}

void AccDisabledStateActionEnableOdrTrgd(Event event, void* args)
{
    (void)event;
    (void)args;
}

void AccEnabledOdrTrgdStateActionDisable(Event event, void* args)
{
    (void)event;
    (void)args;
    /* set Acc EN, ODR, FS to dft */
    MY_CUSTOM_MOTION_SENSOR_Reset (SENSOR_0);    
    CUSTOM_MOTION_SENSOR_Disable(SENSOR_0, MOTION_ACCELERO);
    MY_CUSTOM_MOTION_SENSOR_ACC_Disable_DRDY_On_INT1 (SENSOR_0);  
    /* set "normal" ODR, FS, and Mode back to default */
    ism6hg256x_acc_set_odr(DEFAULT_ACC_ODR_TRGD_HZ);
    ism6hg256x_acc_set_fs(DEFAULT_ACC_FS_G);
    ISM6HG256X_ACC_SetOutputDataRate_With_Mode((ISM6HG256X_Object_t *)MotionCompObj[SENSOR_0], DEFAULT_ACC_ODR_HZ_FLOAT,ISM6HG256X_ACC_HIGH_PERFORMANCE_MODE);    
}

/* The activation function from enabled state */


void AccEnabledOdrTrgdStateActionLogRun(Event event, void* args)
{
    (void)event;
    (void)args;

    led10_timer = LED10_FAST_TIMER; /* change blue led timer to fast */    
    AccStartLoggingODRtrgdMode(*(pnpl_ism6hg256x_acc_odr_t_t*)args);
}

void AccLoggingDataOdrTrgdStateActionLogStop(Event event, void* args) 
{
    (void)event;
    (void)args;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

      /* slow the LED timer */
    led10_timer = LED10_SLOW_TIMER; /* change blue led blink timer to slow */
    /* stop INT1 generation from sensor */
    MY_CUSTOM_MOTION_SENSOR_ACC_Disable_DRDY_On_INT1(SENSOR_0);
    /* stop PWM signal */
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);

    /*ReConfigure GPIO pins : A_G_INT2_Pin as EXTI */
    GPIO_InitStruct.Pin = A_G_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* EXTI interrupt init */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(A_G_INT2_EXTI_IRQn);
}

void AccEnabledStateActionLogRun(Event event, void* args)
{
    (void)event;
    (void)args;

    led10_timer = LED10_FAST_TIMER; /* change blue led timer to fast */

//    MY_CUSTOM_MOTION_SENSOR_Reset (SENSOR_0);    
    MY_CUSTOM_MOTION_SENSOR_ACC_Enable_DRDY_On_INT2(SENSOR_0);
//    ISM6HG256X_ACC_SetOutputDataRate_With_Mode((ISM6HG256X_Object_t *)MotionCompObj[SENSOR_0], DEFAULT_ACC_ODR_HZ_FLOAT,ISM6HG256X_ACC_HIGH_PERFORMANCE_MODE);
    /* first dummy read to unlatch INT */
    ISM6HG256X_AxesRaw_t Value;  
    ISM6HG256X_ACC_GetAxesRaw((ISM6HG256X_Object_t *)MotionCompObj[SENSOR_0], &Value);
}

void AccLoggingDataStateActionInt(Event event, void* args)
{
    (void)event;
    (void)args;
    /* read Accelero data */
    ReadAcc();
}

void AccLoggingDataStateActionLogStop(Event event, void* args)
{
    (void)event;
    (void)args;

    /* slow the LED timer */
    led10_timer = LED10_SLOW_TIMER; /* change blue led blink timer to slow */
    MY_CUSTOM_MOTION_SENSOR_ACC_Disable_DRDY_On_INT2(SENSOR_0);
}


void AccStartLoggingODRtrgdMode(pnpl_ism6hg256x_acc_odr_t_t enum_odr)
{
  uint32_t  tim_pwm_period_us =0;
  uint8_t  nsamples_per_trg =0;
  uint8_t  odr =0;

  switch (enum_odr)
  {
     case pnpl_ism6hg256x_acc_odr_t_hz12_5:
        tim_pwm_period_us = 640000;
        nsamples_per_trg = 4;
        odr = ISM6HG256X_ODR_AT_15Hz;  /* the closest available ODR (allowed +-33% to nominal) */
        break;
     case pnpl_ism6hg256x_acc_odr_t_hz25:
        tim_pwm_period_us = 320000;
        nsamples_per_trg = 4;
        odr = ISM6HG256X_ODR_AT_30Hz;
       break;
     case pnpl_ism6hg256x_acc_odr_t_hz50:
        tim_pwm_period_us = 160000;
        nsamples_per_trg = 4;
        odr = ISM6HG256X_ODR_AT_60Hz;        
       break;
     case pnpl_ism6hg256x_acc_odr_t_hz100:
        tim_pwm_period_us = 80000;
        nsamples_per_trg = 4;
        odr = ISM6HG256X_ODR_AT_120Hz;        
       break;
     case pnpl_ism6hg256x_acc_odr_t_hz200:
        tim_pwm_period_us = 40000;
        nsamples_per_trg = 4;
        odr = ISM6HG256X_ODR_AT_240Hz;        
       break;
     case pnpl_ism6hg256x_acc_odr_t_hz400:
        tim_pwm_period_us = 40000;
        nsamples_per_trg = 8;
        odr = ISM6HG256X_ODR_AT_480Hz;        
       break;
     case pnpl_ism6hg256x_acc_odr_t_hz800:
        tim_pwm_period_us = 40000;
        nsamples_per_trg = 16;
        odr = ISM6HG256X_ODR_AT_960Hz;        
       break;
     case pnpl_ism6hg256x_acc_odr_t_hz1600:
        tim_pwm_period_us = 40000;
        nsamples_per_trg = 32;
        odr = ISM6HG256X_ODR_AT_1920Hz;        
       break;
     case pnpl_ism6hg256x_acc_odr_t_hz3200:
        tim_pwm_period_us = 40000;
        nsamples_per_trg = 64;
        odr = ISM6HG256X_ODR_AT_3840Hz;        
       break;
     default:
       return;  
  }

/* switch INT2 PB4 from EXTI mode to GPIO hiz */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = A_G_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(A_G_INT2_GPIO_Port, &GPIO_InitStruct);

/* set the PWM frequency */
  TIM2_PWM_Period_Init(tim_pwm_period_us); 
/* start PWM */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

/* move sample ready INT from INT2 to INT1 */
  MY_CUSTOM_MOTION_SENSOR_ACC_Disable_DRDY_On_INT2 (SENSOR_0);

  MY_CUSTOM_MOTION_SENSOR_ACC_Enable_DRDY_On_INT1 (SENSOR_0);

  CUSTOM_MOTION_SENSOR_Disable(SENSOR_0, MOTION_ACCELERO);

// API not yet available in the driver, need to be added as custom function using ism6hg256x_odr_trig_cfg_set and ism6hg256x_odr_trig_cfg_get to be able to set/get the ODR trgd mode configuration (ODR + num samples per trg)
//ISM6HG256X_ACC_SetOutputDataRate_With_Mode((ISM6HG256X_Object_t *)MotionCompObj[SENSOR_0], 100, ISM6HG256X_ACC_ODR_TRIGGERED_MODE);

  /* set 8 samples per Trg period */
// ism6hg256x_odr_trig_cfg_set((const stmdev_ctx_t *)MotionCompObj[SENSOR_0], 8);
  /* set nsamples per trigger period */
  CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_ODR_TRIG_CFG, nsamples_per_trg);
// CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_ODR_TRIG_CFG, 4);

 /* set ODR tgrd mode */
  CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_CTRL1, 0x30);

 /* set ODR trgd PWM input active high */
  uint8_t reg4_value;
  CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_CTRL4, &reg4_value);        
  reg4_value |= 0x01; 
  CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_CTRL4, reg4_value);

 /*  set ODR trgd freq */
  CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_CTRL1, (0x30 | odr));
 
 /* set INT pulsed  */
  reg4_value |= 0x02;  // set INT pulsed
//  CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_CTRL4, reg4_value);

/* in INT1 ISR handle Sample Ready (instead of INT2 and verify if MLC can cohexist on INT1 with Acc DataReady or need to move it on INT2) */
  HAL_NVIC_EnableIRQ(A_G_INT1_EXTI_IRQn);
  HAL_NVIC_DisableIRQ(A_G_INT2_EXTI_IRQn);

/* give time 4 * 80mSec to converge discarding first samples  */

}