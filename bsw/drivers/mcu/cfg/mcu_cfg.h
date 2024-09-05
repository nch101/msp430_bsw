#ifndef MCU_CFG_H
#define MCU_CFG_H

#include "bsw_cfg.h"

/**
 * @brief Clock rate 
 * @enum
 *          CLK_SEL_1MHZ            1Mhz
 *          CLK_SEL_8MHZ            8Mhz
 *          CLK_SEL_12MHZ           12Mhz
 *          CLK_SEL_16MHZ           16Mhz
*/
#define MCU_CFG_CLOCK_RATE                  CLK_SEL_8MHZ

#define MCU_CFG_INIT_OPERATING_POWER_MODE   0x00U;

#define MCU_CFG_OPERATING_MODE_API          STD_DISABLED

#if (MCU_CFG_CLOCK_RATE == CLK_SEL_1MHZ)
    #define MCU_CFG_DCOCTL_REG              CALDCO_1MHZ
    #define MCU_CFG_BCSCTL1_REG             CALBC1_1MHZ
#elif (MCU_CFG_CLOCK_RATE == CLK_SEL_8MHZ)
    #define MCU_CFG_DCOCTL_REG              CALDCO_8MHZ
    #define MCU_CFG_BCSCTL1_REG             CALBC1_8MHZ
#elif (MCU_CFG_CLOCK_RATE == CLK_SEL_12MHZ)
    #define MCU_CFG_DCOCTL_REG              CALDCO_12MHZ
    #define MCU_CFG_BCSCTL1_REG             CALBC1_12MHZ
#elif (MCU_CFG_CLOCK_RATE == CLK_SEL_16MHZ)
    #define MCU_CFG_DCOCTL_REG              CALDCO_16MHZ
    #define MCU_CFG_BCSCTL1_REG             CALBC1_16MHZ
#else
    #error "Current clock rate is not supported"
#endif /* (MCU_CFG_CLOCK_RATE == CLK_SEL_1MHZ) */

#endif /* MCU_CFG_H */
