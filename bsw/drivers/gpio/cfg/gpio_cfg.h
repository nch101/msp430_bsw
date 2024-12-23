#ifndef GPIO_CFG_H
#define GPIO_CFG_H

#include "bsw_cfg.h"

#if (BSW_CFG_GPIO_FUNCTION == STD_ENABLED)
/**
 * @brief GPIO interrupt function
 */
#define GPIO_CFG_INTERRUPT_FUNCTION         STD_DISABLED

/**
 * @brief Max number of callback function to handle ISR in each port
 */
#define GPIO_CFG_MAX_CALLBACK_FN            2U

#endif /* (BSW_CFG_GPIO_FUNCTION == STD_ENABLED) */
#endif /* GPIO_CFG_H */
