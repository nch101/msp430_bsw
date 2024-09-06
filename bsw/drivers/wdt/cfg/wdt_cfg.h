#ifndef WDT_CFG_H
#define WDT_CFG_H

#include "bsw_cfg.h"
#include "mcu_cfg.h"

#define WDT_CFG_WDT_TIMEOUT                 WDT_SEL_4MS_TIMEOUT

#define WDT_CFG_CLOCK_SOURCE                WDT_SEL_SMCLK


#if (MCU_CFG_CLOCK_RATE == CLK_SEL_1MHZ)
    #if (WDT_CFG_WDT_TIMEOUT == WDT_SEL_32MS_TIMEOUT)
        #define WDT_CFG_WDT_TIMEOUT_REG     (0x00U)
    #elif (WDT_CFG_WDT_TIMEOUT == WDT_SEL_8MS_TIMEOUT)
        #define WDT_CFG_WDT_TIMEOUT_REG     (WDTIS0)
    #elif (WDT_CFG_WDT_TIMEOUT == WDT_SEL_0_5MS_TIMEOUT)
        #define WDT_CFG_WDT_TIMEOUT_REG     (WDTIS1)
    #else
        #define WDT_CFG_WDT_TIMEOUT_REG     WDTHOLD
        #warning "Watchdog timeout value is not supported. Watchdog timer will be stopped"
    #endif /* (WDT_CFG_WDT_TIMEOUT == WDT_SEL_32MS_TIMEOUT) */
#elif (MCU_CFG_CLOCK_RATE == CLK_SEL_8MHZ)
    #if (WDT_CFG_WDT_TIMEOUT == WDT_SEL_4MS_TIMEOUT)
        #define WDT_CFG_WDT_TIMEOUT_REG     (0x00U)
    #else
        #define WDT_CFG_WDT_TIMEOUT_REG     WDTHOLD
        #warning "Watchdog timeout value is not supported. Watchdog timer will be stopped"
    #endif /* (WDT_CFG_WDT_TIMEOUT == WDT_SEL_4MS_TIMEOUT) */
#else
    #define WDT_CFG_WDT_TIMEOUT_REG         WDTHOLD
    #warning "Watchdog timeout value is not supported. Watchdog timer will be stopped"
#endif /* (MCU_CFG_CLOCK_RATE == CLK_SEL_1MHZ) */


#if (WDT_CFG_CLOCK_SOURCE == WDT_SEL_SMCLK)
    #define WDT_CFG_CLOCK_SOURCE_REG        0x00
#else
    #define WDT_CFG_CLOCK_SOURCE_REG        (WDTSSEL)
#endif /* (WDT_CFG_CLOCK_SOURCE == WDT_SEL_SMCLK) */


/**
 * @brief Watchdog Timer+ Control Register
 */
#define WDT_CFG_WDTCTL_REG                  WDTPW \
                                            | WDTCNTCL \
                                            | WDT_CFG_CLOCK_SOURCE_REG \
                                            | WDT_CFG_WDT_TIMEOUT_REG

#endif /* WDT_CFG_H */
