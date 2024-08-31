#ifndef GPIO_H
#define GPIO_H

#include "gpio_cfg.h"
#include "gpio_types.h"

typedef enum __attribute__((packed))
{
    GPIO_INPUT,
    GPIO_OUTPUT,
} Gpio_Direction;

typedef enum __attribute__((packed))
{
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
    GPIO_PULL_OFF,
} Gpio_PullMode;

typedef enum __attribute__((packed))
{
    GPIO_ENABLE_INTERRUPT,
    GPIO_DISABLE_INTERRUPT,
} Gpio_InterruptEn;

typedef enum __attribute__((packed))
{
    GPIO_LOW_TO_HIGH,
    GPIO_HIGH_TO_LOW,
} Gpio_InterruptEdge;

typedef enum __attribute__((packed))
{
    GPIO_LOW,
    GPIO_HIGH,
} Gpio_Level;

typedef struct
{
    uint8 *inputReg;
    uint8 *outputReg;
    uint8 *dirReg;
    uint8 *resistorReg;
    uint8 *interruptEdgeReg;
    uint8 *interruptEnableReg;
} Gpio_GpioReg_St;

typedef struct
{
    void                        (*cb_func)(void);       /* Callback function when interrupt happends */
    volatile Gpio_GpioReg_St    *reg;                   /* Register */
    Gpio_PullMode               pull;                   /* Pull mode */
    Gpio_Direction              direction;              /* Port direction */
    Gpio_InterruptEn            interrupt;              /* Interrupt  */
    Gpio_InterruptEdge          edge;                   /* Interrupt edge */
    uint8                       pin;                    /* Pin ID */
    uint8                       group;                  /* Group ID */
} Gpio_GpioCfg_St;

#define GPIO_PIN_0                          0x01U
#define GPIO_PIN_1                          0x02U
#define GPIO_PIN_2                          0x04U
#define GPIO_PIN_3                          0x08U
#define GPIO_PIN_4                          0x10U
#define GPIO_PIN_5                          0x20U
#define GPIO_PIN_6                          0x40U
#define GPIO_PIN_7                          0x80U

extern void GPIO_HandlerPort1ISR(volatile uint8* interruptFlag);
extern void GPIO_HandlerPort2ISR(volatile uint8* interruptFlag);
extern void GPIO_ConfigPort(Gpio_GpioCfg_St* pGpioCfg);
extern void GPIO_TogglePort(Gpio_GpioCfg_St* pGpioCfg);
extern void GPIO_SetPort(Gpio_GpioCfg_St* pGpioCfg, Gpio_Level level);
extern Gpio_Level GPIO_ReadPort(Gpio_GpioCfg_St* pGpioCfg);
extern void GPIO_InitFunction(void);

#endif /* GPIO_H */
