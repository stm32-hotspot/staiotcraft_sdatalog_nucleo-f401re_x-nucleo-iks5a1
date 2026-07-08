/**
  ******************************************************************************
  * @file           : fsm.c
  * @brief          : Application code for finished state machine
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
#include "fsm.h"
#include <stdbool.h>

/*  generic FSM Function API code */
/** API to initialize the FSM
 * fsm: Finished State Machine global handler
 * initialState: initial FSM state
 * transitions: handler for FSM state transitions
 * numTransitions: total number of possible transitions
 * irq: the irq generating events for FSM
 */

void FsmInit(FSM* fsm, State initialState, Transition* transitions, int numTransitions, IRQn_Type irq)
{
    fsm->currentState   = initialState;
    fsm->transitions    = transitions;
    fsm->numTransitions = numTransitions;
    fsm->currentEvent   = FSM_EVT_NONE;
    fsm->irq            = irq;
    fsm->eventArgs      = NULL;
}

void FsmDeInit(FSM* fsm)
{
    fsm->currentState   =     FSM_STATE_DISABLED;
    fsm->transitions    = NULL;
    fsm->numTransitions = 0;
    fsm->currentEvent   = FSM_EVT_NONE;
    fsm->irq            = (IRQn_Type)0;
    fsm->eventArgs      = NULL;
}

static inline void FsmClearEvent(FSM* fsm)
{
    uint32_t primask = __get_PRIMASK(); // Save the current interrupt state
    __disable_irq(); // Disable interrupts

    fsm->currentEvent = FSM_EVT_NONE;
    fsm->eventArgs = NULL;

    __set_PRIMASK(primask); // Restore the previous interrupt state
}

// FSM API Handling an event
void FsmHandleEvent(FSM* fsm) {
    if (fsm->currentEvent == FSM_EVT_NONE) return;  // no incoming events

    Event event = fsm->currentEvent;
    void* args = fsm->eventArgs;
    FsmClearEvent(fsm);

    /* search for the transition in the LUT */
    for (int i = 0; i < fsm->numTransitions; i++) {
        Transition* t = &fsm->transitions[i];
        if (t->currentState == fsm->currentState && t->event == event) {
            if (t->action != NULL) {
                t->action(event, args);
            }
            fsm->currentState = t->nextState;
            return;
        }
    }
    //   printf("No transition found for state %d and event %d\n", fsm->currentState, event);
}

/* can be called from within a ISR  */
void FsmSetEventFromINTEx(FSM* fsm, Event event, void* args)
{
    /** check if a race condt is occurring, meaning an event was already set but not yet served by FSM. The time in between the
     * event setting and the event serving by FSM is a critical section because the event can be override by INT
     * (eg. Stop log msg trigger an event in main and before serving it an INT from sensor arrive overriding past event.
     * NOTE to handle this occurrence events should be queued **/
    if (fsm->currentEvent != FSM_EVT_NONE) return; /* ignore this INT to avoid overriding pending cmd event */

    uint32_t primask = __get_PRIMASK(); // Save the current interrupt state
    __disable_irq(); // Disable interrupts
    fsm->currentEvent = event;
    fsm->eventArgs = args;
    __set_PRIMASK(primask); // Restore the previous interrupt state
}

/* can be called from any user func */
void FsmSetEventEx(FSM* fsm, Event event, void* args)
{
    uint32_t primask = __get_PRIMASK(); // Save the current interrupt state
    __disable_irq(); // Disable interrupts

    fsm->currentEvent = event;
    fsm->eventArgs = args;
    /* handle the event immediately to avoid event override by INT */
    FsmHandleEvent(fsm);

    __set_PRIMASK(primask); // Restore the previous interrupt state

}

/* Check if FSM is in a specific state */
bool FsmCheckState(FSM* fsm, State state)
{
    if (!fsm) return false;
    return (fsm->currentState == state) ? true : false;
}
