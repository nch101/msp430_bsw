#include "mcu.h"


#if (MCU_CFG_OPERATING_MODE_API == STD_ENABLED)
static uint8 Mcu_u8OperatingMode;
#endif /* (MCU_CFG_OPERATING_MODE_API == STD_ENABLED) */


/**
 * @brief       Clock initialization function
 * @retval      None
 */
void Mcu_InitClock(void)
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
 * @param[in]   eOperatingMode - Operating mode
 * @retval      None
 */
void Mcu_EnterOperatingMode(const Mcu_OperatingModeType eOperatingMode)
{
    switch (eOperatingMode)
    {
        case MCU_ACTIVE_MODE:
        {
            Mcu_u8OperatingMode = 0x00U;
            break;
        }

        case MCU_LOW_POWER_0_MODE:
        {
            Mcu_u8OperatingMode = LPM0_bits;
            break;
        }

        case MCU_LOW_POWER_1_MODE:
        {
            Mcu_u8OperatingMode = LPM1_bits;
            break;
        }

        case MCU_LOW_POWER_2_MODE:
        {
            Mcu_u8OperatingMode = LPM2_bits;
            break;
        }

        case MCU_LOW_POWER_3_MODE:
        {
            Mcu_u8OperatingMode = LPM3_bits;
            break;
        }

        case MCU_LOW_POWER_4_MODE:
        {
            Mcu_u8OperatingMode = LPM4_bits;
            break;
        }

        default:
            break;
    }

    __bis_SR_register(Mcu_u8OperatingMode);
}

/**
 * @brief       Get operating mode function
 * @retval      Mcu_u8OperatingMode
 */
uint8 Mcu_GetOperatingMode(void)
{
    return Mcu_u8OperatingMode;
}
#endif /* (MCU_CFG_OPERATING_MODE_API == STD_ENABLED) */

/**
 * @brief       Operating mode initialization function
 * @retval      None
 */
void Mcu_InitOperatingMode(void)
{

#if (MCU_CFG_OPERATING_MODE_API == STD_ENABLED)
    Mcu_u8OperatingMode = MCU_CFG_INIT_OPERATING_POWER_MODE;
    __bis_SR_register(Mcu_u8OperatingMode + GIE);
#else
    __bis_SR_register(MCU_CFG_INIT_OPERATING_POWER_MODE + GIE);
#endif /* (MCU_CFG_OPERATING_MODE_API == STD_ENABLED) */

}
