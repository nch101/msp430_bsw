#include "fls.h"
#include "nvm.h"

#if (BSW_CFG_NVM_FUNCTION == STD_ENABLED)

static uint8 Nvm_aProjConfigMirror[BSW_MAX_PROJECT_CONFIG]  = { 0U };
static uint8 Nvm_aDTCMirror[BSW_MAX_ERROR_CODE]             = { 0U };

static Nvm_DataType Nvm_eBlockDataDoing = NVM_NONE;
static Nvm_JobType  Nvm_eJob;
static Nvm_JobType  Nvm_ePreJob;

static uint8        Nvm_bIsDataReady    = FALSE;
static uint8        Nvm_bIsPrjDatChange = FALSE;
static uint8        Nvm_bIsDTCChange    = FALSE;

/**
 * @brief       Change Ongoing Job to current job
 * @param[in]   eJob        Current job
 * @retval      None
 */
static void Nvm_ChangeCurJobDoing(const Nvm_JobType eJob)
{
    Nvm_ePreJob = Nvm_eJob;
    Nvm_eJob    = eJob;
}

/**
 * @brief       Read all Nvm data segment then copy to RAM mirror
 * @retval      None
 */
static void Nvm_ReadAllNvmBlock(void)
{
    switch (Nvm_eBlockDataDoing)
    {
        case NVM_PROJECT_DATA:
        {
            if (Fls_Read(NVM_CFG_PROJ_CONFIG_SEGMENT, Nvm_aProjConfigMirror, BSW_MAX_PROJECT_CONFIG) == STD_OK)
            {
                NVM_READING_FLASH_DATA();

                Nvm_eBlockDataDoing = NVM_DTC_LOG;
            }

            break;
        }

        case NVM_DTC_LOG:
        {
            if (Fls_Read(NVM_CFG_DTC_LOG_SEGMENT, Nvm_aDTCMirror, BSW_MAX_ERROR_CODE) == STD_OK)
            {
                NVM_READING_FLASH_DATA();

                Nvm_eBlockDataDoing = NVM_NONE;
                Nvm_bIsDataReady    = TRUE;
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
 * @brief       Write data from RAM mirror to NvM data segment
 * @retval      None
 */
static void Nvm_WriteNvmBlock(void)
{
    Fls_SegmentType eTargetSegment;
    uint8*          pSourcePtr;
    uint8           u8Length;

    if (Nvm_eBlockDataDoing == NVM_PROJECT_DATA)
    {
        eTargetSegment  = NVM_CFG_PROJ_CONFIG_SEGMENT;
        pSourcePtr      = Nvm_aProjConfigMirror;
        u8Length        = BSW_MAX_PROJECT_CONFIG;
    }
    else if (Nvm_eBlockDataDoing == NVM_DTC_LOG)
    {
        eTargetSegment  = NVM_CFG_DTC_LOG_SEGMENT;
        pSourcePtr      = Nvm_aDTCMirror;
        u8Length        = BSW_MAX_ERROR_CODE;
    }
    else
    {
        /* Invalid Nvm_eBlockDataDoing */
        return;
    }

    if (Fls_Write(eTargetSegment, pSourcePtr, u8Length) == STD_OK)
    {
        /* Waiting for write done */
        Nvm_ChangeCurJobDoing(NVM_JOB_WAITING);
    }
}

/**
 * @brief       Erase NvM data segment
 * @retval      None
 */
static void Nvm_EraseNvmBlock(void)
{
    Fls_SegmentType eTargetSegment;

    if (Nvm_eBlockDataDoing == NVM_PROJECT_DATA)
    {
        eTargetSegment  = NVM_CFG_PROJ_CONFIG_SEGMENT;
    }
    else if (Nvm_eBlockDataDoing == NVM_DTC_LOG)
    {
        eTargetSegment  = NVM_CFG_DTC_LOG_SEGMENT;
    }
    else
    {
        /* Invalid Nvm_eBlockDataDoing */
        return;
    }

    if (Fls_Erase(eTargetSegment) == STD_OK)
    {
        /* Waiting for erase done */
        Nvm_ChangeCurJobDoing(NVM_JOB_WAITING);
    }
}

/**
 * @brief       Get data from RAM mirror by ID
 * @param[in]   eDataType   Type of data
 * @param[in]   u8DataID    DataID
 * @retval      uint8
 */
uint8 Nvm_GetDataById(const Nvm_DataType eDataType, const uint8 u8DataID)
{
    if (Nvm_bIsDataReady == TRUE)
    {
        if (eDataType == NVM_PROJECT_DATA)
        {
            return Nvm_aProjConfigMirror[u8DataID];
        }
        else if (eDataType == NVM_DTC_LOG)
        {
            return Nvm_aDTCMirror[u8DataID];
        }
    }

    return 0;
}

/**
 * @brief       Stores the specified data value into the RAM mirror 
 * @param[in]   eDataType   Type of data
 * @param[in]   u8DataID    DataID
 * @param[in]   u8DataIn    Data value
 * @retval      None
 */
void Nvm_SetDataById(const Nvm_DataType eDataType, const uint8 u8DataID, const uint8 u8DataIn)
{
    if (eDataType == NVM_PROJECT_DATA)
    {
        if (Nvm_aProjConfigMirror[u8DataID] != u8DataIn)
        {
            Nvm_aProjConfigMirror[u8DataID] = u8DataIn;
            Nvm_bIsPrjDatChange = TRUE;
        }
    }
    else if (eDataType == NVM_DTC_LOG)
    {
        if (Nvm_aDTCMirror[u8DataID] != u8DataIn)
        {
            Nvm_aDTCMirror[u8DataID] = u8DataIn;
            Nvm_bIsDTCChange = TRUE;
        }
    }
}

/**
 * @brief       NvM initial function
 * @retval      None
 */
void Nvm_InitFunction(void)
{
    Nvm_bIsDataReady    = FALSE;
    Nvm_bIsPrjDatChange = FALSE;
    Nvm_bIsDTCChange    = FALSE;
    Nvm_eBlockDataDoing = NVM_PROJECT_DATA;

    Nvm_eJob            = NVM_JOB_NONE;
    Nvm_ePreJob         = NVM_JOB_NONE;

    while (Nvm_bIsDataReady == FALSE)
    {
        Nvm_ReadAllNvmBlock();
    }
}

/**
 * @brief       NvM main function
 * @retval      None
 */
void Nvm_MainFunction(void)
{
    switch (Nvm_eJob)
    {
        case NVM_JOB_WRITING:
        {
            Nvm_WriteNvmBlock();
            break;
        }

        case NVM_JOB_ERASING:
        {
            Nvm_EraseNvmBlock();
            break;
        }

        case NVM_JOB_WAITING:
        {
            if (Fls_GetStatus() == FLS_IDLE)
            {
                if (Nvm_ePreJob == NVM_JOB_ERASING)
                {
                    /* Data segment is already erased. Ready to write */
                    Nvm_ChangeCurJobDoing(NVM_JOB_WRITING);
                }
                else if (Nvm_ePreJob == NVM_JOB_WRITING)
                {
                    /* Data is written. No job is pending. Release writing job */
                    Nvm_ChangeCurJobDoing(NVM_JOB_NONE);
                    Nvm_eBlockDataDoing = NVM_NONE;

                    /* Reset data change notification */
                    Nvm_bIsDTCChange    = FALSE;
                    Nvm_bIsPrjDatChange = FALSE;
                }
            }

            break;
        }

        case NVM_JOB_NONE:
        {
            /* Only one type of data can be written to flash at a time */
            if (Nvm_bIsDTCChange == TRUE)
            {
                Nvm_ChangeCurJobDoing(NVM_JOB_ERASING);
                Nvm_eBlockDataDoing = NVM_DTC_LOG;
            }
            else if (Nvm_bIsPrjDatChange == TRUE)
            {
                Nvm_ChangeCurJobDoing(NVM_JOB_ERASING);
                Nvm_eBlockDataDoing = NVM_PROJECT_DATA;
            }
            else
            {
                /* Do nothing */
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

#endif /* (BSW_CFG_NVM_FUNCTION == STD_ENABLED) */
