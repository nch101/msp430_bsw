#ifndef GPIO_CFG_H
#define GPIO_CFG_H

#include "bsw_cfg.h"

/**
 * @brief 
 */
#define GPIO_CFG_INTERRUPT_FUNCTION         STD_DISABLED

/**
 * @brief Max number of callback function to handle ISR in port 1
 */
#define GPIO_CFG_MAX_CALLBACK_FUNC1         2U

/**
 * @brief Max number of callback function to handle ISR in port 2
 */
#define GPIO_CFG_MAX_CALLBACK_FUNC2         2U

#endif /* GPIO_CFG_H */
