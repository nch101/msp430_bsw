#include <string.h>
#include "nvm.h"
#include "circular.h"
#include "project_cfg.h"

#if (BSW_CFG_NVM_FUNCTION == STD_ENABLED)

static Circular_t   Nvm_CircularBuff;

static uint8        Nvm_aEventLogMirror[BSW_MAX_EVENT_LOG];
static PrjCfgType_t Nvm_tPrjCfgMirror;

static Nvm_JobType  Nvm_eJob;
static Nvm_DataType Nvm_eBlockDataDoing;
static Nvm_JobType  Nvm_aQueueJob[NVM_CFG_QUEUE_JOB_SIZE];

static uint8        Nvm_bIsPrjDatChange;
static uint8        Nvm_bIsEventLogChange;

const Nvm_DataBlockType Nvm_DataCfgLookupTable[NVM_MAX_DATA_CONFIG] =
{
    {
        &(Nvm_tPrjCfgMirror.item.dummy.value),
        sizeof(uint8)
    },
};

const Nvm_BlockType     Nvm_BlockInfo[NVM_NUM_BLOCK_TYPE] =
{
    {
        (uint8 *) &Nvm_tPrjCfgMirror,
        BSW_MAX_PROJECT_CONFIG,
        NVM_CFG_PROJ_CONFIG_SEGMENT,
    },
    {
        (uint8 *) &Nvm_aEventLogMirror,
        BSW_MAX_EVENT_LOG,
        NVM_CFG_EVENT_LOG_SEGMENT,
    }
};

/**
 * @brief       Read all Nvm data segment then copy to RAM mirror
 * @retval      None
 */
static void Nvm_ReadAllNvmBlock(void)
{
    uint8 u8Index;

    for (u8Index = 0; u8Index < NVM_NUM_BLOCK_TYPE; u8Index++)
    {
        Fls_Read(Nvm_BlockInfo[u8Index].flsSegment, Nvm_BlockInfo[u8Index].RamMirrorDataAddr, Nvm_BlockInfo[u8Index].len);

        NVM_READING_FLASH_DATA();
    }
}

/**
 * @brief       Get data from RAM mirror by ID
 * @param[in]   eDataType   Type of data
 * @param[in]   u8DataID    DataID
 * @param[out]  pDataOut    Pointer to storing variable
 * @retval      None
 */
void Nvm_GetDataById(const Nvm_DataType eDataType, const uint8 u8DataID, uint8 * pDataOut)
{
    if (eDataType == NVM_PROJECT_DATA)
    {
        memcpy((void *) pDataOut, (void *) Nvm_DataCfgLookupTable[u8DataID].RamBlockDataAddr, Nvm_DataCfgLookupTable[u8DataID].len);
    }
    else if (eDataType == NVM_EVENT_LOG)
    {
        *pDataOut = Nvm_aEventLogMirror[u8DataID];
    }
    else
    {
        *pDataOut = 0xFFU;
    }
}

/**
 * @brief       Stores the specified data value into the RAM mirror 
 * @param[in]   eDataType   Type of data
 * @param[in]   u8DataID    DataID
 * @param[in]   u8DataIn    Data value
 * @retval      None
 */
void Nvm_SetDataById(const Nvm_DataType eDataType, const uint8 u8DataID, uint8 const * const pDataIn)
{
    if (eDataType == NVM_PROJECT_DATA)
    {
        if (*(Nvm_DataCfgLookupTable[u8DataID].RamBlockDataAddr) != *pDataIn)
        {
            memcpy((void *) Nvm_DataCfgLookupTable[u8DataID].RamBlockDataAddr, (void *) pDataIn, Nvm_DataCfgLookupTable[u8DataID].len);
            Nvm_bIsPrjDatChange = TRUE;
        }
    }
    else if (eDataType == NVM_EVENT_LOG)
    {
        if (Nvm_aEventLogMirror[u8DataID] != *pDataIn)
        {
            Nvm_aEventLogMirror[u8DataID] = *pDataIn;
            Nvm_bIsEventLogChange = TRUE;
        }
    }
}

/**
 * @brief       NvM initial function
 * @retval      None
 */
void Nvm_InitFunction(void)
{
    Nvm_bIsPrjDatChange     = FALSE;
    Nvm_bIsEventLogChange   = FALSE;
    Nvm_eBlockDataDoing     = NVM_NOT_PRESENT;
    Nvm_eJob                = NVM_JOB_NONE;
    
    Nvm_ReadAllNvmBlock();
    Circular_InitBuffer(&Nvm_CircularBuff, Nvm_aQueueJob, NVM_CFG_QUEUE_JOB_SIZE);
}

/**
 * @brief       NvM main function
 * @retval      None
 */
void Nvm_MainFunction(void)
{
    Nvm_DataType    eBlockDataChanged;

    switch (Nvm_eJob)
    {
        case NVM_JOB_ERASE_REQUESTING:
        {
            if (Fls_Erase(Nvm_BlockInfo[Nvm_eBlockDataDoing].flsSegment) == STD_OK)
            {
                /* Waiting for erase done */
                Nvm_eJob = NVM_JOB_ERASING;
            }

            break;
        }

        case NVM_JOB_ERASING:
        {
            if (Fls_GetStatus() == FLS_IDLE)
            {
                /* Data segment is already erased. Ready to write */
                Nvm_eJob = NVM_JOB_WRITE_REQUESTING;
            }

            break;
        }

        case NVM_JOB_WRITE_REQUESTING:
        {
            if (Fls_Write(Nvm_BlockInfo[Nvm_eBlockDataDoing].flsSegment, 
                Nvm_BlockInfo[Nvm_eBlockDataDoing].RamMirrorDataAddr, 
                Nvm_BlockInfo[Nvm_eBlockDataDoing].len) == STD_OK)
            {
                /* Waiting for write done */
                Nvm_eJob = NVM_JOB_WRITING;
            }

            break;
        }

        case NVM_JOB_WRITING:
        {
            if (Fls_GetStatus() == FLS_IDLE)
            {
                /* Data is written. No job is pending. Release writing job */
                Nvm_eJob = NVM_JOB_NONE;

                /* Reset data change notification */
                Nvm_eBlockDataDoing = NVM_NOT_PRESENT;
            }

            break;
        }

        case NVM_JOB_NONE:
        {
            if (Nvm_bIsEventLogChange == TRUE)
            {
                Nvm_bIsEventLogChange   = FALSE;
                eBlockDataChanged       = NVM_EVENT_LOG;
                Circular_PushData(&Nvm_CircularBuff, &eBlockDataChanged, 1U);
            }

            if (Nvm_bIsPrjDatChange == TRUE)
            {
                Nvm_bIsPrjDatChange     = FALSE;
                eBlockDataChanged       = NVM_PROJECT_DATA;
                Circular_PushData(&Nvm_CircularBuff, &eBlockDataChanged, 1U);
            }

            if (Circular_PopData(&Nvm_CircularBuff, 1U, &Nvm_eBlockDataDoing) == STD_OK)
            {
                Nvm_eJob = NVM_JOB_ERASE_REQUESTING;
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
