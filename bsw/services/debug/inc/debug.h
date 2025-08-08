#ifndef DEBUG_H
#define DEBUG_H

#include "debug_cfg.h"
#include "debug_dictionary.h"

#if (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED)

typedef struct Debug_DebugMsgSt
{
    uint16              u16Arg;
    Debug_MessageID     eMsgID;
    uint16              u16Line;
    Debug_LevelType     eLevel;
    Debug_ModuleName    eModule;
    uint8               u8Length;
} Debug_DebugMsgSt;

#if (DEBUG_CFG_LEVEL >= DEBUG_SEL_FATAL_LEVEL)
#define DEBUG_FATAL(module, msg)                Debug_LogMessage(FATAL, module, __LINE__, msg, 0, FALSE)
#define DEBUG_FATAL_U16_ARG(module, msg, arg)   Debug_LogMessage(FATAL, module, __LINE__, msg, arg, TRUE)
#else
#define DEBUG_FATAL(module, msg)
#define DEBUG_FATAL_U16_ARG(module, msg, arg)
#endif /* (DEBUG_CFG_LEVEL >= DEBUG_SEL_FATAL_LEVEL) */

#if (DEBUG_CFG_LEVEL >= DEBUG_SEL_ERROR_LEVEL)
#define DEBUG_ERROR(module, msg)                Debug_LogMessage(ERROR, module, __LINE__, msg, 0, FALSE)
#define DEBUG_ERROR_U16_ARG(module, msg, arg)   Debug_LogMessage(ERROR, module, __LINE__, msg, arg, TRUE)
#else
#define DEBUG_ERROR(module, msg)
#define DEBUG_ERROR_U16_ARG(module, msg, arg)
#endif /* (DEBUG_CFG_LEVEL >= DEBUG_SEL_ERROR_LEVEL) */

#if (DEBUG_CFG_LEVEL >= DEBUG_SEL_WARN_LEVEL)
#define DEBUG_WARN(module, msg)                 Debug_LogMessage(WARN, module, __LINE__, msg, 0, FALSE)
#define DEBUG_WARN_U16_ARG(module, msg, arg)    Debug_LogMessage(WARN, module, __LINE__, msg, arg, TRUE)
#else
#define DEBUG_WARN(module, msg)
#define DEBUG_WARN_U16_ARG(module, msg, arg)
#endif /* (DEBUG_CFG_LEVEL >= DEBUG_SEL_WARN_LEVEL) */

#if (DEBUG_CFG_LEVEL >= DEBUG_SEL_INFO_LEVEL)
#define DEBUG_INFO(module, msg)                 Debug_LogMessage(INFO, module, __LINE__, msg, 0, FALSE)
#define DEBUG_INFO_U16_ARG(module, msg, arg)    Debug_LogMessage(INFO, module, __LINE__, msg, arg, TRUE)
#else
#define DEBUG_INFO(module, msg)
#define DEBUG_INFO_U16_ARG(module, msg, arg)
#endif /* (DEBUG_CFG_LEVEL >= DEBUG_SEL_INFO_LEVEL) */

extern void Debug_LogMessage(const Debug_LevelType eLevel, const Debug_ModuleName eModule, const uint16 u16Line, \
                                const Debug_MessageID eMessage, const uint16 u16Num, const uint8 bIntArg);
#else
#define DEBUG_FATAL(module, msg)
#define DEBUG_FATAL_U16_ARG(module, msg, arg)

#define DEBUG_ERROR(module, msg)
#define DEBUG_ERROR_U16_ARG(module, msg, arg)

#define DEBUG_WARN(module, msg)
#define DEBUG_WARN_U16_ARG(module, msg, arg)

#define DEBUG_INFO(module, msg)
#define DEBUG_INFO_U16_ARG(module, msg, arg)

#endif /* (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED) */
#endif /* DEBUG_H */
