#ifndef GPT_H
#define GPT_H

#include "os_api.h"
#include "gpt_cfg.h"

extern void Gpt_StartTimer(void);
extern void Gpt_StopTimer(void);
extern void Gpt_InitFunction(void (*vFuncCallback)(void));

#endif /* GPT_H */
