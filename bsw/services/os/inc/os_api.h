#ifndef OS_API_H
#define OS_API_H

#include "std_types.h"

extern uint16 Os_GetSysTick(void);
extern void Os_EnableAllInterrupts(void);
extern void Os_DisableAllInterrupts(void);
extern void Os_InitApplicationTask(void);
extern void Os_ApplicationTask(void);

#endif /* OS_API_H */
