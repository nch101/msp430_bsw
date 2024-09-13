#ifndef GPIO_TYPES_H
#define GPIO_TYPES_H

#include "std_types.h"

#if (BSW_CFG_GPIO_FUNCTION == STD_ENABLED)
typedef struct
{
    void  (*pCbFuncPtr)(void);  /* Pointer to callback function */
    uint8 u8Pin;                /* Pin ID */
} Gpio_HandleISRFuncType;

#endif /* (BSW_CFG_GPIO_FUNCTION == STD_ENABLED) */
#endif /* GPIO_TYPES_H */
