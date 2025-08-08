#include "uart.h"
#include "timer.h"
#include "com.h"
#include "circular.h"

#if (BSW_CFG_COM_FUNCTION == STD_ENABLED)
#define COM_CORRECT_INDEX(curIdx, maxIdx)       curIdx = (curIdx >= maxIdx) ? 0 : curIdx

static Com_ServiceType  Com_sComAttribute[COM_MAX_SESSION_PRESENT];
static Com_StateType    Com_eCurrentRxState;
static Com_StateType    Com_eCurrentTxState;
static Com_SessionType  Com_eCurrentSession;

static uint8 Com_aRxBuffer[COM_CFG_MAX_RX_DATA_LEN];
static uint8 Com_u8RxIndex;

static Com_TxBufferType Com_aTxBuffer[COM_CFG_MAX_TX_BUFF];
static uint8 Com_u8AvailTxBuff;
static uint8 Com_u8TxBuffWriteIndex;
static uint8 Com_u8TxBuffReadIndex;

static void Com_OnReceiveDataEvent(volatile uint8 * pRxData)
{
    if (Com_u8RxIndex < COM_CFG_MAX_RX_DATA_LEN)
    {
        Timer_SetTimer(TIMER_RX_MSG_TIMEOUT, COM_CFG_RX_MSG_TIMEOUT);
        Com_aRxBuffer[Com_u8RxIndex] = *pRxData;
        Com_u8RxIndex++;
    }
}

static Std_BooleanType Com_DummyChecker(void)
{
    return STD_FALSE;
}

static void Com_ProcessDefaultComSession(void)
{
    /* Do nothing */
}

static void Com_SessionIdentifier(void)
{
    uint8 u8Index;

    if (Com_eCurrentSession == COM_DEFAULT_SESSION)
    {
        for (u8Index = 1U; u8Index < COM_MAX_SESSION_PRESENT; u8Index++)
        {
            if ((Com_sComAttribute[u8Index].eConditionChecker != NULL) \
             && (Com_sComAttribute[u8Index].eConditionChecker() == STD_TRUE))
            {
                Com_eCurrentSession = (Com_SessionType) u8Index;
                break;
            }
        }
    }
}

static void Com_SessionTimeoutChecker(void)
{
    Timer_StatusType    eTimerStatus;

    if (Com_eCurrentSession != COM_DEFAULT_SESSION)
    {
        eTimerStatus = Timer_GetTimerStatus(Com_sComAttribute[Com_eCurrentSession].eSessionTimerID);

        if (eTimerStatus == TIMER_STOP)
        {
            Timer_SetTimer(Com_sComAttribute[Com_eCurrentSession].eSessionTimerID, \
                                Com_sComAttribute[Com_eCurrentSession].u16SessionTimeout);
        }
        else if (eTimerStatus == TIMER_EXPIRED)
        {
            Com_ExitCurrentRxSession();
        }
        else
        {
            /* Do nothing */
        }
    }
}

static void Com_ProcessRxData(void)
{
    switch (Com_eCurrentRxState)
    {
        case COM_IDLE:
        {
            if (Com_u8RxIndex > 1U)
            {
                Com_eCurrentRxState = COM_RECEIVING;
            }

            break;
        }

        case COM_RECEIVING:
        {
            if (Com_sComAttribute[Com_eCurrentSession].eRxDataChecker() == STD_TRUE)
            {
                Com_eCurrentRxState = COM_COMPLETED;
            }

            if (Timer_GetTimerStatus(TIMER_RX_MSG_TIMEOUT) == TIMER_EXPIRED)
            {
                Com_eCurrentRxState = COM_TIMEOUT;
            }

            break;
        }

        case COM_TIMEOUT:
        {
            Com_u8RxIndex           = 0;
            Com_eCurrentRxState     = COM_IDLE;

            break;
        }

        case COM_COMPLETED:
        {
            /* Call service processor */
            Com_sComAttribute[Com_eCurrentSession].vSrvProcessor();
            Com_u8RxIndex           = 0;
            Com_eCurrentRxState     = COM_IDLE;

            break;
        }

        default:
        {
            /* Do nothing */
            break;
        }
    }
}

static void Com_ProcessTxData(void)
{
    switch (Com_eCurrentTxState)
    {
        case COM_IDLE:
        {
            if (Com_u8AvailTxBuff < COM_CFG_MAX_TX_BUFF)
            {
                (void) Uart_TransmitData(Com_aTxBuffer[Com_u8TxBuffReadIndex].aData, 
                        Com_aTxBuffer[Com_u8TxBuffReadIndex].u8Len);

                Com_eCurrentTxState   = COM_SENDING;
            }

            break;
        }

        case COM_SENDING:
        {
            if (Uart_GetTransmissionStatus() == UART_IDLE)
            {
                Com_u8AvailTxBuff++;
                Com_u8TxBuffReadIndex++;
                COM_CORRECT_INDEX(Com_u8TxBuffReadIndex, COM_CFG_MAX_TX_BUFF);
                Com_eCurrentTxState   = COM_IDLE;
            }

            break;
        }

        default:
        {
            /* Do nothing */
            break;
        }
    }
}

