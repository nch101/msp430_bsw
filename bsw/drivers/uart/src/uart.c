#include <string.h>
#include "uart.h"


/* Rx buffer information */
static uint8 *          UART_gpRxBuffer;
static uint16           UART_gRxDataLength;
static uint16           UART_gRxCurDataIdx;

/* Tx buffer information */
static uint8 const *    UART_gpTxBuffer;
static uint16           UART_gTxDataLength;
static uint16           UART_gTxCurDataIdx;

/* Transmission status */
static UART_StatusType  UART_gTxStatus;

/**
 * @brief       Handles data transmission
 * @details     The function shall enable Tx interrupt to send 1 char type data.
 *              Tx interrupt shall disable in USCI0TX_ISR function.
 * @retval      None
 */
static void UART_HandlingDataTransmission(void)
{
    if (UART_gTxCurDataIdx < UART_gTxDataLength)
    {
        /* Enable USCI_A0 TX interrupt */
        IE2 |= UCA0TXIE;

        /* USCI_A0 TX buffer ready? */
        if (IFG2 & UCA0TXIFG)
        {
            UCA0TXBUF = UART_gpTxBuffer[UART_gTxCurDataIdx];
            UART_gTxCurDataIdx++;
        }
    }
    else
    {
        /* All data are transmitted. Set Uart status to IDLE */
        UART_gTxStatus = UART_IDLE;
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
    if (UART_gRxCurDataIdx < UART_gRxDataLength)
    {
        UART_gpRxBuffer[UART_gRxCurDataIdx] = UCA0RXBUF;
        UART_gRxCurDataIdx++;
    }
    else
    {
        /* Need to call UART_ResetRxDataIndex() to reset UART_gRxCurDataIdx. Do nothing */
    }

    /* Clear Rx interrupt flag */
    IFG2    &= ~(UCA0RXIFG);
}

/**
 * @brief       Receive data buffer configuration function
 * @param[in]   DataPtr Pointer to receive data buffer
 * @retval      STD_NOT_OK - Config not successful, STD_OK - Config successful
 */
Std_StatusType UART_ConfigReceiveDataBuffer(uint8 * DataPtr, const uint8 DataLength)
{
    if (DataPtr != NULL)
    {
        UART_gpRxBuffer     = DataPtr;
        UART_gRxDataLength  = DataLength;
        UART_gRxCurDataIdx  = 0U;

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
void UART_ResetRxDataIndex(void)
{
    UART_gRxCurDataIdx = 0U;
}

/**
 * @brief       Get UART Transmission status
 * @retval      UART_StatusType
 */
UART_StatusType UART_GetTransmissionStatus(void)
{
    return UART_gTxStatus;
}

/**
 * @brief       Trigger data transmission. Data shall be transmit in task
 * @param[in]   DataPtr Pointer to buffer
 * @param[in]   Length  Length of data
 * @retval      STD_NOT_OK - Trigger not successful, STD_OK - Trigger successful
 */
Std_StatusType UART_TransmitData(uint8 const * DataPtr, const uint8 DataLength)
{
    if ((DataPtr != NULL) \
    && (UART_gTxStatus == UART_IDLE))
    {
        UART_gpTxBuffer     = DataPtr;
        UART_gTxDataLength  = DataLength;
        UART_gTxCurDataIdx  = 0U;
        UART_gTxStatus      = UART_BUSY;

        return STD_OK;
    }

    return STD_NOT_OK;
}

/**
 * @brief       UART initialize function
 * @retval      None
 */
void UART_InitFunction(void)
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
void UART_MainFunction(void)
{
    UART_HandlingDataTransmission();
}
