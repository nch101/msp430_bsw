#ifndef NVM_CFG_H
#define NVM_CFG_H

#include "bsw_cfg.h"

/* Mapping project config data block with flash segment*/
#define NVM_CFG_PROJ_CONFIG_SEGMENT         FLS_INFO_B

/* Mapping event log block with flash segment*/
#define NVM_CFG_EVENT_LOG_SEGMENT           FLS_INFO_C

/* Nvm job queue size */
#define NVM_CFG_QUEUE_JOB_SIZE              3U

/* Project data configuration ID */
typedef enum __attribute__((packed)) Nvm_DataID
{
    NUM_DUMMY_CONFIG_1,
    NVM_MAX_DATA_CONFIG = BSW_MAX_PROJECT_CONFIG
} Nvm_DataID;

#endif /* NVM_CFG_H */
