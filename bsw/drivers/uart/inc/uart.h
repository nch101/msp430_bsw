#ifndef UART_H
#define UART_H

#include "uart_cfg.h"

#if (UART_CFG_FUNCTION == STD_ENABLED)
typedef enum __attribute__((packed))
{
    UART_IDLE,
    UART_BUSY,
    UART_ERROR,
}  Uart_StatusType;

extern void Uart_ResetRxDataIndex(void);
extern Std_StatusType Uart_ConfigReceiveDataBuffer(uint8 * const DataPtr, const uint8 DataLength);
extern Uart_StatusType Uart_GetTransmissionStatus(void);
extern Std_StatusType Uart_TransmitData(uint8 const * const DataPtr, const uint8 DataLength);
extern void Uart_InitFunction(void);
extern void Uart_MainFunction(void);

#endif /* (UART_CFG_FUNCTION == STD_ENABLED) */
#endif /* UART_H */
