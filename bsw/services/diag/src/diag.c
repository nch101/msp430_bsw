#include "gpio.h"
#include "debouncing.h"
#include "timer.h"
#include "nvm.h"
#include "nvm_dictionary.h"
#include "log.h"
#include "com.h"
#include "diag.h"
#include "diag_dictionary.h"

#if (BSW_CFG_DIAGNOSTIC_FUNCTION == STD_ENABLED)
static void Diag_BuildDefaultPositiveResponse(void);
static void Diag_BuildNegativeResponse(uint8 u8NRC);
static void Diag_ProcessRequestSequenceErrResponse(void);
static void Diag_ProcessSecurityAccessDeniedResponse(void);
static void Diag_ProcessSecurityAccess(void);
static void Diag_ProcessReadErrorCode(void);
static void Diag_ProcessClearErrorCode(void);
static void Diag_ProcessReadDataByID(void);
static void Diag_ProcessWriteDataByID(void);
static void Diag_ProcessCtrlErrCodeSettings(void);
static void Diag_ProcessEndDiagSession(void);
static void Diag_ProcessKeepAliveSession(void);
static void Diag_ProcessInOutControl(void);

static Diag_MsgStruct Diag_uTxBuffer;
static Diag_MsgStruct *Diag_uRxBuffer;

static Diag_SessionType     Diag_eCurrentSession;
static uint8                Diag_u8NumAttemps       = DIAG_CFG_MAX_NUMBER_OF_ATTEMPTS;

static const uint8 Diag_aServicesSupportList[DIAG_CFG_MAX_SERVICES] =
                        {
                            SECURITY_ACCESS,
                            KEEPALIVE_SESSION,
                            CONTROL_ERROR_CODE_SETTINGS,
                            END_DIAGNOSTIC_SESSION,
                            CLEAR_ERROR_CODE,
                            READ_ERROR_CODE,
                            READ_DATA_BY_ID,
                            WRITE_DATA_BY_ID,
                            INPUT_OUTPUT_CONTROL,
                        };

static const Diag_ServiceProcessorType Diag_aSecureServicesList[DIAG_CFG_MAX_SERVICES] =
                        {
                            Diag_ProcessSecurityAccess,
                            Diag_ProcessKeepAliveSession,
                            Diag_ProcessSecurityAccessDeniedResponse,
                            Diag_ProcessEndDiagSession,
                            Diag_ProcessSecurityAccessDeniedResponse,
                            Diag_ProcessReadErrorCode,
                            Diag_ProcessReadDataByID,
                            Diag_ProcessSecurityAccessDeniedResponse,
                            Diag_ProcessSecurityAccessDeniedResponse
                        };

static const Diag_ServiceProcessorType Diag_aFullAccessServicesList[DIAG_CFG_MAX_SERVICES] =
                        {
                            Diag_ProcessSecurityAccess,
                            Diag_ProcessKeepAliveSession,
                            Diag_ProcessCtrlErrCodeSettings,
                            Diag_ProcessEndDiagSession,
                            Diag_ProcessClearErrorCode,
                            Diag_ProcessReadErrorCode,
                            Diag_ProcessReadDataByID,
                            Diag_ProcessWriteDataByID,
                            Diag_ProcessInOutControl
                        };

static void Diag_BuildDefaultPositiveResponse(void)
{
    Diag_uTxBuffer.sField.u8Len         = 2U;
    Diag_uTxBuffer.sField.u8ServiceID   = Diag_uRxBuffer->sField.u8ServiceID + 0x40U;
}

static void Diag_BuildNegativeResponse(uint8 u8NRC)
{
    Diag_uTxBuffer.sField.u8Len         = 4U;
    Diag_uTxBuffer.sField.u8ServiceID   = NEGATIVE_RESPONSE;
    Diag_uTxBuffer.sField.u8Data[0]     = Diag_uRxBuffer->sField.u8ServiceID;
    Diag_uTxBuffer.sField.u8Data[1]     = u8NRC;
}

