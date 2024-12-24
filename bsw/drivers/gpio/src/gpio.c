#include "debug.h"
#include "gpio.h"

#if (BSW_CFG_GPIO_FUNCTION == STD_ENABLED)
static const Gpio_GpioRegType Gpio_sGroupRegs[GPIO_MAX_GPIO_GROUP] =
    {
        {
            &P1IN,
            &P1OUT,
            &P1DIR,
            &P1REN,
        #if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
            &P1IES,
            &P1IE,
        #endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */
        },
        {
            &P2IN,
            &P2OUT,
            &P2DIR,
            &P2REN,
        #if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
            &P2IES,
            &P2IE,
        #endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */
        },
        {
            &P3IN,
            &P3OUT,
            &P3DIR,
            &P3REN,
        #if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
            NULL,
            NULL,
        #endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */
        }
    };

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
static Gpio_CbFnAttriType    Gpio_CallbackFnAtt[2U];
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

/**
 * @brief       Port configuration function
 * @param[in]   pGpioCfgPtr     Pointer to port config variable
 * @retval      None
 */
void Gpio_ConfigPort(Gpio_GpioCfgType* const pGpioCfgPtr)
{
    Gpio_GpioRegType    sRegisters      = Gpio_sGroupRegs[pGpioCfgPtr->eGroup];
#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
    Gpio_CbFnAttriType  sCallbackFnAtt  = Gpio_CallbackFnAtt[pGpioCfgPtr->eGroup];
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

    /* Config direction */
    if (pGpioCfgPtr->eDirection == GPIO_OUTPUT)
    {
        *(sRegisters.pDirRegPtr)    |= (uint8) pGpioCfgPtr->ePin;
    }
    else
    {
        /* Working as an input pin */
        *(sRegisters.pDirRegPtr)    &= ~((uint8) pGpioCfgPtr->ePin);
        /* Enable pull mode */
        *(sRegisters.pResRegPtr)    |= (uint8) pGpioCfgPtr->ePin;

        /* Config pull mode */
        if ((pGpioCfgPtr->ePull) == GPIO_PULL_DOWN)
        {
            *(sRegisters.pOutputRegPtr) &= ~((uint8) pGpioCfgPtr->ePin);
        }
        else
        {
            *(sRegisters.pOutputRegPtr) |= (uint8) pGpioCfgPtr->ePin;
        }
    }

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
    if ((pGpioCfgPtr->eGroup == GPIO_GROUP_1) \
        || (pGpioCfgPtr->eGroup == GPIO_GROUP_2))
    {
        if (pGpioCfgPtr->sItrrpt.eItrruptEn == GPIO_ENABLE_INTERRUPT)
        {
            /* Set corresponding port to 1 */
            *(sRegisters.pItrrptEnRegPtr)   |= (uint8) pGpioCfgPtr->ePin;

            if (pGpioCfgPtr->sItrrpt.eEdge == GPIO_HIGH_TO_LOW)
            {
                /* Set corresponding port to 1 */
                *(sRegisters.pItrrptEdgeRegPtr) |= (uint8) pGpioCfgPtr->ePin;
            }
            else
            {
                /* Reset corresponding port to 0 */
                *(sRegisters.pItrrptEdgeRegPtr) &= ~((uint8) pGpioCfgPtr->ePin);
            }

            if (sCallbackFnAtt.u8Index < GPIO_CFG_MAX_CALLBACK_FN)
            {
                sCallbackFnAtt.vCallbackFn[sCallbackFnAtt.u8Index]  = pGpioCfgPtr->sItrrpt.vCallbackFn;
                sCallbackFnAtt.ePin[sCallbackFnAtt.u8Index]         = pGpioCfgPtr->ePin;
                sCallbackFnAtt.u8Index++;
            }
            else
            {
                DEBUG_WARN(GPIO, REACHED_MAX_CALLBACK_FUNCTION);
            }
        }
        else
        {
            /* Reset corresponding port to 0 */
            *(sRegisters.pItrrptEnRegPtr)   &= ~((uint8) pGpioCfgPtr->ePin);
        }
    }
    else
    {
        /* Hardware does not support. Do nothing */
    }
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */
}

/**
 * @brief       Port toggling function
 * @param[in]   pGpioCfgPtr     Pointer to port configuration variable
 * @retval      None
 */
void Gpio_TogglePort(const Gpio_GpioCfgType* const pGpioCfgPtr)
{
    Gpio_GpioRegType    sRegisters  = Gpio_sGroupRegs[pGpioCfgPtr->eGroup];

    *(sRegisters.pOutputRegPtr)     ^= (uint8) pGpioCfgPtr->ePin;
}

/**
 * @brief       Setting port to level
 * @param[in]   pGpioCfgPtr     Pointer to port configuration variable
 * @param[in]   eLevel          GPIO_HIGH, GPIO_LOW
 * @retval      None
 */
void Gpio_SetPort(const Gpio_GpioCfgType* const pGpioCfgPtr, const Gpio_LevelType eLevel)
{
    Gpio_GpioRegType    sRegisters  = Gpio_sGroupRegs[pGpioCfgPtr->eGroup];

    if (eLevel == GPIO_HIGH)
    {
        *(sRegisters.pOutputRegPtr) |= (uint8) pGpioCfgPtr->ePin;
    }
    else
    {
        *(sRegisters.pOutputRegPtr) &= ~((uint8) pGpioCfgPtr->ePin);
    }
}

/**
 * @brief       Read port
 * @param[in]   pGpioCfgPtr    Pointer to port config variable
 * @retval      GPIO_HIGH
 * @retval      GPIO_LOW
 */
Gpio_LevelType Gpio_ReadPort(const Gpio_GpioCfgType* const pGpioCfgPtr)
{
    Gpio_GpioRegType    sRegisters  = Gpio_sGroupRegs[pGpioCfgPtr->eGroup];

    if (*(sRegisters.pInputRegPtr) & ((uint8) pGpioCfgPtr->ePin))
    {
        return GPIO_HIGH;
    }
    else
    {
        return GPIO_LOW;
    }
}

/**
 * @brief       GPIO initialization function
 * @retval      None
 */
void Gpio_InitFunction(void)
{
    /* Initializing all ports to eliminate wasted current consumption on unused pins */
    P1DIR = 0xFF;
    P1OUT = 0x00;

    P2DIR = 0xFF;
    P2OUT = 0x00;

    P3DIR = 0xFF;
    P3OUT = 0x00;
}

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) PORT1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    for (uint8 index = 0; index < GPIO_CFG_MAX_CALLBACK_FN; index++)
    {
        if (Gpio_CallbackFnAtt[GPIO_GROUP_1].ePin[index] & P1IFG)
        {
            Gpio_CallbackFnAtt[GPIO_GROUP_1].vCallbackFn[index]();
        }
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) PORT2_ISR (void)
#else
#error Compiler not supported!
#endif
{
    for (uint8 index = 0; index < GPIO_CFG_MAX_CALLBACK_FN; index++)
    {
        if (Gpio_CallbackFnAtt[GPIO_GROUP_2].ePin[index] & P2IFG)
        {
            Gpio_CallbackFnAtt[GPIO_GROUP_2].vCallbackFn[index]();
        }
    }
}
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */
#endif /* (BSW_CFG_GPIO_FUNCTION == STD_ENABLED) */
