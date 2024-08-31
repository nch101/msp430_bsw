#ifndef UART_CFG_H
#define UART_CFG_H

#include "bsw_cfg.h"
#include "mcu_cfg.h"

/**
 * @brief UART Rx buffer length
 */
#define UART_CFG_RX_BUFF_LEN                8U

/**
 * @brief UART baudrate
 * @note  Only baudrates corresponding to the clock speed as shown below are supported
 *          9600        at  1MHz, 8MHz;
 *          19200       at  1MHz, 8MHz;
 *          115200      at  8MHz, 12Mhz;
 */
#define UART_CFG_BAUDRATE                   9600

/**
 * @brief UART parity configuration
 * 
*/
#define UART_CFG_PARITY                     STD_DISABLED

/**
 * @brief UART parity type configuration
 * @enum
 *          UART_SEL_EVEN_PARITY            Even parity
 *          UART_SEL_ODD_PARITY             Odd parity
*/
#define UART_CFG_PARITY_TYPE                UART_SEL_EVEN_PARITY

/**
 * @brief UART Controls the direction of the receive and transmit 
 * shift register
 * @enum
 *          UART_SEL_LSB_FIRST              LSB frist
 *          UART_SEL_MSB_FIRST              MSB frist
*/
#define UART_CFG_DIRECTION                  UART_SEL_LSB_FIRST

/**
 * @brief UART character length
 * @enum
 *          UART_SEL_7BIT_DATA              7 bit character length
 *          UART_SEL_8BIT_DATA              8 bit character length
*/
#define UART_CFG_CHARACTER_LENGTH           UART_SEL_8BIT_DATA

/**
 * @brief Stop bit select
 * @enum
 *          UART_SEL_1_STOP_BIT             1 stop bit
 *          UART_SEL_2_STOP_BIT             2 stop bits
*/
#define UART_CFG_STOP_BIT                   UART_SEL_1_STOP_BIT

/**
 * @brief USCI mode
 * @enum
 *          UART_SEL_UART_MODE              UART mode
 *          UART_SEL_ILDE_LINE_MULTI_MODE   Idle-line multiprocessor mode
 *          UART_SEL_ADDRESS_BIT_MULTI_MODE Address-bit multiprocessor mode
 *          UART_SEL_AUTO_MODE              UART mode with automatic baud rate detection
*/
#define UART_CFG_USCI_MODE                  UART_SEL_UART_MODE

/**
 * @brief Synchronous mode enable
 * @enum
 *          UART_SEL_ASYNC_MODE             Asynchronous mode
 *          UART_SEL_SYNC_MODE              Synchronous mode
*/
#define UART_CFG_SYNC_MODE                  UART_SEL_ASYNC_MODE

/**
 * @brief USCI clock source select
 * @enum
 *          UART_SEL_UCLK                   UCLK
 *          UART_SEL_ACLK                   ACLK
 *          UART_SEL_SMCLK                  SMCLK
 */
#define UART_CFG_CLOCK_SOURCE               UART_SEL_SMCLK

/**
 * @brief Receive erroneous-character interrupt-enable
 */
#define UART_CFG_RX_ERROR_ISR               STD_DISABLED

/**
 * @brief Receive break character interrupt-enable
 */
#define UART_CFG_RX_BREAK_ISR               STD_DISABLED


#if (UART_CFG_BAUDRATE == 9600)
    #define UART_CFG_MAX_DATA_TRANSFER              1U
    #if (MCU_CFG_CLOCK_RATE == CLK_SEL_1MHZ)
        #define UART_CFG_UCBR0_REG                  104U
        #define UART_CFG_UCBR1_REG                  0U
        #define UART_CFG_UCBRSx_REG                 (1U << 1U)
    #elif (MCU_CFG_CLOCK_RATE == CLK_SEL_8MHZ)
        #define UART_CFG_UCBR0_REG                  65U
        #define UART_CFG_UCBR1_REG                  3U
        #define UART_CFG_UCBRSx_REG                 (2U << 1U)
    #else
        #error "Current UART baudrate is not supported for current clock rate"
    #endif /* (MCU_CFG_CLOCK_RATE == CLK_SEL_1MHZ) */
#elif (UART_CFG_BAUDRATE == 19200)
    #define UART_CFG_MAX_DATA_TRANSFER              2U
    #if (MCU_CFG_CLOCK_RATE == CLK_SEL_1MHZ)
        #define UART_CFG_UCBR0_REG                  52U
        #define UART_CFG_UCBR1_REG                  0U
        #define UART_CFG_UCBRSx_REG                 (0U << 1U)
    #elif (MCU_CFG_CLOCK_RATE == CLK_SEL_8MHZ)
        #define UART_CFG_UCBR0_REG                  160U
        #define UART_CFG_UCBR1_REG                  1U
        #define UART_CFG_UCBRSx_REG                 (6U << 1U)
    #else
        #error "Current UART baudrate is not supported for current clock rate"
    #endif /* (MCU_CFG_CLOCK_RATE == CLK_SEL_1MHZ) */
