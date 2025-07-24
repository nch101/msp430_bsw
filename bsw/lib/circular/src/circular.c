#include <string.h>
#include "circular.h"

#if (BSW_CFG_CIRCULAR_FUNCTION == STD_ENABLED)
/**
 * @brief Corrects the current index within a circular buffer.
 *
 * This function updates the value pointed to by pCurrentIndex by adding u8Len.
 * If the result exceeds or equals the maximum size (u8MaxSize), the index is reset to 0,
 * effectively wrapping around the circular buffer.
 *
 * @param[in,out] pCurrentIndex Pointer to the current index to be updated.
 * @param[in]     u8MaxSize     Maximum size of the circular buffer.
 * @param[in]     u8Len         Length to increment the current index by.
 */
static void Circular_CorrectIndex(uint8 * const pCurrentIndex, const uint8 u8MaxSize, const uint8 u8Len)
{
    if ((*pCurrentIndex + u8Len) < u8MaxSize)
    {
        (*pCurrentIndex) += u8Len;
    }
    else
    {
        (*pCurrentIndex) = 0;
    }
}

/**
 * @brief Initializes a circular buffer structure.
 *
 * This function sets up the circular buffer by assigning the provided buffer pointer,
 * initializing the read and write indices to zero, and setting the available size and
 * capacity to the specified value.
 *
 * @param[in,out] pCircularBuff Pointer to the Circular_t structure to initialize.
 * @param[in]     pBuffer       Pointer to the memory buffer to use for storage.
 * @param[in]     u8Capacity    The total capacity of the buffer.
 *
 * @return STD_OK on successful initialization.
 */
Std_StatusType Circular_InitBuffer(Circular_t * const pCircularBuff, uint8 * const pBuffer, uint8 const u8Capacity)
{
    pCircularBuff->pBuffer      = pBuffer;
    pCircularBuff->readIdx      = 0;
    pCircularBuff->writeIdx     = 0;
    pCircularBuff->availSize    = u8Capacity;
    pCircularBuff->capacity     = u8Capacity;

    return STD_OK;
}

/**
 * @brief Pushes data into the circular buffer.
 *
 * This function attempts to copy a block of data into the circular buffer.
 * It first checks if there is enough available space to store the data.
 * If sufficient space is available, the data is copied to the buffer at the current write index,
 * the available size is updated, and the write index is corrected to wrap around the buffer if necessary.
 *
 * @param[in,out] pCircularBuff Pointer to the circular buffer structure.
 * @param[in]     pData         Pointer to the data to be pushed into the buffer.
 * @param[in]     u8Len         Length of the data to be pushed.
 *
 * @return STD_OK if the data was successfully pushed into the buffer,
 *         STD_NOT_OK if there was not enough space to store the data.
 */
Std_StatusType Circular_PushData(Circular_t * const pCircularBuff, uint8 const * const pData, const uint8 u8Len)
{
    /* Check if circular buffer has slot to store data */
    if (pCircularBuff->availSize > u8Len)
    {
        memcpy((void*) &pCircularBuff->pBuffer[pCircularBuff->writeIdx], (void *) pData, u8Len);
        pCircularBuff->availSize -= u8Len;

        Circular_CorrectIndex(&(pCircularBuff->writeIdx), pCircularBuff->capacity, u8Len);

        return STD_OK;
    }

    return STD_NOT_OK;
}

/**
 * @brief Pops data from the circular buffer.
 *
 * This function retrieves up to `u8Len` bytes of data from the circular buffer pointed to by `pCircularBuff`
 * and copies it to the memory location pointed to by `pData`. It checks if there is enough available data
 * in the buffer before performing the operation. After copying, it updates the buffer's available size and
 * read index accordingly.
 *
 * @param[in,out] pCircularBuff Pointer to the circular buffer structure.
 * @param[in]     u8Len         Number of bytes to pop from the buffer.
 * @param[out]    pData         Pointer to the memory where the popped data will be stored.
 *
 * @return STD_OK if the operation was successful, STD_NOT_OK otherwise.
 */
Std_StatusType Circular_PopData(Circular_t * const pCircularBuff, const uint8 u8Len, uint8 * pData)
{
    /* Check if circular buffer has data */
    if ((pCircularBuff->availSize < pCircularBuff->capacity)
        && ((pCircularBuff->capacity - pCircularBuff->availSize) >= u8Len))
    {
        memcpy((void *) pData, (void*) &pCircularBuff->pBuffer[pCircularBuff->readIdx], u8Len);
        pCircularBuff->availSize += u8Len;

        Circular_CorrectIndex(&(pCircularBuff->readIdx), pCircularBuff->capacity, u8Len);

        return STD_OK;
    }

    return STD_NOT_OK;
}

#endif /* BSW_CFG_CIRCULAR_FUNCTION */
