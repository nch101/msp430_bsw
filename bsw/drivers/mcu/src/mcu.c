#include "mcu.h"


/**
 * @brief       Clock initialization function
 * @retval      None
 */
void MCU_InitClock(void)
{
    /* Reset DCO settings */
    DCOCTL = 0;

    /* Config clock */
    DCOCTL  = MCU_CFG_DCOCTL_REG;
    BCSCTL1 = MCU_CFG_BCSCTL1_REG;
}
