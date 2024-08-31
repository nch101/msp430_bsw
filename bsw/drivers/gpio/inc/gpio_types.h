#ifndef GPIO_TYPES_H
#define GPIO_TYPES_H

#include "std_types.h"

typedef struct
{
    void  (*cb_func)(void); 
    uint8 pin;                    /* Pin ID */
} Gpio_HandleISRFunc_St;

#endif /* GPIO_TYPES_H */
