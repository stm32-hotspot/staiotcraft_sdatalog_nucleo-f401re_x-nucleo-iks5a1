/**
  ******************************************************************************
  * @file           : fsm.h
  * @brief          : Application code for sensor FSM
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FSM_H
#define __FSM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

// Define FSM states
typedef enum {
    FSM_STATE_DISABLED,
    FSM_STATE_ENABLED,
    FSM_STATE_ENABLED_ODR_TRGD,    
    FSM_STATE_LOADED,
    FSM_STATE_LOGGING_DATA,
    FSM_STATE_LOGGING_DATA_ODR_TRGD    
} State;

// Define FSM events
typedef enum {
    FSM_EVT_INT,
    FSM_EVT_ENABLE,
    FSM_EVT_ENABLE_ODR_TRGD,
    FSM_EVT_DISABLE,
    FSM_EVT_LOAD,
    FSM_EVT_START_LOG,
    FSM_EVT_STOP_LOG,
    FSM_EVT_NONE
} Event;

// Define the function pointer type for the user defined state actions
typedef void (*StateAction)(Event event, void* args);

// Define the structure for the FSM state transition
typedef struct {
    State       currentState;
    Event       event;
    State       nextState;
    StateAction action;
} Transition;

// Define the structure for the user instantiated FSM object
typedef struct {
    State             currentState;
    Transition *      transitions;
    int               numTransitions;
    volatile Event    currentEvent;
    void*             eventArgs; // pointer to event-specific arguments
    IRQn_Type         irq;
} FSM;

/* Public API must be called from any user func to init the FSM*/
extern void FsmInit(FSM* fsm, State initialState, Transition* transitions, int numTransitions, IRQn_Type irq);

/* Public API must be called from any user func to deinit the FSM */
extern void FsmDeInit(FSM* fsm);

/* Public API can be called from within a ISR user func to notify event */
// Extended version
extern void FsmSetEventFromINTEx(FSM* fsm, Event event, void* args);
// Backward compatible 2-argument version
static inline void FsmSetEventFromINT(FSM* fsm, Event event) { FsmSetEventFromINTEx(fsm, event, NULL); }

/* Public API can be called from any user func to notify event */
// Extended version
extern void FsmSetEventEx(FSM* fsm, Event event, void* args);
// Backward compatible 2-argument version
static inline void FsmSetEvent(FSM* fsm, Event event) { FsmSetEventEx(fsm, event, NULL); }

/* Public API must be called periodically from any user func to process events */
extern void FsmHandleEvent(FSM* fsm);

/* Public API to check if FSM is in a specific state */
extern bool FsmCheckState(FSM* fsm, State state);

#ifdef __cplusplus
}
#endif

#endif /* __FSM_H */