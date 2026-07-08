/**
 ******************************************************************************
 * @file    timestamp.c
 *
 * @brief   This file provides a way to obtaining an accurate timestamp based
 *          on STM32 clock through a 32bit HW timer.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "timestamp.h"
#include "vdatalog_conf.h"
#include "fsm.h"
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef VD_TIMESTAMP_TIM;
extern TIM_HandleTypeDef MLC_POLLING_TIM;
extern volatile uint8_t  mlc_tim_interrupt;
extern FSM 			         MlcFsm;

/* Private function prototypes -----------------------------------------------*/
static void TS_TIM_VD_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
static void TS_TIM_MLC_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

static volatile uint64_t GlobalTimeStamp = 0;   /* Sensor Manager global timestamp */

static bool vd_tim_started = false;  
static bool mlc_tim_started = false; 
/**
  * Initialize the VD timer to be used for system timestamp.
  *
  * @return None
  */
void TS_TIM_VD_Init(void)
{
  vd_tim_started = false;
  #ifndef NO_TIM5
  HAL_TIM_RegisterCallback(&VD_TIMESTAMP_TIM, HAL_TIM_PERIOD_ELAPSED_CB_ID, TS_TIM_VD_PeriodElapsedCallback);
  #endif
}

/**
  * Initialize the MLC timer to be used for periodically 1Sec send MLC status.
  *
  * @return None
  */
void TS_TIM_MLC_Init(void)
{
  mlc_tim_started = false;
#ifndef NO_TIM3  
  HAL_TIM_RegisterCallback(&MLC_POLLING_TIM, HAL_TIM_PERIOD_ELAPSED_CB_ID, TS_TIM_MLC_PeriodElapsedCallback);
#endif
}


/**
  * This function cannot be called from ISR.
  * Get the system timestamp in seconds. If the service was not started then the value is undefined.
  *
  * @return the value of the system timestamp in seconds.
  */
#ifndef NO_TIM5
double TS_VD_GetTimeStamp(void)
{
  uint32_t tim_counter;
  uint32_t tim_period;
  uint64_t timestamp;
  double timestamp_seconds;

  /* -------- Start critical section -----------*/
  __disable_irq();

  tim_counter = LL_TIM_GetCounter(VD_TIMESTAMP_TIM.Instance);
  tim_period = LL_TIM_GetAutoReload(VD_TIMESTAMP_TIM.Instance) + 1U;

  if (__HAL_TIM_GET_FLAG(&VD_TIMESTAMP_TIM, TIM_FLAG_UPDATE)) /* Update Event happened while already in critical section */
  {
    /* Evaluate if the timer was read before or after the "UPDATE" event */
    if (tim_counter < (tim_period / (uint64_t)2)) /* After */
    {
      timestamp = GlobalTimeStamp + tim_counter + tim_period;
    }
    else /* Before */
    {
      timestamp = GlobalTimeStamp + tim_counter;
    }
  }
  else
  {
    /* No Update Event, just sum the timer value to the global TimeStamp */
    timestamp = GlobalTimeStamp + tim_counter;
  }

  timestamp_seconds = ((double)timestamp / (double)SystemCoreClock);

  __enable_irq();
  /* -------- End critical section -----------*/
  return timestamp_seconds;
}
#elif defined(NO_TIM5)
double TS_VD_GetTimeStamp(void)  
{
  return GlobalTimeStamp++;
}       
#endif

/**
  * This function can be called from ISR.
  * Get the system timestamp in seconds. If the service was not started then the value is undefined.
  *
  * @return the value of the system timestamp in seconds.
  */
