#include "com.h"
#include "debug.h"

#if (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED)
void Debug_LogMessage( \
            const Debug_LevelType   eLevel,  \
            const Debug_ModuleName  eModule, \
            const uint16            u16Line, \
            const Debug_MessageID   eMessage,\
            const uint16            u16Num,  \
            const uint8             bIntArg  \
        )
{
    Debug_DebugMsgSt Debug_aMsgBuffer;

    /* Build debug message */
    Debug_aMsgBuffer.eLevel     = eLevel;
    Debug_aMsgBuffer.eModule    = eModule;
    Debug_aMsgBuffer.u16Line    = u16Line;
    Debug_aMsgBuffer.eMsgID     = eMessage;
    Debug_aMsgBuffer.u8Length   = 6U;

    if (bIntArg == TRUE)
    {
        /* Build debug message with 1 uint16 argument */
        Debug_aMsgBuffer.u16Arg = u16Num;
        Debug_aMsgBuffer.u8Length = 8U;
    }

    (void) Com_TransmitData((uint8 *) &Debug_aMsgBuffer, Debug_aMsgBuffer.u8Length);
}
#endif /* (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED) */
