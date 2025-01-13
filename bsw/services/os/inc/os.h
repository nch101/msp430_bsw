#ifndef OS_H
#define OS_H
#include "os_cfg.h"
#include "os_api.h"

#include "fls.h"
#include "gpt.h"
#include "mcu.h"
#include "uart.h"
#include "gpio.h"
#include "wdt.h"
#include "debug.h"
#include "debouncing.h"
#include "diag.h"
#include "com.h"
#include "nvm.h"

#define Os_MainFunction()                   int main(void)

#endif /* OS_H */
