#ifndef BUTTON_CFG_H
#define BUTTON_CFG_H

#include "bsw_cfg.h"

#if (BSW_CFG_BUTTON_FUNCTION == STD_ENABLED)
    #if (BSW_CFG_TIMER_FUNCTION == STD_DISABLED)
        #error "Must enable timer function to use this button function"
    #endif /* (BSW_CFG_TIMER_FUNCTION = STD_DISABLED) */

    #if (BSW_CFG_GPIO_FUNCTION == STD_DISABLED)
        #error "Must enable GPIO function to use this button function"
    #endif /* (BSW_CFG_GPIO_FUNCTION = STD_DISABLED) */

#define BUTTON_CFG_DEBOUNCING_TIME              50

#endif /* (BSW_CFG_BUTTON_FUNCTION == STD_ENABLED) */
#endif /* BUTTON_CFG_H */
