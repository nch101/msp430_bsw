#ifndef FLS_CFG_H
#define FLS_CFG_H

#include "bsw_cfg.h"
#include "mcu_cfg.h"

#if (BSW_CFG_FLS_FUNCTION == STD_ENABLED)

#define FLS_CFG_MAX_BYTES_WRITTEN           8U
#define FLS_CFG_MAX_BYTES_READ              8U

#endif /* (BSW_CFG_FLS_FUNCTION == STD_ENABLED) */
#endif /* FLS_CFG_H */
