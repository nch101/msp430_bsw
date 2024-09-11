#include "fls.h"


#if (FLS_CFG_FUNCTION == STD_ENABLED)

/* Currently executed job */
static Fls_JobType      Fls_eJob;

/* Current job status */
static Fls_StatusType   Fls_eJobStatus;

/* Data u16Length */
static uint16           Fls_u16Length;

static uint8 *          Fls_pSourceAddrPtr;

static volatile uint8 * Fls_pTargetAddrPtr;

/* Physical segmentation start address */
static volatile uint8 * const Fls_pSegmentStartAddrPtr[] =
{
    (volatile uint8 *) 0x1080U,             /* FLS_INFO_B */
    (volatile uint8 *) 0x1040U,             /* FLS_INFO_C */
    (volatile uint8 *) 0x1000U,             /* FLS_INFO_D */
};

/**
 * @brief       Reset flash internal variables
 * @retval      None
 */
static void Fls_ResetInternalVariable(void)
{
    Fls_eJob            = FLS_JOB_NONE;
    Fls_eJobStatus      = FLS_IDLE;
    Fls_u16Length       = 0U;
    Fls_pTargetAddrPtr  = NULL;
    Fls_pSourceAddrPtr  = NULL;
}

/**
 * @brief       Handling flashing erase job
 * @retval      None
 */
static void Fls_HandlingFlsEraseJob(void)
{
    switch (Fls_eJobStatus)
    {
        case FLS_PENDING:
        {
            if ((FCTL3 & BUSY) == 0U)
            {
                /* Stop watchdog timer */
                WDT_SuspendWatchdogTimer();
                FLS_CLEAR_LOCK_BIT();
                FLS_SET_ERASE_BIT();

                /* Dummy write to erase flash segment */
                *Fls_pTargetAddrPtr = 0xFFU;

                Fls_eJobStatus = FLS_PROCESSING;
            }

            break;
        }

        case FLS_PROCESSING:
        {
            if ((FCTL3 & BUSY) == 0U)
            {
                FLS_SET_LOCK_BIT();
                WDT_StartWatchdogTimer();
                Fls_ResetInternalVariable();
            }

            break;
        }

        default:
        {
            break;
        }
    }
}

/**
 * @brief       Handling flashing write job
 * @retval      None
 */
static void Fls_HandlingFlsWriteJob(void)
{
    switch (Fls_eJobStatus)
    {
        case FLS_PENDING:
        {
            if ((FCTL3 & BUSY) == 0U)
            {
                /* Stop watchdog timer */
                WDT_SuspendWatchdogTimer();
                FLS_CLEAR_LOCK_BIT();
                FLS_SET_WRITE_BIT();

                Fls_eJobStatus = FLS_PROCESSING;
            }

            break;
        }

        case FLS_PROCESSING:
        {
            if ((FCTL3 & BUSY) == 0U)
            {
                for (uint8 index = 0U; index < FLS_CFG_MAX_BYTES_WRITTEN; index++)
                {
                    /* Write data to target address */
                    *Fls_pTargetAddrPtr = *Fls_pSourceAddrPtr;

                    /* Increase target and source address */
                    Fls_pTargetAddrPtr++;
                    Fls_pSourceAddrPtr++;

                    Fls_u16Length--;

                    /* While until data has been written */
                    FLS_WAIT_WRITE_DONE();

                    if (Fls_u16Length <= 0U)
                    {
                        /* No more data to write */
                        FLS_CLEAR_WRITE_BIT();

                        /* Skip waitting for setting BUSY bit to 0 */

                        FLS_SET_LOCK_BIT();
                        WDT_StartWatchdogTimer();
                        Fls_ResetInternalVariable();

                        break;
                    }
                }
            }

            break;
        }

        default:
        {
            break;
        }
    }
}

/**
 * @brief       Handling flashing read job
 * @retval      None
 */
static void Fls_HandlingFlsReadJob(void)
{
    switch (Fls_eJobStatus)
    {
        case FLS_PROCESSING:
        {
            for(uint8 index = 0; index < FLS_CFG_MAX_BYTES_READ; index++)
            {
                /* Copy data from flash to user buffer */
                *Fls_pTargetAddrPtr = *Fls_pSourceAddrPtr;
                Fls_pTargetAddrPtr++;
                Fls_pSourceAddrPtr++;

                Fls_u16Length--;

                if (Fls_u16Length <= 0U)
                {
                    /* Release read job */
                    Fls_ResetInternalVariable();

                    break;
                }
            }

            break;
        }

        default:
        {
            break;
        }
    }
}

