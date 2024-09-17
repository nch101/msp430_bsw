#include "uart.h"
#include "debug.h"

#if (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED)
#include "printf.h"

static char Debug_aMsgBuffer[DEBUG_CFG_MAX_BUFFER][DEBUG_CFG_MAX_BUFFER_LEN] = { 0U };
static uint8 Debug_aLength[DEBUG_CFG_MAX_BUFFER] = { 0U };
static uint8 Debug_u8WriteBufferIdx;
static uint8 Debug_u8ReadBufferIdx;
static uint8 Debug_u8AvailBuffer;
static Debug_JobType Debug_eJob;

void Debug_LogMessage( \
            const Debug_LevelType   eLevel,  \
            const Debug_ModuleName  eModule, \
            const uint16            u16Line, \
            const Debug_MessageID   eMessage,\
            const uint16            u16Num   \
        )
{
    if (Debug_u8AvailBuffer > 0U)
    {
        if (u16Num == 0xFFU)
        {
            /* Build debug message */
            Debug_aLength[Debug_u8WriteBufferIdx] = snprintf( \
                                                        Debug_aMsgBuffer[Debug_u8WriteBufferIdx],   \
                                                        DEBUG_CFG_MAX_BUFFER_LEN,                   \
                                                        "%d:%d:%d:%d\r\n",                          \
                                                        eLevel, eModule, u16Line, eMessage);
        }
        else 
        {
            /* Build debug message with 1 uint16 argument */
            Debug_aLength[Debug_u8WriteBufferIdx] = snprintf( \
                                                        Debug_aMsgBuffer[Debug_u8WriteBufferIdx],   \
                                                        DEBUG_CFG_MAX_BUFFER_LEN,                   \
                                                        "%d:%d:%d:%d:%d\r\n",                       \
                                                        eLevel, eModule, u16Line, eMessage, u16Num);
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
                bIsTriggerSucc = Uart_TransmitData((uint8*) Debug_aMsgBuffer[Debug_u8ReadBufferIdx], Debug_aLength[Debug_u8ReadBufferIdx]);

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
