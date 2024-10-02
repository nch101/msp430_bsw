#include "uart.h"
#include "debug.h"

#if (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED)

static Debug_DebugMsgSt Debug_aMsgBuffer[DEBUG_CFG_MAX_BUFFER] = { 0U };
static uint8 Debug_u8WriteBufferIdx;
static uint8 Debug_u8ReadBufferIdx;
static uint8 Debug_u8AvailBuffer;
static Debug_JobType Debug_eJob;

void Debug_LogMessage( \
            const Debug_LevelType   eLevel,  \
            const Debug_ModuleName  eModule, \
            const uint16            u16Line, \
            const Debug_MessageID   eMessage,\
            const uint16            u16Num,  \
            const uint8             bIntArg  \
        )
{
    if (Debug_u8AvailBuffer > 0U)
    {
        /* Build debug message */
        Debug_aMsgBuffer[Debug_u8WriteBufferIdx].level  = eLevel;
        Debug_aMsgBuffer[Debug_u8WriteBufferIdx].module = eModule;
        Debug_aMsgBuffer[Debug_u8WriteBufferIdx].line   = u16Line;
        Debug_aMsgBuffer[Debug_u8WriteBufferIdx].msgID  = eMessage;
        Debug_aMsgBuffer[Debug_u8WriteBufferIdx].eol[0] = '\r';
        Debug_aMsgBuffer[Debug_u8WriteBufferIdx].eol[1] = '\n';
        Debug_aMsgBuffer[Debug_u8WriteBufferIdx].length = 8U;

        if (bIntArg == TRUE)
        {
            /* Build debug message with 1 uint16 argument */
            Debug_aMsgBuffer[Debug_u8WriteBufferIdx].arg    = u16Num;
            Debug_aMsgBuffer[Debug_u8WriteBufferIdx].length = 10U;
        }

        Debug_u8AvailBuffer--;
        Debug_u8WriteBufferIdx++;

        DEBUG_LIMIT_BUFFER_IDX(Debug_u8WriteBufferIdx, DEBUG_CFG_MAX_BUFFER);
    }
    else 
    {
        /* Queue is full. Abort */
    }
}

void Debug_MainFunction(void)
{
    uint8 bIsTriggerSucc;

    switch (Debug_eJob)
    {
        case DEBUG_JOB_IDLE:
        {
            if (Debug_u8AvailBuffer < DEBUG_CFG_MAX_BUFFER)
            {
                if (Debug_aMsgBuffer[Debug_u8ReadBufferIdx].length == 8U)
                {
                    bIsTriggerSucc = Uart_TransmitData((uint8*) &Debug_aMsgBuffer[Debug_u8ReadBufferIdx].msgID, Debug_aMsgBuffer[Debug_u8ReadBufferIdx].length);
                }
                else
                {
                    bIsTriggerSucc = Uart_TransmitData((uint8*) &Debug_aMsgBuffer[Debug_u8ReadBufferIdx], Debug_aMsgBuffer[Debug_u8ReadBufferIdx].length);
                }

                if (bIsTriggerSucc == STD_OK)
                {
                    Debug_eJob = DEBUG_JOB_SENDING;
                }
            }

            break;
        }

        case DEBUG_JOB_SENDING:
        {
            if (Uart_GetTransmissionStatus() == UART_IDLE)
            {
                Debug_eJob = DEBUG_JOB_IDLE;

                Debug_u8AvailBuffer++;
                Debug_u8ReadBufferIdx++;

                DEBUG_LIMIT_BUFFER_IDX(Debug_u8ReadBufferIdx, DEBUG_CFG_MAX_BUFFER);
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

void Debug_InitFunction(void)
{
    Debug_u8WriteBufferIdx    = 0U;
    Debug_u8ReadBufferIdx     = 0U;
    Debug_u8AvailBuffer       = DEBUG_CFG_MAX_BUFFER;
    Debug_eJob                = DEBUG_JOB_IDLE;
}

#endif /* (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED) */