/**
 * @brief       Get current job status
 * @retval      Fls_StatusType
 */
Fls_StatusType Fls_GetStatus(void)
{
    return Fls_eJobStatus;
}

/**
 * @brief       Erase one segment 
 * @param[in]   eSegmentName            Name of segment
 * @retval      STD_NOT_OK - Trigger not successful, STD_OK - Trigger successful
 */
Std_StatusType Fls_Erase(Fls_SegmentType eSegmentName)
{
    if (Fls_eJobStatus != FLS_IDLE)
    {
        return STD_NOT_OK;
    }

    Fls_eJob            = FLS_JOB_ERASE;
    Fls_eJobStatus      = FLS_PENDING;
    Fls_pTargetAddrPtr  = Fls_pSegmentStartAddrPtr[(uint8) eSegmentName];

    return STD_OK;
}

/**
 * @brief       Write to flash segment
 * @param[in]   eSegmentName            Name of segment
 * @param[in]   pSourceAddrPtr          Pointer to source data buffer
 * @param[in]   u16Length               Number of bytes to write
 * @retval      STD_NOT_OK - Trigger not successful, STD_OK - Trigger successful
 */
Std_StatusType Fls_Write(const Fls_SegmentType eSegmentName, uint8 * const pSourceAddrPtr, const uint16 u16Length)
{
    if (pSourceAddrPtr == NULL)
    {
        return STD_NOT_OK;
    }

    if (Fls_eJobStatus != FLS_IDLE)
    {
        return STD_NOT_OK;
    }

    if (u16Length > FLS_MAX_SEGMENT_SIZE)
    {
        return STD_NOT_OK;
    }

    Fls_eJob            = FLS_JOB_WRITE;
    Fls_eJobStatus      = FLS_PENDING;
    Fls_pTargetAddrPtr  = Fls_pSegmentStartAddrPtr[(uint8) eSegmentName];
    Fls_pSourceAddrPtr  = pSourceAddrPtr;
    Fls_u16Length       = u16Length;

    return STD_OK;
}


/**
 * @brief       Read data from  
 * @param[in]   pSourceAddrPtr          Pointer to source data buffer
 * @param[out]  pTargetAddrPtr          Pointer to target data buffer
 * @param[in]   u16Length               Number of bytes to read
 * @retval      STD_NOT_OK - Trigger not successful, STD_OK - Trigger successful
 */
Std_StatusType Fls_Read(uint8 * const pSourceAddrPtr, uint8 * const pTargetAddrPtr, const uint16 u16Length)
{
    if ((pTargetAddrPtr == NULL) \
      || (pSourceAddrPtr == NULL))
    {
        return STD_NOT_OK;
    }

    if (Fls_eJobStatus != FLS_IDLE)
    {
        return STD_NOT_OK;
    }

    Fls_eJob            = FLS_JOB_READ;
    Fls_eJobStatus      = FLS_PROCESSING;
    Fls_pTargetAddrPtr  = pTargetAddrPtr;
    Fls_pSourceAddrPtr  = pSourceAddrPtr;
    Fls_u16Length       = u16Length;

    return STD_OK;
}

/**
 * @brief       Initialize flash module
 * @retval      None
 */
void Fls_InitFunction(void)
{
    Fls_ResetInternalVariable();

    FCTL2 = FWKEY + FSSEL0 + FN1;
}

/**
 * @brief       Performs actual flash read, write and erase
 * @retval      None
 */
void Fls_MainFunction(void)
{
    switch (Fls_eJob)
    {
        case FLS_JOB_ERASE:
        {
            Fls_HandlingFlsEraseJob();
            break;
        }

        case FLS_JOB_WRITE:
        {
            Fls_HandlingFlsWriteJob();
            break;
        }

        case FLS_JOB_READ:
        {
            Fls_HandlingFlsReadJob();
            break;
        }
    
        case FLS_JOB_NONE:
        default:
        {
            /* Do nothing */
            break;
        }
    }
}
#endif /* (FLS_CFG_FUNCTION == STD_ENABLED) */