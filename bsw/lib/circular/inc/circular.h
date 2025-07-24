#ifndef CIRCULAR_H
#define CIRCULAR_H

#include "circular_cfg.h"

#if (BSW_CFG_CIRCULAR_FUNCTION == STD_ENABLED)
typedef struct Circular_t
{
    uint8*  pBuffer;
    uint8   capacity;
    uint8   readIdx;
    uint8   writeIdx;
    uint8   availSize;
} Circular_t;

extern Std_StatusType Circular_InitBuffer(Circular_t * const pCircularBuff, uint8 * const pBuffer, uint8 const u8Capacity);
extern Std_StatusType Circular_PushData(Circular_t * const pCircularBuff, uint8 const * const pData, const uint8 u8Len);
extern Std_StatusType Circular_PopData(Circular_t * const pCircularBuff, const uint8 u8Len, uint8 * pData);

#endif /* (BSW_CFG_CIRCULAR_FUNCTION == STD_ENABLED) */
#endif /* CIRCULAR_H */
