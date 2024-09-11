#ifndef FLS_H
#define FLS_H

#include "wdt.h"
#include "fls_cfg.h"
#include "fls_types.h"

#if (FLS_CFG_FUNCTION == STD_ENABLED)

typedef enum __attribute__((packed))
{
    FLS_JOB_NONE,
    FLS_JOB_ERASE,
    FLS_JOB_WRITE,
    FLS_JOB_READ,
} Fls_JobType;

typedef enum __attribute__((packed))
{
    FLS_IDLE,
    FLS_PENDING,
    FLS_PROCESSING,
    FLS_ERROR,
} Fls_StatusType;

extern Fls_StatusType Fls_GetStatus(void);
extern Std_StatusType Fls_Erase(Fls_SegmentType eSegmentName);
extern Std_StatusType Fls_Write(const Fls_SegmentType eSegmentName, uint8 * const pSourceAddrPtr, const uint16 u16Length);
extern Std_StatusType Fls_Read(uint8 * const pSourceAddrPtr, uint8 * const pTargetAddrPtr, const uint16 u16Length);
extern void Fls_InitFunction(void);
extern void Fls_MainFunction(void);

#endif /* (FLS_CFG_FUNCTION == STD_ENABLED) */
#endif /* FLS_H */