static void Diag_ProcessRequestSequenceErrResponse(void)
{
    Diag_BuildNegativeResponse(REQUEST_SEQUENCE_ERROR);
}

static void Diag_ProcessSecurityAccessDeniedResponse(void)
{
    Diag_BuildNegativeResponse(SECURITY_ACCESS_DENIED);
}

static void Diag_ProcessSecurityAccess(void)
{
    if (Diag_u8NumAttemps > 0)
    {
        if ((Diag_uRxBuffer->sField.u8Data[0] == Nvm_GetDataById(NVM_PROJECT_DATA, NVM_SEC_KEY_1_ID)) \
            && (Diag_uRxBuffer->sField.u8Data[1] == Nvm_GetDataById(NVM_PROJECT_DATA, NVM_SEC_KEY_2_ID)))
        {
            Timer_SetTimer(TIMER_SECURE_SESSION, DIAG_CFG_SECURE_SESSION_TIMEOUT);
            Diag_u8NumAttemps       = DIAG_CFG_MAX_NUMBER_OF_ATTEMPTS;
            Diag_eCurrentSession    = DIAG_FULL_ACCESS_SESSION;
            Diag_BuildDefaultPositiveResponse();
        }
        else
        {
            Diag_u8NumAttemps--;
            Diag_BuildNegativeResponse(INVALID_KEY);
        }
    }
    else
    {
        Diag_BuildNegativeResponse(EXCEEDED_NUMBER_OF_ATTEMPTS);
    }
}

static void Diag_ProcessReadErrorCode(void)
{
    uint8 u8NumErrors;

    Log_ReadErrorCode(&u8NumErrors, Diag_uRxBuffer->sField.u8Data);

    Diag_uTxBuffer.sField.u8ServiceID   = Diag_uRxBuffer->sField.u8ServiceID + 0x40U;
    Diag_uTxBuffer.sField.u8Len         = u8NumErrors + 2U;
}

static void Diag_ProcessClearErrorCode(void)
{
    Log_ClearErrorCode();
    Diag_BuildDefaultPositiveResponse();
}

static void Diag_ProcessReadDataByID(void)
{
    if (Diag_uRxBuffer->sField.u8Data[0] == (uint8)NVM_PROJECT_DATA)
    {
        Diag_uTxBuffer.sField.u8Data[0] = Diag_uRxBuffer->sField.u8Data[0];
        Diag_uTxBuffer.sField.u8Data[0] = Nvm_GetDataById(NVM_PROJECT_DATA, Diag_uRxBuffer->sField.u8Data[1]);
    }
    else
    {
        Diag_BuildNegativeResponse(SUB_FUNCTION_NOT_SUPPORTED);
    }
}

static void Diag_ProcessWriteDataByID(void)
{
    if (Diag_uRxBuffer->sField.u8Data[0] == (uint8)NVM_PROJECT_DATA)
    {
        Nvm_SetDataById(NVM_PROJECT_DATA, Diag_uRxBuffer->sField.u8Data[1], Diag_uRxBuffer->sField.u8Data[2]);
        Diag_BuildDefaultPositiveResponse();
    }
    else
    {
        Diag_BuildNegativeResponse(SUB_FUNCTION_NOT_SUPPORTED);
    }
}

static void Diag_ProcessCtrlErrCodeSettings(void)
{
    Diag_BuildNegativeResponse(SERVICE_NOT_SUPPORTED);
}

static void Diag_ProcessEndDiagSession(void)
{
    Diag_eCurrentSession = DIAG_SECURE_SESSION;
    Diag_BuildDefaultPositiveResponse();
    Com_ExitCurrentRxSession();
}

static void Diag_ProcessKeepAliveSession(void)
{
    Timer_SetTimer(TIMER_SECURE_SESSION, DIAG_CFG_SECURE_SESSION_TIMEOUT);
    Timer_SetTimer(TIMER_DIAGNOSTIC_SESSION, DIAG_CFG_DIAGNOSTIC_SESSION_TIMEOUT);
    Diag_BuildDefaultPositiveResponse();
}

