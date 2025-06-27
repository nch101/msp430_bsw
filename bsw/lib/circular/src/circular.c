#include "circular.h"

#if (BSW_CFG_CIRCULAR_FUNCTION == STD_ENABLED)

static void Circular_CorrectIndex(uint8 * const pCurrentIndex, const uint8 u8MaxSize)
{
    if (*pCurrentIndex < (u8MaxSize - 1U))
    {
        (*pCurrentIndex)++;
    }
    else
    {
        (*pCurrentIndex) = 0;
    }
}

/**
 * @brief       Initializes a cirular buffer
 * @param[out]  pCircularBuff   Pointer to circular buffer
 * @param[in]   pBuffer         Pointer to uint8 type buffer
 * @param[in]   u8MaxSize       Size of uint8 buffer
 * @retval      STD_OK
 */
Std_StatusType Circular_InitBuffer(Circular_t * const pCircularBuff, uint8 * const pBuffer, uint8 const u8MaxSize)
{
    pCircularBuff->pBuffer      = pBuffer;
    pCircularBuff->readIdx      = 0;
    pCircularBuff->writeIdx     = 0;
    pCircularBuff->availSize    = u8MaxSize;
    pCircularBuff->maxSize      = u8MaxSize;

    return STD_OK;
}

/**
 * @brief       Pushes data into circular buffer
 * @param[in]   pCircularBuff   Pointer to circular buffer
 * @param[in]   data            Data to push
 * @retval      STD_OK      - buffer is free to store data
 * @retval      STD_NOT_OK  - buffer is full
 */
Std_StatusType Circular_PushData(Circular_t * const pCircularBuff, const uint8 data)
{
    /* Check if circular buffer has slot to store data */
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
 * @brief       Pops data from circular buffer
 * @param[in]   pCircularBuff   Pointer to circular buffer
 * @param[out]  pData           Pointer to variable to store
 * @retval      STD_OK      - Data is available in buffer
 * @retval      STD_NOT_OK  - Buffer is empty
 */
Std_StatusType Circular_PopData(Circular_t * const pCircularBuff, uint8 * pData)
{
    /* Check if circular buffer has data */
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
