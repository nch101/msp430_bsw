#ifndef WDT_H
#define WDT_H

#include "wdt_cfg.h"

extern void WDT_SuspendWatchdogTimer(void);
extern void WDT_ClearWatchdogTimer(void);
extern void WDT_StartWatchdogTimer(void);

#endif /* WDT_H */
