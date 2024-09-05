#include <string.h>
#include "uart.h"

/* Tx buffer information */
static Uart_UartTxBuff_St   Uart_gTxBuffInf;
/* Rx buffer information */
static Uart_UartRxBuff_St   Uart_gRxBuffInf;
/* Transmission status */
static Uart_StatusType      Uart_gTxStatus;
/* Internal Rx buffer */
static uint8                Uart_gRxBuff[UART_CFG_RX_BUFF_LEN];

/**
 * @brief       Push received data to Rx buffer 
 * @param[in]   DataPtr Pointer to RxData
 * @retval      None
 */
static void Uart_PushDataToRxBuff(uint8 const * rxData)
{
    if (Uart_gRxBuffInf.status != UART_BUFFER_READY)
    {
        Uart_gRxBuffInf.buffPtr[Uart_gRxBuffInf.curIndex] = *rxData;
        Uart_gRxBuffInf.curIndex++;

        if (Uart_gRxBuffInf.curIndex >= UART_CFG_RX_BUFF_LEN)
        {
            /* No more space. Set Rx buffer status to READY */
            Uart_gRxBuffInf.status = UART_BUFFER_READY;
        }
    }
}

/**
 * @brief       Handles data transmission
 * @details     The function shall enable Tx interrupt to send 1 char type data.
 *              Tx interrupt shall disable in USCI0TX_ISR function.
 * @retval      None
 */
static void Uart_HandlingDataTransmission(void)
{
    if ( Uart_gTxBuffInf.curIndex < Uart_gTxBuffInf.length )
    {
        /* Enable USCI_A0 TX interrupt */
        IE2 |= UCA0TXIE;

        /* USCI_A0 TX buffer ready? */
        if (IFG2 & UCA0TXIFG)
        {
            UCA0TXBUF = Uart_gTxBuffInf.buffPtr[Uart_gTxBuffInf.curIndex];
            Uart_gTxBuffInf.curIndex++;
        }
    }
    else
    {
        /* All data are transmitted. Set Uart status to IDLE */
        Uart_gTxStatus = UART_IDLE;
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
    Uart_PushDataToRxBuff( (uint8*) &UCA0RXBUF );

    /* Clear Rx interrupt flag */
    IFG2    &= ~(UCA0RXIFG);
}

/**
 * @brief       Get UART Transmission status
 * @retval      Uart_StatusType
 */
Uart_StatusType UART_GetTransmissionStatus(void)
{
    return Uart_gTxStatus;
}

/**
 * @brief       Get received data
 * @param[out]  DataPtr Pointer to get data buffer
 * @retval      STD_NOT_OK - Get not successful, STD_OK - get successful
 */
Std_StatusType UART_GetReceivedData(uint8 * DataPtr)
{
    if (Uart_gRxBuffInf.status == UART_BUFFER_READY)
    {
        memcpy(DataPtr, Uart_gRxBuffInf.buffPtr, UART_CFG_RX_BUFF_LEN);

        Uart_gRxBuffInf.curIndex = 0U;
        Uart_gRxBuffInf.status   = UART_BUFFER_EMPTY;

        return STD_OK;
    }
    else
    {
        /* Rx buffer is not ready yet */
        return STD_NOT_OK;
    }
}

/**
 * @brief       Trigger data transmission. Data shall be transmit in task
 * @param[in]   DataPtr Pointer to buffer
 * @param[in]   Length  Length of data
 * @retval      STD_NOT_OK - Trigger not successful, STD_OK - Trigger successful
 */
Std_StatusType UART_TransmitData(uint8 const * DataPtr, const uint8 Length)
{
    if ((DataPtr != NULL) \
    && (Uart_gTxStatus == UART_IDLE))
    {
        Uart_gTxBuffInf = (Uart_UartTxBuff_St) {DataPtr, Length, 0U};
        Uart_gTxStatus  = UART_BUSY;

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

    /* Enable USCI_A0 RX interrupt */
    IE2 |= UCA0RXIE;

    Uart_gRxBuffInf = (Uart_UartRxBuff_St) {Uart_gRxBuff, 0U, UART_BUFFER_EMPTY};
}

/**
 * @brief       UART main function
 * @retval      None
 */
void UART_MainFunction(void)
{
    Uart_HandlingDataTransmission();
}
