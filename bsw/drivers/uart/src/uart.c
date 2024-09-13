#include <string.h>
#include "uart.h"

#if (BSW_CFG_UART_FUNCTION == STD_ENABLED)

/* Rx buffer information */
static uint8 *          Uart_pRxBufferPtr;
static uint16           Uart_u16RxDataLength;
static uint16           Uart_u16RxDataIdx;

/* Tx buffer information */
static uint8 const *    Uart_pTxBufferPtr;
static uint16           Uart_u16TxDataLength;
static uint16           Uart_u16TxDataIdx;

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
        /* Enable USCI_A0 TX interrupt */
        IE2 |= UCA0TXIE;

        /* USCI_A0 TX buffer ready? */
        if (IFG2 & UCA0TXIFG)
        {
            UCA0TXBUF = Uart_pTxBufferPtr[Uart_u16TxDataIdx];
            Uart_u16TxDataIdx++;
        }
    }
    else
    {
        /* All data are transmitted. Set Uart status to IDLE */
        Uart_eTxStatus = UART_IDLE;
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
    /* Disable USCI_A0 TX interrupt */
    IE2     &= ~UCA0TXIE;
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
    /* Check if Rx buffer has space to store data */
    if (Uart_u16RxDataIdx < Uart_u16RxDataLength)
    {
        Uart_pRxBufferPtr[Uart_u16RxDataIdx] = UCA0RXBUF;
        Uart_u16RxDataIdx++;
    }
    else
    {
        /* Need to call Uart_ResetRxDataIndex() to reset Uart_u16RxDataIdx. Do nothing */
    }

    /* Clear Rx interrupt flag */
    IFG2    &= ~(UCA0RXIFG);
}

/**
 * @brief       Configures receive data buffer
 * @param[in]   pDataPtr        Pointer to receive data buffer
 * @param[in]   u8DataLength    Length of data
 * @retval      STD_NOT_OK - Config not successful, STD_OK - Config successful
 */
Std_StatusType Uart_ConfigReceiveDataBuffer(uint8 * const pDataPtr, const uint8 u8DataLength)
{
    if (pDataPtr != NULL)
    {
        Uart_pRxBufferPtr    = pDataPtr;
        Uart_u16RxDataLength = u8DataLength;
        Uart_u16RxDataIdx    = 0U;

        /* Enable USCI_A0 RX interrupt */
        IE2 |= UCA0RXIE;

        return STD_OK;
    }

    return STD_NOT_OK;
}

/**
 * @brief       Reset UART reception data index
 * @retval      UART_StatusType
 */
void Uart_ResetRxDataIndex(void)
{
    Uart_u16RxDataIdx = 0U;
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
 * @brief       Trigger data transmission. Data shall be transmit in task
 * @param[in]   pDataPtr        Pointer to buffer
 * @param[in]   u8DataLength    Length of data
 * @retval      STD_NOT_OK - Trigger not successful, STD_OK - Trigger successful
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
}

/**
 * @brief       UART main function
 * @retval      None
 */
void Uart_MainFunction(void)
{
    Uart_HandlingDataTransmission();
}

#endif /* (BSW_CFG_UART_FUNCTION == STD_ENABLED) */
