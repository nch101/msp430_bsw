#ifndef CIRCULAR_H
#define CIRCULAR_H

#include "circular_cfg.h"

#if (BSW_CFG_CIRCULAR_FUNCTION == STD_ENABLED)
typedef struct Circular_t
{
    uint8*  pBuffer;
    uint16  readIdx;
    uint16  writeIdx;
    uint16  availSize;
    uint16  maxSize;
} Circular_t;

Std_StatusType Circular_InitBuffer(Circular_t * const pCircularBuff, uint8 * const pBuffer, uint16 const u16MaxSize);
Std_StatusType Circular_PushData(Circular_t * const pCircularBuff, const uint8 data);
Std_StatusType Circular_PullData(Circular_t * const pCircularBuff, uint8 * pData);

#endif /* (BSW_CFG_CIRCULAR_FUNCTION == STD_ENABLED) */
#endif /* CIRCULAR_H */