#ifndef NO_TIM5  
double TS_VD_GetTimeStamp_fromISR(void)
{
  uint32_t tim_counter;
  uint32_t tim_period;
  uint64_t timestamp;
  double timestamp_seconds;
  uint32_t isr_mask;

  /* -------- Start critical section -----------*/
  isr_mask = __get_PRIMASK();       /**< backup Priority Mask bit */
  __disable_irq();                  /**< Disable all interrupts by setting PRIMASK bit on Cortex*/

  tim_counter = LL_TIM_GetCounter(VD_TIMESTAMP_TIM.Instance);
  tim_period = LL_TIM_GetAutoReload(VD_TIMESTAMP_TIM.Instance) + 1U;
  if (__HAL_TIM_GET_FLAG(&VD_TIMESTAMP_TIM, TIM_FLAG_UPDATE)) /* Update Event happened while already in critical section */
  {
    /* Evaluate if the timer was read before or after the "UPDATE" event */
    if (tim_counter < (tim_period / (uint64_t)2)) /* After */
    {
      timestamp = GlobalTimeStamp + tim_counter + tim_period;
    }
    else /* Before */
    {
      timestamp = GlobalTimeStamp + tim_counter;
    }
  }
  else
  {
    /* No Update Event, just sum the timer value to the global TimeStamp */
    timestamp = GlobalTimeStamp + tim_counter;
  }

  timestamp_seconds = ((double)timestamp / (double)SystemCoreClock);

  __set_PRIMASK(isr_mask);     /**< Restore Priority Mask bit*/
  /* -------- End critical section -----------*/

  return timestamp_seconds;
}
#elif defined(NO_TIM5)
double TS_VD_GetTimeStamp_fromISR(void)
{
  return GlobalTimeStamp++;   // simulate incrementing timestamp
} 
#endif      
/*
 * Update the global VDL Timestamp when the timer rolls back
 */
static void TS_TIM_VD_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  (void)htim;
  uint64_t tim_period = LL_TIM_GetAutoReload(VD_TIMESTAMP_TIM.Instance);

//  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET);
  tim_period = LL_TIM_GetAutoReload(VD_TIMESTAMP_TIM.Instance);
  GlobalTimeStamp += tim_period + 1U;
//  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET);
}

/*
 * Periodic interrupt for MLC use-cases.
 */
void TS_TIM_MLC_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    (void)htim;
    mlc_tim_interrupt = 1;
    /* signal the MLC FSM to update GUI with last MLC state */
    FsmSetEventFromINT(&MlcFsm, FSM_EVT_INT);
}

void TS_TIM_MLC_Start(void)
{
  #ifndef NO_TIM3  
  LL_TIM_ClearFlag_UPDATE(MLC_POLLING_TIM.Instance);   
  HAL_TIM_Base_Start_IT(&MLC_POLLING_TIM);
  #endif  
  mlc_tim_started = true;
}

void TS_TIM_MLC_Stop(void)
{
  #ifndef NO_TIM3
  HAL_TIM_Base_Stop_IT(&MLC_POLLING_TIM);
  LL_TIM_ClearFlag_UPDATE(MLC_POLLING_TIM.Instance);   
  #endif
  mlc_tim_started = false;
}

bool TS_TIM_MLC_IsStarted(void)
{
  return mlc_tim_started;
}

/**
  * Start the timer to be used for system timestamp.
  *
  * @return None
  */

void TS_TIM_VD_Start(void)
{
  /* Start the TIM Base generation */
  vd_tim_started = true;
  #ifndef NO_TIM5  
  LL_TIM_ClearFlag_UPDATE(VD_TIMESTAMP_TIM.Instance);
  HAL_TIM_Base_Start_IT(&VD_TIMESTAMP_TIM);
  #endif
}

/**
  * Stop the Timestamp timer and reset counters.
  *
  * @return None
  */

void TS_TIM_VD_Stop(void)
{
  /* Stop Timer */
  #ifndef NO_TIM5     
  HAL_TIM_Base_Stop(&VD_TIMESTAMP_TIM);
  __HAL_TIM_SET_COUNTER(&VD_TIMESTAMP_TIM, 0);
  #endif
  GlobalTimeStamp = 0;
  vd_tim_started = false;
}
/**
  * Return the Timer status
  *
  * @return None
  */
bool TS_TIM_VD_IsStarted(void)
{
  return vd_tim_started;
}
