#ifndef DEBOUNCING_H
#define DEBOUNCING_H

#include "gpio.h"
#include "debouncing_cfg.h"

#if (BSW_CFG_DEBOUNCING_FUNCTION == STD_ENABLED)
typedef enum __attribute__((packed)) Debounce_DeviceID
{
    DEBOUNCE_SWITCHING_MODE,
    DEBOUNCE_MAX_DEVICE_ID_PRESENT,
} Debounce_DeviceID;

typedef struct Debounce_DebounceAttributes
{
    Gpio_GpioCfgType*   pGpioCfgPtr;
    Gpio_LevelType      eOldState;
    Gpio_LevelType      eStableState;
} Debounce_DebounceAttributes;

extern void Debounce_ConfigureDevice(const Debounce_DeviceID eDeviceID, Gpio_GpioCfgType* const pGpioPin);
extern Gpio_LevelType Debounce_ReadDeviceState(const Debounce_DeviceID eDeviceID);

#endif /* (BSW_CFG_DEBOUNCING_FUNCTION == STD_ENABLED) */
#endif /* DEBOUNCING_H */