#elif (UART_CFG_BAUDRATE == 115200)
    #define UART_CFG_MAX_DATA_TRANSFER              1U
    #if (MCU_CFG_CLOCK_RATE == CLK_SEL_8MHZ)
        #define UART_CFG_UCBR0_REG                  69U
        #define UART_CFG_UCBR1_REG                  0U
        #define UART_CFG_UCBRSx_REG                 (4U << 1U)
    #elif (MCU_CFG_CLOCK_RATE == CLK_SEL_12MHZ)
        #define UART_CFG_UCBR0_REG                  104U
        #define UART_CFG_UCBR1_REG                  0U
        #define UART_CFG_UCBRSx_REG                 (1U << 1U)
    #else
        #error "Current UART baudrate is not supported for current clock rate"
    #endif /* (MCU_CFG_CLOCK_RATE == CLK_SEL_8MHZ) */
#else
    #error "Current UART baudrate is not supported"
#endif


#if (UART_CFG_PARITY == STD_ENABLED)
    #if (UART_CFG_PARITY_TYPE == UART_SEL_EVEN_PARITY)
        #define UART_CFG_PARITY_REG                 (UCPEN | UCPAR)
    #else 
        #define UART_CFG_PARITY_REG                 (UCPEN)
    #endif /* (UART_CFG_PARITY_TYPE == UART_SEL_EVEN_PARITY) */
#else
    #define UART_CFG_PARITY_REG                 0x00
#endif /* (UART_CFG_PARITY == STD_ENABLED) */


#if (UART_CFG_DIRECTION == UART_SEL_MSB_FIRST)
    #define UART_CFG_DIRECTION_REG              (UCMSB)
#else
    #define UART_CFG_DIRECTION_REG              0x00
#endif /* (UART_CFG_DIRECTION == UART_SEL_MSB_FIRST) */


#if (UART_CFG_CHARACTER_LENGTH == UART_SEL_7BIT_DATA)
    #define UART_CFG_CHARACTER_LENGTH_REG       (UC7BIT)
#else
    #define UART_CFG_CHARACTER_LENGTH_REG       0x00
#endif /* (UART_CFG_CHARACTER_LENGTH == UART_SEL_7BIT_DATA) */


#if (UART_CFG_STOP_BIT == UART_SEL_2_STOP_BIT)
    #define UART_CFG_STOP_BIT_REG               (UCSPB)
#else
    #define UART_CFG_STOP_BIT_REG               0x00
#endif /* (UART_CFG_STOP_BIT == UART_SEL_2_STOP_BIT) */


#if (UART_CFG_USCI_MODE == UART_SEL_AUTO_MODE)
    #define UART_CFG_USCI_MODE_REG              (UCMODE0 | UCMODE1)
#elif (UART_CFG_USCI_MODE == UART_SEL_ILDE_LINE_MULTI_MODE)
    #define UART_CFG_USCI_MODE_REG              (UCMODE0)
#elif (UART_CFG_USCI_MODE == UART_SEL_ADDRESS_BIT_MULTI_MODE)
    #define UART_CFG_USCI_MODE_REG              (UCMODE1)
#else
    #define UART_CFG_USCI_MODE_REG              0x00
#endif /* (UART_CFG_USCI_MODE == UART_SEL_AUTO_MODE) */


#if (UART_CFG_SYNC_MODE == UART_SEL_SYNC_MODE)
    #define UART_CFG_SYNC_MODE_REG              (UCSYNC)
#else
    #define UART_CFG_SYNC_MODE_REG              0x00
#endif /* (UART_CFG_SYNC_MODE == UART_CFG_SYNC_MODE) */


#if (UART_CFG_CLOCK_SOURCE == UART_SEL_SMCLK)
    #define UART_CFG_CLK_SRC_REG                (UCSSEL1)
#elif (UART_CFG_CLOCK_SOURCE == UART_SEL_ACLK)
    #define UART_CFG_CLK_SRC_REG                (UCSSEL0)
#else
    #define UART_CFG_CLK_SRC_REG                0x00
#endif /* (UART_CFG_CLOCK_SOURCE == UART_SEL_SMCLK) */


#if (UART_CFG_RX_ERROR_ISR == STD_ENABLED)
    #define UART_CFG_RX_ERROR_ISR_REG           (UCRXEIE)
#else
    #define UART_CFG_RX_ERROR_ISR_REG           0x00
#endif


#if (UART_CFG_RX_BREAK_ISR == STD_ENABLED)
    #define UART_CFG_RX_BREAK_ISR_REG           (UCBRKIE)
#else
    #define UART_CFG_RX_BREAK_ISR_REG           0x00
#endif


/**
 * @brief USCI_A0 Control 0 Register
 */
#define UART_CFG_UCA0CTL0_REG                   UART_CFG_PARITY_REG | \
                                                UART_CFG_DIRECTION_REG | \
                                                UART_CFG_CHARACTER_LENGTH_REG | \
                                                UART_CFG_STOP_BIT_REG | \
                                                UART_CFG_USCI_MODE_REG | \
                                                UART_CFG_SYNC_MODE_REG

/**
 * @brief USCI_A0 Control 1 Register
 */
#define UART_CFG_UCA0CTL1_REG                   (((UART_CFG_CLK_SRC_REG | \
                                                UART_CFG_RX_ERROR_ISR_REG) | \
                                                UART_CFG_RX_BREAK_ISR_REG) & \
                                                ~UCSWRST)

#endif /* UART_CFG_H */
