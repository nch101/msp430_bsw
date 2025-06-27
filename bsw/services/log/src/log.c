#include "nvm.h"
#include "log.h"

#if (BSW_CFG_LOG_FUNCTION == STD_ENABLED)
static uint8 Log_IsErrorCodePresent(const Log_ErrorId eErrorId)
{
    uint8   u8NumErrors;
    uint8   u8ErrorID;

    Nvm_GetDataById(NVM_EVENT_LOG, LOG_NUM_ERRORS_ID, &u8NumErrors);

    for (uint8 u8Index = u8NumErrors; u8Index > 0; u8Index--)
    {
        Nvm_GetDataById(NVM_EVENT_LOG, u8Index, &u8ErrorID);

        if (u8ErrorID == eErrorId)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @brief       Save error code to NvM
 * @param[in]   eErrorId    Error code ID
 * @retval      None
 */
void Log_LogErrorCode(const Log_ErrorId eErrorId)
{
    uint8   u8NumErrors;

    Nvm_GetDataById(NVM_EVENT_LOG, LOG_NUM_ERRORS_ID, &u8NumErrors);

    if ((u8NumErrors < BSW_MAX_EVENT_LOG) \
     && (Log_IsErrorCodePresent(eErrorId) == FALSE))
    {
        /* Increase number of errors */
        u8NumErrors++;

        /* Save ErrorId into flash */
        Nvm_SetDataById(NVM_EVENT_LOG, u8NumErrors, &eErrorId);

        /* Save number of errors into flash */
        Nvm_SetDataById(NVM_EVENT_LOG, LOG_NUM_ERRORS_ID, &u8NumErrors);
    }
}

/**
 * @brief       Get all error codes 
 * @param[out]  pNumErrors  Pointer to variable holding error number
 * @param[out]  pErrorOut   Pointer to buffer
 * @retval      None
 */
void Log_ReadErrorCode(uint8 * const pNumErrors, uint8 * pErrorOut)
{
    Nvm_GetDataById(NVM_EVENT_LOG, LOG_NUM_ERRORS_ID, pNumErrors);

    for (uint8 u8Index = 0; u8Index < *pNumErrors; u8Index++)
    {
        Nvm_GetDataById(NVM_EVENT_LOG, u8Index + 1U, &pErrorOut[u8Index]);
    }
}

/**
 * @brief       Clear all error codes in NvM
 * @retval      None
 */
void Log_ClearErrorCode(void)
{
    uint8   u8NumErrors;
    uint8   u8DefaultData;

    Nvm_GetDataById(NVM_EVENT_LOG, LOG_NUM_ERRORS_ID, &u8NumErrors);

    for (uint8 u8Index = 1U; u8Index < u8NumErrors; u8Index++)
    {
        /* Save ErrorId into flash */
        u8DefaultData = LOG_CFG_DEFAULT_LOG_VALUE;
        Nvm_SetDataById(NVM_EVENT_LOG, u8Index, &u8DefaultData);
    }

    /* Save number of errors into flash */
    u8DefaultData = 0;
    Nvm_SetDataById(NVM_EVENT_LOG, LOG_NUM_ERRORS_ID, &u8DefaultData);
}
#endif /* (BSW_CFG_LOG_FUNCTION == STD_ENABLED) */
