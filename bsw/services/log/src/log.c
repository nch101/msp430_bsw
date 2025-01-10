#include "nvm.h"
#include "log.h"

#if (BSW_CFG_LOG_FUNCTION == STD_ENABLED)

#define LOG_GET_NUM_ERRORS()                    Nvm_GetDataById(NVM_DTC_LOG, LOG_NUM_ERRORS_ID)

static uint8 Log_IsErrorCodePresent(const Log_ErrorId eErrorId)
{
    uint8   u8NumErrors = LOG_GET_NUM_ERRORS();

    for (uint8 u8Index = u8NumErrors; u8Index > 0; u8Index--)
    {
        if (Nvm_GetDataById(NVM_DTC_LOG, u8Index) == eErrorId)
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
    uint8   u8NumErrors = LOG_GET_NUM_ERRORS();

    if ((u8NumErrors < BSW_MAX_ERROR_CODE) \
     && (Log_IsErrorCodePresent(eErrorId) == FALSE))
    {
        /* Increase number of errors */
        u8NumErrors++;

        /* Save ErrorId into flash */
        Nvm_SetDataById(NVM_DTC_LOG, u8NumErrors, eErrorId);

        /* Save number of errors into flash */
        Nvm_SetDataById(NVM_DTC_LOG, LOG_NUM_ERRORS_ID, u8NumErrors);
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
    *pNumErrors = LOG_GET_NUM_ERRORS();

    for (uint8 u8Index = 0; u8Index < *pNumErrors; u8Index++)
    {
        pErrorOut[u8Index]  = Nvm_GetDataById(NVM_DTC_LOG, u8Index + 1U);
    }
}

/**
 * @brief       Clear all error codes in NvM
 * @retval      None
 */
void Log_ClearErrorCode(void)
{
    uint8   u8NumErrors = LOG_GET_NUM_ERRORS();

    for (uint8 u8Index = 1U; u8Index < u8NumErrors; u8Index++)
    {
        /* Save ErrorId into flash */
        Nvm_SetDataById(NVM_DTC_LOG, u8Index, LOG_CFG_DEFAULT_LOG_VALUE);
    }

    /* Save number of errors into flash */
    Nvm_SetDataById(NVM_DTC_LOG, LOG_NUM_ERRORS_ID, 0);
}
#endif /* (BSW_CFG_LOG_FUNCTION == STD_ENABLED) */
