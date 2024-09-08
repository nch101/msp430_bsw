#include "mcu.h"


#if (MCU_CFG_OPERATING_MODE_API == STD_ENABLED)
static uint8 MCU_gOperatingMode;
#endif /* (MCU_CFG_OPERATING_MODE_API == STD_ENABLED) */


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

#if (MCU_CFG_OPERATING_MODE_API == STD_ENABLED)
/**
 * @brief       Entering operating mode function
 * @param[in]   OperatingMode - Operating mode
 * @retval      None
 */
void MCU_EnterOperatingMode(const Mcu_OperatingModeType OperatingMode)
{
    switch (OperatingMode)
    {
        case MCU_ACTIVE_MODE:
        {
            MCU_gOperatingMode = 0x00U;
            break;
        }

        case MCU_LOW_POWER_0_MODE:
        {
            MCU_gOperatingMode = LPM0_bits;
            break;
        }

        case MCU_LOW_POWER_1_MODE:
        {
            MCU_gOperatingMode = LPM1_bits;
            break;
        }

        case MCU_LOW_POWER_2_MODE:
        {
            MCU_gOperatingMode = LPM2_bits;
            break;
        }

        case MCU_LOW_POWER_3_MODE:
        {
            MCU_gOperatingMode = LPM3_bits;
            break;
        }

        case MCU_LOW_POWER_4_MODE:
        {
            MCU_gOperatingMode = LPM4_bits;
            break;
        }

        default:
            break;
    }

    __bis_SR_register(MCU_gOperatingMode);
}

/**
 * @brief       Get operating mode function
 * @retval      uint8 OperatingMode
 */
uint8 MCU_GetOperatingMode(void)
{
    return MCU_gOperatingMode;
}
#endif /* (MCU_CFG_OPERATING_MODE_API == STD_ENABLED) */

/**
 * @brief       Operating mode initialization function
 * @retval      None
 */
void MCU_InitOperatingMode(void)
{

#if (MCU_CFG_OPERATING_MODE_API == STD_ENABLED)
    MCU_gOperatingMode = MCU_CFG_INIT_OPERATING_POWER_MODE;
    __bis_SR_register(MCU_gOperatingMode + GIE);
#else
    __bis_SR_register(MCU_CFG_INIT_OPERATING_POWER_MODE + GIE);
#endif /* (MCU_CFG_OPERATING_MODE_API == STD_ENABLED) */

}
