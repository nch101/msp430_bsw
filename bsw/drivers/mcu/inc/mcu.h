#ifndef MCU_H
#define MCU_H

#include "mcu_cfg.h"


/**
 * @brief   Operating mode enum type
 * @details
 *  MCU_ACTIVE_MODE            
 *      - All clocks are active
 *  MCU_LOW_POWER_0_MODE
 *      - CPU is disabled
 *      - ACLK and SMCLK remain active, MCLK is disabled
 *  MCU_LOW_POWER_1_MODE
 *      - CPU is disabled
 *      - ACLK and SMCLK remain active, MCLK is disabled
 *      - DCO's dc generator is disabled if DCO not used in active mode
 *  MCU_LOW_POWER_2_MODE
 *      - CPU is disabled
 *      - MCLK and SMCLK are disabled
 *      - DCO's dc generator remains enabled
 *      - ACLK remains active
 *  MCU_LOW_POWER_3_MODE
 *      - CPU is disabled
 *      - MCLK and SMCLK are disabled
 *      - DCO's dc generator is disabled
 *      - ACLK remains active
 *  MCU_LOW_POWER_4_MODE
 *      - CPU is disabled
 *      - ACLK is disabled
 *      - MCLK and SMCLK are disabled
 *      - DCO's dc generator is disabled
 *      - Crystal oscillator is stopped
 */
typedef enum
{
    MCU_ACTIVE_MODE,
    MCU_LOW_POWER_0_MODE,
    MCU_LOW_POWER_1_MODE,
    MCU_LOW_POWER_2_MODE,
    MCU_LOW_POWER_3_MODE,
    MCU_LOW_POWER_4_MODE,
} Mcu_OperatingModeType;

extern void MCU_InitClock(void);
extern void MCU_InitOperatingMode(void);

#if (MCU_CFG_OPERATING_MODE_API == STD_ENABLED)
extern void MCU_EnterOperatingMode(const Mcu_OperatingModeType OperatingMode);
extern uint8 MCU_GetOperatingMode(void);
#endif /* (MCU_CFG_OPERATING_MODE_API == STD_ENABLED) */

#endif /* MCU_H */
