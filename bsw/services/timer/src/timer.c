#include "os_api.h"
#include "timer.h"

#if (BSW_CFG_TIMER_FUNCTION == STD_ENABLED)
typedef struct Timer_t
{
    uint16              u16Timeout;
    uint16              u16StartedTick;
    Timer_StatusType    eStatus;
} Timer_t;

static Timer_t Timer[TIMER_MAX_TIMER_ID_PRESENT];

/**
 * @brief       Stop timer
 * @param[in]   eTimerID        TimerID
 * @retval      None
 */
void Timer_StopTimer(const Timer_TimerID eTimerID)
{
    Timer[eTimerID].eStatus = TIMER_STOP;
}

/**
 * @brief       Set timer
 * @param[in]   eTimerID        TimerID
 * @param[in]   u16Timeout      Timeout value
 * @retval      None
 */
void Timer_SetTimer(const Timer_TimerID eTimerID, const uint16 u16Timeout)
{
    Timer[eTimerID].u16Timeout      = u16Timeout;
    Timer[eTimerID].u16StartedTick  = Os_GetSysTick();
    Timer[eTimerID].eStatus         = TIMER_RUNNING;
}

/**
 * @brief       Get timer status by ID
 * @param[in]   eTimerID        TimerID
 * @retval      TIMER_STOP      Timer is stop
 * @retval      TIMER_RUNNING   Timer is running
 * @retval      TIMER_EXPIRED   Timer is expired
 */
Timer_StatusType Timer_GetTimerStatus(const Timer_TimerID eTimerID)
{
    if (Timer[eTimerID].eStatus != TIMER_STOP)
    {
        if (Os_GetSysTick() >= Timer[eTimerID].u16StartedTick)
        {
            if (Os_GetSysTick() - Timer[eTimerID].u16StartedTick >= Timer[eTimerID].u16Timeout)
            {
                Timer[eTimerID].eStatus = TIMER_EXPIRED;
            }
        }
        else
        {
            if (Os_GetSysTick() - Timer[eTimerID].u16StartedTick >= Timer[eTimerID].u16Timeout)
            {
                Timer[eTimerID].eStatus = TIMER_EXPIRED;
            }
        }
    }

    return Timer[eTimerID].eStatus;
}
#endif /* (BSW_CFG_TIMER_FUNCTION = STD_ENABLED) */
