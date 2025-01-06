#ifndef DEBUG_CFG_H
#define DEBUG_CFG_H

#include "bsw_cfg.h"
#include "cfg_types.h"

#if (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED)
#define DEBUG_CFG_LEVEL                     DEBUG_SEL_INFO_LEVEL

#endif /* (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED) */
#endif /* DEBUG_CFG_H */
