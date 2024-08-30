#ifndef CFG_TYPES_H
#define CFG_TYPES_H


/****************************************** GPT *******************************************/

/**
 * @brief GPT timer clock source enum configuration
*/
#define GPT_SEL_TACLK                       0
#define GPT_SEL_ACLK                        1
#define GPT_SEL_SMCLK                       2

/**
 * @brief GPT timer mode enum configuration
*/
#define GPT_SEL_STOP_MODE                   0
#define GPT_SEL_UP_MODE                     1
#define GPT_SEL_CON_MODE                    2
#define GPT_SEL_UP_DOWN_MODE                3

/**
 * @brief GPT trigger time enum configuration
*/
#define GPT_SEL_TRIGGER_0_5MS               0
#define GPT_SEL_TRIGGER_1MS                 1

/****************************************** Clock *******************************************/

/**
 * @brief Clock enum configuration
*/
#define CLK_SEL_1MHZ                        0
#define CLK_SEL_8MHZ                        1
#define CLK_SEL_12MHZ                       2
#define CLK_SEL_16MHZ                       3

/******************************************* UART *******************************************/

/**
 * @brief UART parity enum configuration
*/
#define UART_SEL_EVEN_PARITY                0
#define UART_SEL_ODD_PARITY                 1

/**
 * @brief UART direction of the receive and transmit shift register enum configuration
*/
#define UART_SEL_LSB_FIRST                  0
#define UART_SEL_MSB_FIRST                  1

/**
 * @brief Character length
*/
#define UART_SEL_8BIT_DATA                  0
#define UART_SEL_7BIT_DATA                  1

/**
 * @brief Number of stop bits 
*/
#define UART_SEL_1_STOP_BIT                 0
#define UART_SEL_2_STOP_BIT                 1

/**
 * @brief USCI mode
*/
#define UART_SEL_UART_MODE                  0   /* UART mode */
#define UART_SEL_ILDE_LINE_MULTI_MODE       1   /* Idle-line multiprocessor mode */
#define UART_SEL_ADDRESS_BIT_MULTI_MODE     2   /* Address-bit multiprocessor mode */
#define UART_SEL_AUTO_MODE                  3   /* UART mode with automatic baud rate detection */

/**
 * @brief Synchronous mode
*/
#define UART_SEL_ASYNC_MODE                 0   /* Asynchronous mode */
#define UART_SEL_SYNC_MODE                  1   /* Synchronous mode */

/**
 * @brief USCI clock source
 */
#define UART_SEL_UCLK                       0   /* UCLK */
#define UART_SEL_ACLK                       1   /* ACLK */
#define UART_SEL_SMCLK                      2   /* SMCLK */

/*********************************************************************************************/


#endif  /* CFG_TYPES_H */
