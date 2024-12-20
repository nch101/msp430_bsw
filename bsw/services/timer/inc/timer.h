#ifndef TIMER_H
#define TIMER_H

#include "timer_cfg.h"

#if (BSW_CFG_TIMER_FUNCTION == STD_ENABLED)
typedef enum __attribute__((packed))
{
    TIMER_DEBOUNCE,
    TIMER_MAX_TIMER_ID_PRESENT,
} Timer_TimerID;

typedef enum __attribute__((packed))
{
    TIMER_RUNNING,
    TIMER_EXPIRED,
} Timer_StatusType;

extern void Timer_SetTimer(const Timer_TimerID eTimerID, const uint16 u16Timeout);
extern Timer_StatusType Timer_GetTimerStatus(const Timer_TimerID eTimerID);
extern void Timer_InitFunction(void);

#endif /* (BSW_CFG_TIMER_FUNCTION = STD_ENABLED) */
#endif /* TIMER_H */