static void Diag_ProcessInOutControl(void)
{
    /* TODO: To be implemented */
    Diag_BuildNegativeResponse(SERVICE_NOT_SUPPORTED);
}

Std_BooleanType Diag_DiagConditionChecker(void)
{
    Timer_StatusType    eTimerStatus;
    eTimerStatus        = Timer_GetTimerStatus(TIMER_START_DIAG_SESSION);

    if (Debounce_ReadDeviceState(DEBOUNCE_DIAGNOSTIC_MODE) == GPIO_LOW)
    {
        if (eTimerStatus == TIMER_STOP)
        {
            Timer_SetTimer(TIMER_START_DIAG_SESSION, DIAG_CFG_ENTERING_DIAG_SESSION_TIMEOUT);
        }
        else if (eTimerStatus == TIMER_EXPIRED)
        {
            return STD_TRUE;
        }
        else 
        {
            /* Do nothing */
        }
    }
    else 
    {
        Timer_StopTimer(TIMER_START_DIAG_SESSION);
    }

    return STD_FALSE;
}

Std_BooleanType Diag_DiagFrameMessageChecker(void)
{
    uint8   u8CurLen;

    Com_GetRxData((uint8**) &Diag_uRxBuffer, &u8CurLen);

    if (Diag_uRxBuffer->sField.u8Len == u8CurLen)
    {
        return STD_TRUE;
    }

    return STD_FALSE;
}

void Diag_ProcessDiagService(void)
{
    uint8 u8ServiceIndex;

    if (Timer_GetTimerStatus(TIMER_SECURE_SESSION) != TIMER_RUNNING)
    {
        Diag_eCurrentSession = DIAG_SECURE_SESSION;
    }

    for (u8ServiceIndex = 0; u8ServiceIndex < DIAG_CFG_MAX_SERVICES; u8ServiceIndex++)
    {
        if (Diag_uRxBuffer->sField.u8ServiceID == Diag_aServicesSupportList[u8ServiceIndex])
        {
            break;
        }
    }

    if (u8ServiceIndex < DIAG_CFG_MAX_SERVICES)
    {
        switch (Diag_eCurrentSession)
        {
            case DIAG_SECURE_SESSION:
            {
                Diag_aSecureServicesList[u8ServiceIndex]();
                break;
            }

            case DIAG_FULL_ACCESS_SESSION:
            {
                Diag_aFullAccessServicesList[u8ServiceIndex]();
                break;
            }

            default:
            {
                Diag_ProcessRequestSequenceErrResponse();
                break;
            }
        }
    }
    else 
    {
        Diag_BuildNegativeResponse(SERVICE_NOT_SUPPORTED);
    }

    (void) Com_TransmitData((uint8*) &Diag_uTxBuffer, Diag_uTxBuffer.sField.u8Len);
}

void Diag_InitFunction(void)
{
    Gpio_GpioCfgType    button13;
    Com_ServiceType     sDiagService;

    button13.eGroup     = GPIO_GROUP_1;
    button13.ePin       = GPIO_PIN_3;
    button13.eDirection = GPIO_INPUT;
    button13.ePull      = GPIO_PULL_UP;
    Debounce_ConfigureDevice(DEBOUNCE_DIAGNOSTIC_MODE, &button13);

    sDiagService.eConditionChecker  = Diag_DiagConditionChecker;
    sDiagService.eRxDataChecker     = Diag_DiagFrameMessageChecker;
    sDiagService.vSrvProcessor      = Diag_ProcessDiagService;
    sDiagService.eSessionTimerID    = TIMER_DIAGNOSTIC_SESSION;
    sDiagService.u16SessionTimeout  = DIAG_CFG_DIAGNOSTIC_SESSION_TIMEOUT;
    Com_RegisterNewRxComSession(COM_DIAGNOSTIC_SESSION, &sDiagService);
}

#endif /* (BSW_CFG_DIAGNOSTIC_FUNCTION == STD_ENABLED) */
