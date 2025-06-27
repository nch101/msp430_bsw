#ifndef NVM_H
#define NVM_H

#include "fls.h"
#include "nvm_cfg.h"

#if (BSW_CFG_NVM_FUNCTION == STD_ENABLED)
typedef enum __attribute__((packed)) Nvm_JobType
{
    NVM_JOB_NONE,
    NVM_JOB_ERASE_REQUESTING,
    NVM_JOB_ERASING,
    NVM_JOB_WRITE_REQUESTING,
    NVM_JOB_WRITING,
} Nvm_JobType;

typedef enum __attribute__((packed)) Nvm_DataType
{
    NVM_PROJECT_DATA,
    NVM_EVENT_LOG,
    NVM_NUM_BLOCK_TYPE,
    NVM_NOT_PRESENT = 0xFF,
} Nvm_DataType;

typedef struct Nvm_BlockType
{
    uint8           *RamMirrorDataAddr;
    uint8           len;
    Fls_SegmentType flsSegment;
} Nvm_BlockType;

typedef struct  __attribute__((packed)) Nvm_DataBlockType
{
    uint8* RamBlockDataAddr;
    uint8  len;
} Nvm_DataBlockType;

#define NVM_READING_FLASH_DATA()            while (Fls_GetStatus() != FLS_IDLE) { Fls_MainFunction(); }

extern void Nvm_GetDataById(const Nvm_DataType eDataType, const uint8 u8DataID, uint8 * pDataOut);
extern void Nvm_SetDataById(const Nvm_DataType eDataType, const uint8 u8DataID, uint8 const * const pDataIn);
extern void Nvm_InitFunction(void);
extern void Nvm_MainFunction(void);

#endif /* (BSW_CFG_NVM_FUNCTION == STD_ENABLED) */
#endif /* NVM_H */
