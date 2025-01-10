#include "os_api.h"
#include "timer.h"

#if (BSW_CFG_TIMER_FUNCTION == STD_ENABLED)
static uint16 Timer_aTimeout[TIMER_MAX_TIMER_ID_PRESENT];
static uint16 Timer_aTick[TIMER_MAX_TIMER_ID_PRESENT];
static uint16 Timer_aStatus[TIMER_MAX_TIMER_ID_PRESENT];

/**
 * @brief       Stop timer
 * @param[in]   eTimerID        TimerID
 * @retval      None
 */
void Timer_StopTimer(const Timer_TimerID eTimerID)
{
    Timer_aStatus[eTimerID]  = TIMER_STOP;
}

/**
 * @brief       Set timer
 * @param[in]   eTimerID        TimerID
 * @param[in]   u16Timeout      Timeout value
 * @retval      None
 */
void Timer_SetTimer(const Timer_TimerID eTimerID, const uint16 u16Timeout)
{
    Timer_aTimeout[eTimerID] = u16Timeout;
    Timer_aTick[eTimerID]    = Os_GetSysTick();
    Timer_aStatus[eTimerID]  = TIMER_RUNNING;
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
    if (Timer_aStatus[eTimerID] != TIMER_STOP)
    {
        if (Os_GetSysTick() - Timer_aTick[eTimerID] >= Timer_aTimeout[eTimerID])
        {
            Timer_aStatus[eTimerID] = TIMER_EXPIRED;
        }
        else
        {
            Timer_aStatus[eTimerID] = TIMER_RUNNING;
        }
    }

    return Timer_aStatus[eTimerID];
}
#endif /* (BSW_CFG_TIMER_FUNCTION = STD_ENABLED) */
