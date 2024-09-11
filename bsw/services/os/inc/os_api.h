#ifndef OS_API_H
#define OS_API_H

extern void OS_EnableAllInterrupts(void);
extern void OS_DisableAllInterrupts(void);
extern void OS_InitApplicationTask(void);
extern void OS_ApplicationTask(void);
extern inline void OS_ProcessTiming(void);

#endif /* OS_API_H */
