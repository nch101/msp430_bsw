#ifndef UART_H
#define UART_H

#include "uart_cfg.h"

#if (BSW_CFG_UART_FUNCTION == STD_ENABLED)
typedef enum __attribute__((packed))
{
    UART_IDLE,
    UART_BUSY,
    UART_ERROR,
}  Uart_StatusType;

extern Uart_StatusType Uart_GetTransmissionStatus(void);
extern Std_StatusType Uart_TransmitData(uint8 const * const DataPtr, const uint8 DataLength);
extern void Uart_ConfigReceiveDataCallBack(void (*vFuncCallBack)(volatile uint8 *));
extern void Uart_InitFunction(void);

#endif /* (BSW_CFG_UART_FUNCTION == STD_ENABLED) */
#endif /* UART_H */
