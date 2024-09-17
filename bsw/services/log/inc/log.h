#ifndef LOG_H
#define LOG_H

#include "log_cfg.h"

#if (BSW_CFG_LOG_FUNCTION == STD_ENABLED)
#define LOG_NUM_ERRORS_ID        0U

typedef enum __attribute__((packed))
{
    TEST_LOG_ERROR
} Log_ErrorType;

#define LOG_ERROR(errorID)                  Log_LogErrorCode(errorID)

extern void Log_LogErrorCode(Log_ErrorType eErrorId);
extern void Log_InitFunction(void);

#else
#define LOG_ERROR(errorID)

#endif /* (BSW_CFG_LOG_FUNCTION == STD_ENABLED) */
#endif /* LOG_H */
