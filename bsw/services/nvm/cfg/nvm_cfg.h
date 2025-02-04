#ifndef NVM_CFG_H
#define NVM_CFG_H

#include "bsw_cfg.h"

/* Mapping project config data block with flash segment*/
#define NVM_CFG_PROJ_CONFIG_SEGMENT         FLS_INFO_B

/* Mapping DTC data block with flash segment*/
#define NVM_CFG_ERROR_CODE_SEGMENT          FLS_INFO_C

#endif /* NVM_CFG_H */
