#ifndef WDT_H
#define WDT_H

#include "wdt_cfg.h"

extern void Wdt_SuspendWatchdogTimer(void);
extern void Wdt_ClearWatchdogTimer(void);
extern void Wdt_StartWatchdogTimer(void);

#endif /* WDT_H */
