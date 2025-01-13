#ifndef TIMER_H
#define TIMER_H

#include "timer_cfg.h"

#if (BSW_CFG_TIMER_FUNCTION == STD_ENABLED)
typedef enum __attribute__((packed))
{
    TIMER_DEBOUNCE,

#if (BSW_CFG_COM_FUNCTION == STD_ENABLED)
    TIMER_RX_MSG_TIMEOUT,
#endif /* (BSW_CFG_COM_FUNCTION == STD_ENABLED) */

#if (BSW_CFG_DIAGNOSTIC_FUNCTION == STD_ENABLED)
    TIMER_START_DIAG_SESSION,
    TIMER_DIAGNOSTIC_SESSION,
    TIMER_SECURE_SESSION,
#endif /* (BSW_CFG_DIAGNOSTIC_FUNCTION == STD_ENABLED) */

    TIMER_MAX_TIMER_ID_PRESENT,
} Timer_TimerID;

typedef enum __attribute__((packed))
{
    TIMER_STOP,
    TIMER_RUNNING,
    TIMER_EXPIRED,
} Timer_StatusType;

extern void Timer_StopTimer(const Timer_TimerID eTimerID);
extern void Timer_SetTimer(const Timer_TimerID eTimerID, const uint16 u16Timeout);
extern Timer_StatusType Timer_GetTimerStatus(const Timer_TimerID eTimerID);

#endif /* (BSW_CFG_TIMER_FUNCTION = STD_ENABLED) */
#endif /* TIMER_H */
