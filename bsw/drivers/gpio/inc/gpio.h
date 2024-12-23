#ifndef GPIO_H
#define GPIO_H

#include "gpio_cfg.h"

#if (BSW_CFG_GPIO_FUNCTION == STD_ENABLED)
typedef enum __attribute__((packed))
{
    GPIO_UNINIT_PIN         = 0x00U,
    GPIO_PIN_0              = 0x01U,
    GPIO_PIN_1              = 0x02U,
    GPIO_PIN_2              = 0x04U,
    GPIO_PIN_3              = 0x08U,
    GPIO_PIN_4              = 0x10U,
    GPIO_PIN_5              = 0x20U,
    GPIO_PIN_6              = 0x40U,
    GPIO_PIN_7              = 0x80U,
} Gpio_PinType;

typedef enum __attribute__((packed))
{
    GPIO_GROUP_1,
    GPIO_GROUP_2,
    GPIO_GROUP_3,
    GPIO_MAX_GPIO_GROUP,
} Gpio_GroupType;

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
} Gpio_ItrrptEnType;

typedef enum __attribute__((packed))
{
    GPIO_LOW_TO_HIGH        = 0U,
    GPIO_HIGH_TO_LOW,
} Gpio_ItrrptEdgeType;

typedef enum __attribute__((packed))
{
    GPIO_LOW                = 0U,
    GPIO_HIGH,
} Gpio_LevelType;

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
typedef void (*Gpio_CallbackFn)(void);

typedef struct
{
    Gpio_CallbackFn         vCallbackFn[GPIO_CFG_MAX_CALLBACK_FN];  /* Pointer to callback function */
    Gpio_PinType            ePin[GPIO_CFG_MAX_CALLBACK_FN];         /* Pin ID */
    uint8                   u8Index;        /*  */
} Gpio_CbFnAttriType;

typedef struct 
{
    Gpio_CallbackFn         vCallbackFn;    /* Pointer to callback function */
    Gpio_ItrrptEdgeType     eEdge;          /* Interrupt edge */
    Gpio_ItrrptEnType       eItrruptEn;     /* Enable interrupt */
} Gpio_ItrrptAttriType;
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

typedef struct
{
    volatile uint8 *pInputRegPtr;
    volatile uint8 *pOutputRegPtr;
    volatile uint8 *pDirRegPtr;
    volatile uint8 *pResRegPtr;

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
    volatile uint8 *pItrrptEdgeRegPtr;
    volatile uint8 *pItrrptEnRegPtr;
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */
} Gpio_GpioRegType;

typedef struct 
{
#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
    Gpio_ItrrptAttriType    sItrrpt;        /* Interrupt attribute */
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

    Gpio_PullModeType       ePull;          /* Pull mode */
    Gpio_DirectionType      eDirection;     /* Port direction */
    Gpio_PinType            ePin;           /* Pin ID */
    Gpio_GroupType          eGroup;         /* Group ID */
} Gpio_GpioCfgType;

extern void Gpio_ConfigPort(Gpio_GpioCfgType* const pGpioCfgPtr);
extern void Gpio_TogglePort(const Gpio_GpioCfgType* const pGpioCfgPtr);
extern void Gpio_SetPort(const Gpio_GpioCfgType* const pGpioCfgPtr, const Gpio_LevelType eLevel);
extern Gpio_LevelType Gpio_ReadPort(const Gpio_GpioCfgType* const pGpioCfgPtr);
extern void Gpio_InitFunction(void);

#endif /* (BSW_CFG_GPIO_FUNCTION == STD_ENABLED) */
#endif /* GPIO_H */
