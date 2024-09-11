#ifndef BSW_CFG_H
#define BSW_CFG_H

#include <msp430.h>
#include "std_types.h"
#include "cfg_types.h"

/**
 * @brief UART driver enable
 */
#define UART_CFG_FUNCTION                   STD_DISABLED

/**
 * @brief Flash driver enable
 */
#define FLS_CFG_FUNCTION                    STD_DISABLED

/**
 * @brief GPIO driver enable
 */
#define GPIO_CFG_FUNCTION                   STD_DISABLED

#endif /* BSW_CFG_H */