/**
 * @brief       Register a transmit task
 * @param[in]   pDataPtr        Pointer to buffer
 * @param[in]   u8DataLength    Length of data
 * @retval      STD_NOT_OK  - Trigger not successful. No Tx buffers available
 * @retval      STD_OK      - Trigger successful
 */
Std_StatusType Com_TransmitData(uint8 const * const pDataPtr, uint8 const u8DataLength)
{
    uint16 copiedDataLen;
    uint16 remainingDataLen     = u8DataLength;
    uint8  i;

    if ((Com_u8AvailTxBuff > 0) \
     && (remainingDataLen <= (Com_u8AvailTxBuff * COM_CFG_MAX_TX_DATA_LEN)))
    {
        do 
        {
            copiedDataLen    = (remainingDataLen < COM_CFG_MAX_TX_DATA_LEN) ? remainingDataLen : COM_CFG_MAX_TX_DATA_LEN;
            remainingDataLen -= copiedDataLen;

            Com_aTxBuffer[Com_u8TxBuffWriteIndex].u8Len = copiedDataLen;

            for (i = 0; i < copiedDataLen; i++)
            {
                Com_aTxBuffer[Com_u8TxBuffWriteIndex].aData[i] = pDataPtr[i];
            }

            Com_u8AvailTxBuff--;
            Com_u8TxBuffWriteIndex++;
            COM_CORRECT_INDEX(Com_u8TxBuffWriteIndex, COM_CFG_MAX_TX_BUFF);
        }
        while (remainingDataLen > 0U);

        return STD_OK;
    }
    else 
    {
        return STD_NOT_OK;
    }
}

/**
 * @brief       Get Rx data. This function shall pass address of Rx buffer to pDataOut
 * @param[out]  pDataOut    Pointer
 * @param[out]  pDataLen    Length of current data
 * @retval      None
 */
void Com_GetRxData(uint8** const pDataOut, uint8* const pDataLen)
{
    *pDataOut   = Com_aRxBuffer;
    *pDataLen   = Com_u8RxIndex + 1U;
}

/**
 * @brief       Changes current Rx session to default session
 * @retval      None
 */
void Com_ExitCurrentRxSession(void)
{
    Timer_StopTimer(Com_sComAttribute[Com_eCurrentSession].eSessionTimerID);

    Com_u8RxIndex       = 0;
    Com_eCurrentRxState = COM_IDLE;
    Com_eCurrentSession = COM_DEFAULT_SESSION;
}

/**
 * @brief       Register new Rx com session
 * @param[in]   sSessionID          SessionID
 * @param[in]   sSessionProcessor   Switching condition checker and session processor function
 * @retval      STD_OK      Create successfully
 * @retval      STD_NOT_OK  Create failed
 */
Std_StatusType Com_RegisterNewRxComSession(const Com_SessionType sSessionID, Com_ServiceType const * const sSessionProcessor)
{
    if (sSessionID < COM_MAX_SESSION_PRESENT)
    {
        Com_sComAttribute[sSessionID] = *sSessionProcessor;
        return STD_OK;
    }
    else 
    {
        return STD_NOT_OK;
    }
}

/**
 * @brief       Com main function
 * @retval      None
 */
void Com_MainFunction(void)
{
    Com_SessionIdentifier();
    Com_SessionTimeoutChecker();
    Com_ProcessRxData();
    Com_ProcessTxData();
}

/**
 * @brief       Com initialize function
 * @retval      None
 */
void Com_InitFunction(void)
{
    Uart_ConfigReceiveDataCallBack(Com_OnReceiveDataEvent);

    Com_sComAttribute[COM_DEFAULT_SESSION].eConditionChecker    = Com_DummyChecker;
    Com_sComAttribute[COM_DEFAULT_SESSION].eRxDataChecker       = Com_DummyChecker;
    Com_sComAttribute[COM_DEFAULT_SESSION].vSrvProcessor        = Com_ProcessDefaultComSession;

    Com_eCurrentRxState = COM_IDLE;
    Com_eCurrentTxState = COM_IDLE;
    Com_eCurrentSession = COM_DEFAULT_SESSION;
    Com_u8AvailTxBuff   = COM_CFG_MAX_TX_BUFF;
}
#endif /* (BSW_CFG_COM_FUNCTION == STD_ENABLED) */
