#ifndef GPT_CFG_H
#define GPT_CFG_H

#include "bsw_cfg.h"
#include "mcu_cfg.h"

/**
 * @brief 
 */
#define GPT_CFG_TIMER_A                     STD_ENABLED

/**
 * @brief 
 * @enum
 *          GPT_SEL_TACLK                   TACLK
 *          GPT_SEL_ACLK                    ACLK
 *          GPT_SEL_SMCLK                   SMCLK
 */
#define GPT_CFG_TIMER_A_CLOCK_SOURCE        GPT_SEL_SMCLK

/**
 * @brief 
 * @enum
 *          GPT_SEL_STOP_MODE               Stop mode: the timer is halted
 *          GPT_SEL_UP_MODE                 Up mode: the timer counts up to TACCR0
 *          GPT_SEL_CON_MODE                Continuous mode: the timer counts up to 0FFFFh.
 *          GPT_SEL_UP_DOWN_MODE            Up/down mode: the timer counts up to TACCR0 then down to 0000h
 */
#define GPT_CFG_TIMER_A_MODE                GPT_SEL_UP_MODE

/**
 * @brief 
 * @enum
 *          GPT_SEL_TRIGGER_0_5MS           Timer A ISR is triggered every 0.5 ms
 *          GPT_SEL_TRIGGER_1MS             Timer A ISR is triggered every 1 ms
 */
#define GPT_CFG_TIMER_A_TRIGGER_TIME        GPT_SEL_TRIGGER_1MS

#if (MCU_CFG_CLOCK_RATE == CLK_SEL_1MHZ)
    #if (GPT_CFG_TIMER_A_TRIGGER_TIME == GPT_SEL_TRIGGER_1MS)
        #define GPT_CFG_TIMER_A_COUNT           1000
    #else
        #define GPT_CFG_TIMER_A_COUNT           500
    #endif /* (GPT_CFG_TIMER_A_TRIGGER_TIME == GPT_SEL_TRIGGER_1MS) */
#elif (MCU_CFG_CLOCK_RATE == CLK_SEL_8MHZ)
    #if (GPT_CFG_TIMER_A_TRIGGER_TIME == GPT_SEL_TRIGGER_1MS)
        #define GPT_CFG_TIMER_A_COUNT           8000
    #else
        #define GPT_CFG_TIMER_A_COUNT           4000
    #endif /* (GPT_CFG_TIMER_A_TRIGGER_TIME == GPT_SEL_TRIGGER_1MS) */
#elif (MCU_CFG_CLOCK_RATE == CLK_SEL_12MHZ)
    #if (GPT_CFG_TIMER_A_TRIGGER_TIME == GPT_SEL_TRIGGER_1MS)
        #define GPT_CFG_TIMER_A_COUNT           12000
    #else
        #define GPT_CFG_TIMER_A_COUNT           6000
    #endif /* (GPT_CFG_TIMER_A_TRIGGER_TIME == GPT_SEL_TRIGGER_1MS) */
#elif (MCU_CFG_CLOCK_RATE == CLK_SEL_16MHZ)
    #if (GPT_CFG_TIMER_A_TRIGGER_TIME == GPT_SEL_TRIGGER_1MS)
        #define GPT_CFG_TIMER_A_COUNT           16000
    #else
        #define GPT_CFG_TIMER_A_COUNT           8000
    #endif /* (GPT_CFG_TIMER_A_TRIGGER_TIME == GPT_SEL_TRIGGER_1MS) */
#else
    #error "Timer A counter is not supported for current clock rate"
#endif /* (MCU_CFG_CLOCK_RATE == CLK_SEL_1MHZ) */


#if (GPT_CFG_TIMER_A_CLOCK_SOURCE == GPT_SEL_TACLK)
    #define GPT_CFG_TIMER_A_CLK_SRC_REG     TASSEL_0
#elif (GPT_CFG_TIMER_A_CLOCK_SOURCE == GPT_SEL_ACLK)
    #define GPT_CFG_TIMER_A_CLK_SRC_REG     TASSEL_1
#elif (GPT_CFG_TIMER_A_CLOCK_SOURCE == GPT_SEL_SMCLK)
    #define GPT_CFG_TIMER_A_CLK_SRC_REG     TASSEL_2
#else
    #error "Wrong GPT_CFG_TIMER_A_CLOCK_SOURCE configuration"
#endif /* (GPT_CFG_TIMER_A_CLOCK_SOURCE == GPT_SEL_TACLK) */


#if (GPT_CFG_TIMER_A_MODE == GPT_SEL_STOP_MODE)
    #define GPT_CFG_TIMER_A_MODE_REG        MC_0
#elif (GPT_CFG_TIMER_A_MODE == GPT_SEL_UP_MODE)
    #define GPT_CFG_TIMER_A_MODE_REG        MC_1
#elif (GPT_CFG_TIMER_A_MODE == GPT_SEL_CON_MODE)
    #define GPT_CFG_TIMER_A_MODE_REG        MC_2
#elif (GPT_CFG_TIMER_A_MODE == GPT_SEL_UP_DOWN_MODE)
    #define GPT_CFG_TIMER_A_MODE_REG        MC_3
#else
    #error "Wrong GPT_CFG_TIMER_A_CLOCK_SOURCE configuration"
#endif /* (GPT_CFG_TIMER_A_CLOCK_SOURCE == GPT_SEL_TACLK) */


#endif /* GPT_CFG_H */
