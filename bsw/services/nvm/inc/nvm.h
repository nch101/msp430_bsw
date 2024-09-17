#ifndef NVM_H
#define NVM_H

#include "nvm_cfg.h"

#if (BSW_CFG_NVM_FUNCTION == STD_ENABLED)
typedef enum __attribute__((packed))
{
    NVM_NONE,
    NVM_PROJECT_DATA,
    NVM_DTC_LOG,
} Nvm_DataType;

typedef enum __attribute__((packed))
{
    NVM_JOB_NONE,
    NVM_JOB_WRITING,
    NVM_JOB_ERASING,
    NVM_JOB_WAITING,
} Nvm_JobType;

#define NVM_READING_FLASH_DATA()            while (Fls_GetStatus() != FLS_IDLE) { Fls_MainFunction(); }

extern uint8 Nvm_GetDataById(Nvm_DataType eDataType, uint8 u8DataID);
extern void Nvm_SetDataById(Nvm_DataType eDataType, uint8 u8DataID, uint8 *pDataPtr);
extern void Nvm_InitFunction(void);
extern void Nvm_MainFunction(void);

#endif /* (BSW_CFG_NVM_FUNCTION == STD_ENABLED) */
#endif /* NVM_H */
