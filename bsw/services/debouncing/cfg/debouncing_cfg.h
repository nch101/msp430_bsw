#ifndef DEBOUNCING_CFG_H
#define DEBOUNCING_CFG_H

#include "bsw_cfg.h"

#if (BSW_CFG_DEBOUNCING_FUNCTION == STD_ENABLED)
    #if (BSW_CFG_TIMER_FUNCTION == STD_DISABLED)
        #error "Timer function must be enabled to use this function"
    #endif /* (BSW_CFG_TIMER_FUNCTION = STD_DISABLED) */

    #if (BSW_CFG_GPIO_FUNCTION == STD_DISABLED)
        #error "GPIO function must be enabled to use this function"
    #endif /* (BSW_CFG_GPIO_FUNCTION = STD_DISABLED) */

#define DEBOUNCE_CFG_DEBOUNCING_TIME        10

#endif /* (BSW_CFG_DEBOUNCING_FUNCTION == STD_ENABLED) */
#endif /* DEBOUNCING_CFG_H */
