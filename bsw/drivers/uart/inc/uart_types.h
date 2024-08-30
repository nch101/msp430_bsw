#ifndef UART_TYPES_H
#define UART_TYPES_H

#include "std_types.h"

typedef enum __attribute__((packed))
{
    UART_BUFFER_EMPTY,
    UART_BUFFER_READY,
} Uart_BuffStatusType;

typedef struct
{
    uint8 const * buffPtr;
    uint16 length;
    uint16 curIndex;
} Uart_UartTxBuff_St;

typedef struct  
{
    uint8*              buffPtr;
    uint16              curIndex;
    Uart_BuffStatusType status;
} Uart_UartRxBuff_St;

#endif /* UART_TYPES_H */
