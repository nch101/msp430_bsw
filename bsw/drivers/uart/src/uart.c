#include "uart.h"

#if (BSW_CFG_UART_FUNCTION == STD_ENABLED)
#define ENABLE_TX_INTERRUPT()               IE2  |= UCA0TXIE
#define DISABLE_TX_INTERRUPT()              IE2  &= ~UCA0TXIE

#define ENABLE_RX_INTERRUPT()               IE2  |= UCA0RXIE
#define CLEAR_RX_INTERRUPT_FLAG()           IFG2 &= ~(UCA0RXIFG)

/* Rx information */
static void          (*Uart_pReceiveDataFnCallback)(volatile uint8 *);

/* Tx information */
static uint8 const * Uart_pTxBufferPtr;
static uint16        Uart_u16TxDataLength;
static uint16        Uart_u16TxDataIdx;

/* Transmission status */
static Uart_StatusType  Uart_eTxStatus;

/**
 * @brief       Handles data transmission
 * @details     The function shall enable Tx interrupt to send 1 char type data.
 *              Tx interrupt shall disable in USCI0TX_ISR function.
 * @retval      None
 */
static void Uart_HandlingDataTransmission(void)
{
    if (Uart_u16TxDataIdx < Uart_u16TxDataLength)
    {
        UCA0TXBUF = Uart_pTxBufferPtr[Uart_u16TxDataIdx];
        Uart_u16TxDataIdx++;
    }
    else
    {
        /* All data are transmitted. Set Uart status to IDLE */
        Uart_eTxStatus = UART_IDLE;
        DISABLE_TX_INTERRUPT();
    }
}

/**
 * @brief       UART Tx interrupt handler function
 * @retval      None
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    Uart_HandlingDataTransmission();
}

/**
 * @brief       UART Rx interrupt handler function
 * @retval      None
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if (Uart_pReceiveDataFnCallback != NULL)
    {
        (*Uart_pReceiveDataFnCallback)(&UCA0RXBUF);
    }

    CLEAR_RX_INTERRUPT_FLAG();
}

/**
 * @brief       Configures receive data function callback
 * @param[in]   vFuncCallBack       Pointer to function callback
 * @retval      None
 */
void Uart_ConfigReceiveDataCallBack(void (*vFuncCallBack)(volatile uint8 *))
{
    if (vFuncCallBack != NULL)
    {
        Uart_pReceiveDataFnCallback = vFuncCallBack;
        ENABLE_RX_INTERRUPT();
    }
}

/**
 * @brief       Get UART Transmission status
 * @retval      UART_StatusType
 */
Uart_StatusType Uart_GetTransmissionStatus(void)
{
    return Uart_eTxStatus;
}

/**
 * @brief       Trigger data transmission. Data shall be transmit in Tx interrupt
 * @param[in]   pDataPtr        Pointer to buffer
 * @param[in]   u8DataLength    Length of data
 * @retval      STD_NOT_OK - Trigger not successful
 * @retval      STD_OK - Trigger successful
 */
Std_StatusType Uart_TransmitData(uint8 const * const pDataPtr, const uint8 u8DataLength)
{
    if ((pDataPtr != NULL) \
    && (Uart_eTxStatus == UART_IDLE))
    {
        Uart_pTxBufferPtr    = pDataPtr;
        Uart_u16TxDataLength = u8DataLength;
        Uart_u16TxDataIdx    = 0U;
        Uart_eTxStatus       = UART_BUSY;

        ENABLE_TX_INTERRUPT();
        Uart_HandlingDataTransmission();

        return STD_OK;
    }

    return STD_NOT_OK;
}

/**
 * @brief       UART initialize function
 * @retval      None
 */
void Uart_InitFunction(void)
{
    /* Set P1.1 = RXD, P1.2 = TXD */
    P1SEL   = BIT1 + BIT2;
    P1SEL2  = BIT1 + BIT2;

    /* Configure UART */
    UCA0CTL0    = UART_CFG_UCA0CTL0_REG;
    UCA0CTL1    = UART_CFG_UCA0CTL1_REG;

    /* Configure UART baud rate */
    UCA0BR0     = UART_CFG_UCBR0_REG;
    UCA0BR1     = UART_CFG_UCBR1_REG;
    UCA0MCTL    = UART_CFG_UCBRSx_REG;

    /* Initialize internal variables */
    Uart_pReceiveDataFnCallback = NULL;
    Uart_pTxBufferPtr           = NULL;
    Uart_u16TxDataLength        = 0U;
    Uart_u16TxDataIdx           = 0U;
    Uart_eTxStatus              = UART_IDLE;
}

#endif /* (BSW_CFG_UART_FUNCTION == STD_ENABLED) */
