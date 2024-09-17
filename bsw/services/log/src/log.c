#include "nvm.h"
#include "log.h"

#if (BSW_CFG_LOG_FUNCTION == STD_ENABLED)

/* Current number of errors */
static uint8 Log_u8NumErrors;

static uint8 Log_IsErrorCodePresent(Log_ErrorType eErrorId)
{
    uint8 u8Index;

    for (u8Index = Log_u8NumErrors; u8Index > 0; u8Index--)
    {
        if (Nvm_GetDataById(NVM_DTC_LOG, u8Index) == eErrorId)
        {
            return TRUE;
        }
    }

    return FALSE;
}

void Log_LogErrorCode(Log_ErrorType eErrorId)
{
    if ((Log_u8NumErrors == 0) \
     || (( Log_u8NumErrors < BSW_MAX_DTC_CODE ) && ( Log_IsErrorCodePresent(eErrorId) == FALSE )))
    {
        /* Increase number of errors */
        Log_u8NumErrors++;

        /* Save ErrorId into flash */
        Nvm_SetDataById(NVM_DTC_LOG, Log_u8NumErrors, &eErrorId);

        /* Save number of errors into flash */
        Nvm_SetDataById(NVM_DTC_LOG, LOG_NUM_ERRORS_ID, &Log_u8NumErrors);
    }
}

void Log_InitFunction(void)
{
    /* Get current number of errors in flash */
    Log_u8NumErrors = Nvm_GetDataById(NVM_DTC_LOG, LOG_NUM_ERRORS_ID);
}

#endif /* (BSW_CFG_LOG_FUNCTION == STD_ENABLED) */
