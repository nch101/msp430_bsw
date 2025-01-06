#ifndef BSW_CFG_H
#define BSW_CFG_H

#include <msp430.h>
#include "std_types.h"
#include "cfg_types.h"

/**
 * @brief UART driver enable
 */
#define BSW_CFG_UART_FUNCTION               STD_DISABLED

/**
 * @brief Flash driver enable
 */
#define BSW_CFG_FLS_FUNCTION                STD_DISABLED

/**
 * @brief GPIO driver enable
 */
#define BSW_CFG_GPIO_FUNCTION               STD_ENABLED

/**
 * @brief NVM service enable
 */
#define BSW_CFG_NVM_FUNCTION                STD_DISABLED

/**
 * @brief Debug service enable
 */
#define BSW_CFG_DEBUG_FUNCTION              STD_DISABLED

/**
 * @brief Log service enable
 */
#define BSW_CFG_LOG_FUNCTION                STD_DISABLED

/**
 * @brief Timer service enable
 */
#define BSW_CFG_TIMER_FUNCTION              STD_DISABLED

/**
 * @brief Debouncing service enable
 */
#define BSW_CFG_DEBOUNCING_FUNCTION         STD_DISABLED

/**
 * @brief Communication service enable
 */
#define BSW_CFG_COM_FUNCTION                STD_DISABLED

/**
 * @brief Max number of project configurations
 */
#define BSW_MAX_PROJECT_CONFIG              5U

/**
 * @brief Max number of error codes
 */
#define BSW_MAX_ERROR_CODE                  5U

#endif /* BSW_CFG_H */
