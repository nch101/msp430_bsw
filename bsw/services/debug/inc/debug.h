#ifndef DEBUG_H
#define DEBUG_H

#include "debug_cfg.h"

#if (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED)

typedef enum __attribute__((packed))
{
    DEBUG_JOB_IDLE,
    DEBUG_JOB_SENDING,
} Debug_JobType;

#define DEBUG_LIMIT_BUFFER_IDX(x, y)        x = (x >= y) ? 0 : x

#if (DEBUG_CFG_LEVEL >= DEBUG_SEL_FATAL_LEVEL)
#define DEBUG_FATAL(...)                    Debug_LogMessage("FATAL:", __VA_ARGS__)
#else
#define DEBUG_FATAL(...)
#endif /* (DEBUG_CFG_LEVEL >= DEBUG_SEL_FATAL_LEVEL) */

#if (DEBUG_CFG_LEVEL >= DEBUG_SEL_ERROR_LEVEL)
#define DEBUG_ERROR(...)                    Debug_LogMessage("ERROR:", __VA_ARGS__)
#else
#define DEBUG_ERROR(...)
#endif /* (DEBUG_CFG_LEVEL >= DEBUG_SEL_ERROR_LEVEL) */

#if (DEBUG_CFG_LEVEL >= DEBUG_SEL_WARN_LEVEL)
#define DEBUG_WARN(...)                     Debug_LogMessage("WARN:", __VA_ARGS__)
#else
#define DEBUG_WARN(...)
#endif /* (DEBUG_CFG_LEVEL >= DEBUG_SEL_WARN_LEVEL) */

#if (DEBUG_CFG_LEVEL >= DEBUG_SEL_INFO_LEVEL)
#define DEBUG_INFO(...)                     Debug_LogMessage("INFO:", __VA_ARGS__)
#else
#define DEBUG_INFO(...)
#endif /* (DEBUG_CFG_LEVEL >= DEBUG_SEL_INFO_LEVEL) */

extern void Debug_LogMessage(const char *pLevelPtr, const char *pFormatPtr, ...);
extern void Debug_MainFunction(void);
extern void Debug_InitFunction(void);


#endif /* (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED) */
#endif /* DEBUG_H */
