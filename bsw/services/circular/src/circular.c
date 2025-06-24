#include "circular.h"

#if (BSW_CFG_CIRCULAR_FUNCTION == STD_ENABLED)

static void Circular_CorrectIndex(uint16 * const currentIndex, const uint16 maxSize)
{
    if (*currentIndex < maxSize - 1U)
    {
        (*currentIndex)++;
    }
    else
    {
        (*currentIndex) = 0;
    }
}

/**
 * @brief       Initializing buffer function
 * @param[out]  pCircularBuff   Pointer to circular buffer
 * @param[in]   pBuffer         Pointer to uint8 type buffer
 * @param[in]   u16MaxSize      Size of uint8 buffer
 * @retval      STD_OK
 */
Std_StatusType Circular_InitBuffer(Circular_t * const pCircularBuff, uint8 * const pBuffer, uint16 const u16MaxSize)
{
    pCircularBuff->pBuffer      = pBuffer;
    pCircularBuff->readIdx      = 0;
    pCircularBuff->writeIdx     = 0;
    pCircularBuff->availSize    = u16MaxSize;
    pCircularBuff->maxSize      = u16MaxSize;

    return STD_OK;
}

/**
 * @brief       Push data into circular buffer
 * @param[in]   pCircularBuff   Pointer to circular buffer
 * @param[in]   data            Data to push
 * @retval      STD_OK      - buffer is free to store data
 * @retval      STD_NOT_OK  - buffer is full
 */
Std_StatusType Circular_PushData(Circular_t * const pCircularBuff, const uint8 data)
{
    /* Check if is buffer available to store data */
    if (pCircularBuff->availSize > 0U)
    {
        pCircularBuff->pBuffer[pCircularBuff->writeIdx] = data;
        pCircularBuff->availSize--;

        Circular_CorrectIndex(&(pCircularBuff->writeIdx), pCircularBuff->maxSize);

        return STD_OK;
    }

    return STD_NOT_OK;
}

/**
 * @brief       Pull data from circular buffer
 * @param[in]   pCircularBuff   Pointer to circular buffer
 * @param[out]  pData           Pointer to variable to store
 * @retval      STD_OK      - Data is available in buffer
 * @retval      STD_NOT_OK  - Buffer is empty
 */
Std_StatusType Circular_PullData(Circular_t * const pCircularBuff, uint8 * pData)
{
    /* Check if is data available in circular buffer */
    if (pCircularBuff->availSize < pCircularBuff->maxSize)
    {
        *pData = pCircularBuff->pBuffer[pCircularBuff->readIdx];
        pCircularBuff->availSize++;

        Circular_CorrectIndex(&(pCircularBuff->readIdx), pCircularBuff->maxSize);

        return STD_OK;
    }

    return STD_NOT_OK;
}

#endif /* BSW_CFG_CIRCULAR_FUNCTION */
