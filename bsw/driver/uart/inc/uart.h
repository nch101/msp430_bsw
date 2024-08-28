#ifndef UART_H
#define UART_H

#include "uart_cfg.h"
#include "uart_types.h"

typedef enum __attribute__((packed))
{
    UART_IDLE,
    UART_BUSY,
    UART_ERROR,
}  Uart_StatusType;

extern Uart_StatusType UART_GetTransmissionStatus(void);
extern Std_StatusType UART_GetReceivedData(uint8 * DataPtr);
extern Std_StatusType UART_TransmitData(uint8 const * DataPtr, const uint8 Length);
extern void UART_Init(void);
extern void UART_MainFunction(void);

#endif /* UART_H */
