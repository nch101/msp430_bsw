#ifndef GPIO_H
#define GPIO_H

#include "gpio_cfg.h"
#include "gpio_types.h"

#if (GPIO_CFG_FUNCTION == STD_ENABLED)
typedef enum __attribute__((packed))
{
    GPIO_OUTPUT             = 0U,
    GPIO_INPUT,
} Gpio_DirectionType;

typedef enum __attribute__((packed))
{
    GPIO_PULL_DOWN          = 0U,
    GPIO_PULL_UP,
} Gpio_PullModeType;

typedef enum __attribute__((packed))
{
    GPIO_DISABLE_INTERRUPT  = 0U,
    GPIO_ENABLE_INTERRUPT,
} Gpio_InterruptEnType;

typedef enum __attribute__((packed))
{
    GPIO_LOW_TO_HIGH        = 0U,
    GPIO_HIGH_TO_LOW,
} Gpio_InterruptEdgeType;

typedef enum __attribute__((packed))
{
    GPIO_LOW                = 0U,
    GPIO_HIGH,
} Gpio_LevelType;

typedef struct
{
    uint8 *pInputRegPtr;
    uint8 *pOutputRegPtr;
    uint8 *pDirRegPtr;
    uint8 *pResRegPtr;

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
    uint8 *pInterruptEdgeRegPtr;
    uint8 *pInterruptEnRegPtr;
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

} Gpio_GpioRegType;

typedef struct
{
    volatile Gpio_GpioRegType   sReg;           /* Register */
    Gpio_PullModeType           ePull;          /* Pull mode */
    Gpio_DirectionType          eDirection;     /* Port direction */

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
    void                        (*pCbFuncPtr)(void);  /* Callback function when interrupt happends */
    Gpio_InterruptEnType        eInterrupt;     /* Interrupt  */
    Gpio_InterruptEdgeType      eEdge;          /* Interrupt edge */
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

    uint8                       u8Pin;          /* Pin ID */
    uint8                       u8Group;        /* Group ID */
} Gpio_GpioCfgType;

#define GPIO_PIN_0                          0x01U
#define GPIO_PIN_1                          0x02U
#define GPIO_PIN_2                          0x04U
#define GPIO_PIN_3                          0x08U
#define GPIO_PIN_4                          0x10U
#define GPIO_PIN_5                          0x20U
#define GPIO_PIN_6                          0x40U
#define GPIO_PIN_7                          0x80U

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
extern void Gpio_HandlerPort1ISR(volatile uint8* const pInterruptFlagPtr);
extern void Gpio_HandlerPort2ISR(volatile uint8* const pInterruptFlagPtr);
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

extern void Gpio_ConfigPort(Gpio_GpioCfgType* const pGpioCfgPtr);
extern void Gpio_TogglePort(const Gpio_GpioCfgType* const pGpioCfgPtr);
extern void Gpio_SetPort(const Gpio_GpioCfgType* const pGpioCfgPtr, const Gpio_LevelType eLevel);
extern Gpio_LevelType Gpio_ReadPort(const Gpio_GpioCfgType* const pGpioCfgPtr);
extern void Gpio_InitFunction(void);

#endif /* (GPIO_CFG_FUNCTION == STD_ENABLED) */
#endif /* GPIO_H */
