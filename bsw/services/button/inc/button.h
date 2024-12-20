#ifndef BUTTON_H
#define BUTTON_H

#include "gpio.h"
#include "button_cfg.h"

#if (BSW_CFG_BUTTON_FUNCTION == STD_ENABLED)

typedef enum __attribute__((packed))
{
    BUTTON_SWITCHING_MODE,
    BUTTON_MAX_BUTTON_ID_PRESENT,
} Button_ButtonID;

typedef enum __attribute__((packed))
{
    BUTTON_STABLE,
    BUTTON_DEBOUNCING,
} Button_StatusType;

typedef enum __attribute__((packed))
{
    BUTTON_INACTIVE,
    BUTTON_ACTIVE,
} Button_StateType;

typedef struct
{
    Gpio_GpioCfgType*   pGpioCfgPtr;
    Button_StatusType   status;
    Gpio_LevelType      stableState;
    Gpio_LevelType      activeState;
} Button_ButtonPrototype;

extern void Button_ConfigButton ( \
    const Button_ButtonID eButtonID, \
    Gpio_GpioCfgType* const pGpioPin, \
    const Gpio_LevelType eActiveState);

extern Button_StateType Button_ReadButton (const Button_ButtonID eButtonID);

#endif /* (BSW_CFG_BUTTON_FUNCTION == STD_ENABLED) */
#endif /* BUTTON_H */
