#include "os_api.h"
#include "timer.h"

#if (BSW_CFG_TIMER_FUNCTION == STD_ENABLED)

static uint16 Timer_aTimeout[TIMER_MAX_TIMER_ID_PRESENT];
static uint16 Timer_aTick[TIMER_MAX_TIMER_ID_PRESENT];

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
}

/**
 * @brief       Get timer status by ID
 * @param[in]   eTimerID        TimerID
 * @retval      TIMER_EXPIRED   Timer is expired
 * @retval      TIMER_RUNNING   Timer is running
 */
Timer_StatusType Timer_GetTimerStatus(const Timer_TimerID eTimerID)
{
    if (Os_GetSysTick() - Timer_aTick[eTimerID] >= Timer_aTimeout[eTimerID])
    {
        return TIMER_EXPIRED;
    }
    else
    {
        return TIMER_RUNNING;
    }
}

/**
 * @brief       Timer initialize function
 * @retval      None
 */
void Timer_InitFunction(void)
{
    uint8 u8Index;

    for (u8Index = 0; u8Index < TIMER_MAX_TIMER_ID_PRESENT; u8Index++)
    {
        Timer_aTimeout[u8Index] = 0U;
        Timer_aTick[u8Index]    = 0U;
    }
}

#endif /* (BSW_CFG_TIMER_FUNCTION = STD_ENABLED) */
