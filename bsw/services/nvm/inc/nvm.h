#ifndef NVM_H
#define NVM_H

#include "fls.h"
#include "nvm_cfg.h"

#if (BSW_CFG_NVM_FUNCTION == STD_ENABLED)
typedef enum __attribute__((packed)) Nvm_DataType
{
    NVM_PROJECT_DATA,
    NVM_DTC_LOG,
    NVM_NUM_BLOCKS,
    NVM_NOT_PRESENT,
} Nvm_DataType;

typedef enum __attribute__((packed)) Nvm_JobType
{
    NVM_JOB_NONE,
    NVM_JOB_WRITING,
    NVM_JOB_ERASING,
    NVM_JOB_WAITING,
} Nvm_JobType;

typedef struct
{
    uint8           *aRamMirror;
    uint8           u8Len;
    Fls_SegmentType eFlsSegment;
} Nvm_BlockType;

#define NVM_READING_FLASH_DATA()            while (Fls_GetStatus() != FLS_IDLE) { Fls_MainFunction(); }

extern uint8 Nvm_GetDataById(const Nvm_DataType eDataType, const uint8 u8DataID);
extern void Nvm_SetDataById(const Nvm_DataType eDataType, const uint8 u8DataID, const uint8 u8DataIn);
extern void Nvm_InitFunction(void);
extern void Nvm_MainFunction(void);

#endif /* (BSW_CFG_NVM_FUNCTION == STD_ENABLED) */
#endif /* NVM_H */
