#ifndef UART_H
#define UART_H

#include "uart_cfg.h"

typedef enum __attribute__((packed))
{
    UART_IDLE,
    UART_BUSY,
    UART_ERROR,
}  UART_StatusType;

extern void UART_ResetRxDataIndex(void);
extern Std_StatusType UART_ConfigReceiveDataBuffer(uint8 * DataPtr, const uint8 DataLength);
extern UART_StatusType UART_GetTransmissionStatus(void);
extern Std_StatusType UART_TransmitData(uint8 const * DataPtr, const uint8 DataLength);
extern void UART_InitFunction(void);
extern void UART_MainFunction(void);

#endif /* UART_H */
