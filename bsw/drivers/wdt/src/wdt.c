#include "wdt.h"


/**
 * @brief       Suspend watchdog timer
 * @retval      None
 */
void WDT_SuspendWatchdogTimer(void)
{
    WDTCTL = WDTPW + WDTHOLD;
}

/**
 * @brief       Clear watchdog timer
 * @retval      None
 */
void WDT_ClearWatchdogTimer(void)
{
    WDTCTL = WDTPW + WDTCNTCL;
}

/**
 * @brief       Start watchdog timer
 * @retval      None
 */
void WDT_StartWatchdogTimer(void)
{
    WDTCTL = WDT_CFG_WDTCTL_REG;
}


/* TODO: Implement logging when watchdog timer is expired */
