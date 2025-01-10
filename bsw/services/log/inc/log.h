#ifndef LOG_H
#define LOG_H

#include "log_cfg.h"
#include "log_dictionary.h"

#if (BSW_CFG_LOG_FUNCTION == STD_ENABLED)
#define LOG_NUM_ERRORS_ID        0U

#define LOG_ERROR(errorID)                  Log_LogErrorCode(errorID)

extern void Log_LogErrorCode(const Log_ErrorId eErrorId);
extern void Log_ReadErrorCode(uint8 * const pNumErrors, uint8 * pErrorOut);
extern void Log_ClearErrorCode(void);

#else
#define LOG_ERROR(errorID)

#endif /* (BSW_CFG_LOG_FUNCTION == STD_ENABLED) */
#endif /* LOG_H */
