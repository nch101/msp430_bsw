#ifndef COM_CFG_H
#define COM_CFG_H
#include "bsw_cfg.h"

#if (BSW_CFG_COM_FUNCTION == STD_ENABLED)
#define COM_CFG_RX_MSG_TIMEOUT          2U
#define COM_CFG_MAX_RX_DATA_LEN         10U

#define COM_CFG_MAX_TX_DATA_LEN         8U
#define COM_CFG_MAX_TX_BUFF             2U

#endif /* (BSW_CFG_COM_FUNCTION == STD_ENABLED) */
#endif /* COM_CFG_H */
