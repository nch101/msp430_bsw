#include <string.h>
#include "gpio.h"

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
static Gpio_HandleISRFunc_St Gpio_gCallBackFuncPort1[GPIO_CFG_MAX_CALLBACK_FUNC1];
static Gpio_HandleISRFunc_St Gpio_gCallBackFuncPort2[GPIO_CFG_MAX_CALLBACK_FUNC2];

static uint8 Gpio_gCallbackFuncPort1Idx;
static uint8 Gpio_gCallbackFuncPort2Idx;
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

/**
 * @brief       Get GPIO register by group ID
 * @param[in]   group   Group ID
 * @retval      Gpio_GpioReg_St
 */
static Gpio_GpioReg_St GPIO_GetGPIOReg(uint8 group)
{
    volatile Gpio_GpioReg_St reg = {0};

    switch (group)
    {
        case 1U:
        {
            reg.inputReg    = (uint8 *) &P1IN;
            reg.outputReg   = (uint8 *) &P1OUT;
            reg.dirReg      = (uint8 *) &P1DIR;
            reg.resistorReg = (uint8 *) &P1REN;

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
            reg.interruptEdgeReg   = (uint8 *) &P1IES;
            reg.interruptEnableReg = (uint8 *) &P1IE;
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

            break;
        }

        case 2U:
        {
            reg.inputReg    = (uint8 *) &P2IN;
            reg.outputReg   = (uint8 *) &P2OUT;
            reg.dirReg      = (uint8 *) &P2DIR;
            reg.resistorReg = (uint8 *) &P2REN;

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
            reg.interruptEdgeReg   = (uint8 *) &P2IES;
            reg.interruptEnableReg = (uint8 *) &P2IE;
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

            break;
        }

        case 3U:
        {
            reg.inputReg    = (uint8 *) &P3IN;
            reg.outputReg   = (uint8 *) &P3OUT;
            reg.dirReg      = (uint8 *) &P3DIR;
            reg.resistorReg = (uint8 *) &P3REN;

            break;
        }

        default:
        {
            /* Do nothing */
            break;
        }
    }

    return reg;
}

/**
 * @brief       Port configuration function
 * @param[in]   pGpioCfg    Pointer to port config variable
 * @retval      None
 */
void GPIO_ConfigPort(Gpio_GpioCfg_St* pGpioCfg)
{
    pGpioCfg->reg = GPIO_GetGPIOReg(pGpioCfg->group);

    /* Config direction */
    if (pGpioCfg->direction == GPIO_OUTPUT)
    {
        *(pGpioCfg->reg.dirReg) |= pGpioCfg->pin;
    }
    else
    {
        /* Working as an input pin */
        *(pGpioCfg->reg.dirReg) &= ~(pGpioCfg->pin);

        /* Config pull mode */
        if ((pGpioCfg->pull) == GPIO_PULL_DOWN)
        {
            *(pGpioCfg->reg.resistorReg) |= pGpioCfg->pin;
            *(pGpioCfg->reg.outputReg)   &= ~(pGpioCfg->pin);
        }
        else
        {
            *(pGpioCfg->reg.resistorReg) |= pGpioCfg->pin;
            *(pGpioCfg->reg.outputReg)   |= pGpioCfg->pin;
        }
    }

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
    /* Config interrupt */
    if ((pGpioCfg->group == 1U) \
        || (pGpioCfg->group == 2U))
    {
        if (pGpioCfg->interrupt == GPIO_ENABLE_INTERRUPT)
        {
            /* Set corresponding port to 1 */
            *(pGpioCfg->reg.interruptEnableReg) |= pGpioCfg->pin;

            if (pGpioCfg->group == 1U)
            {
                Gpio_gCallBackFuncPort1[Gpio_gCallbackFuncPort1Idx].cb_func = pGpioCfg->cb_func;
                Gpio_gCallBackFuncPort1[Gpio_gCallbackFuncPort1Idx].pin     = pGpioCfg->pin;
                Gpio_gCallbackFuncPort1Idx++;
            }
            else
            {
                Gpio_gCallBackFuncPort2[Gpio_gCallbackFuncPort2Idx].cb_func = pGpioCfg->cb_func;
                Gpio_gCallBackFuncPort2[Gpio_gCallbackFuncPort2Idx].pin     = pGpioCfg->pin;
                Gpio_gCallbackFuncPort2Idx++;
            }

            if (pGpioCfg->edge == GPIO_HIGH_TO_LOW)
            {
                /* Set corresponding port to 1 */
                *(pGpioCfg->reg.interruptEdgeReg) |= pGpioCfg->pin;
            }
            else
            {
                /* Reset corresponding port to 0 */
                *(pGpioCfg->reg.interruptEdgeReg) &= ~(pGpioCfg->pin);
            }
        }
        else
        {
            /* Reset corresponding port to 0 */
            *(pGpioCfg->reg.interruptEnableReg) &= ~(pGpioCfg->pin);
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
 * @param[in]   pGpioCfg    Pointer to port config variable
 * @retval      None
 */
void GPIO_TogglePort(Gpio_GpioCfg_St* pGpioCfg)
{
    *(pGpioCfg->reg.outputReg) ^= pGpioCfg->pin;
}

/**
 * @brief       Setting port to level
 * @param[in]   pGpioCfg    Pointer to port config variable
 * @param[in]   level       High, low
 * @retval      None
 */
void GPIO_SetPort(Gpio_GpioCfg_St* pGpioCfg, Gpio_Level level)
{
    if (level == GPIO_HIGH)
    {
        *(pGpioCfg->reg.outputReg) |= pGpioCfg->pin;
    }
    else
    {
        *(pGpioCfg->reg.outputReg) &= ~(pGpioCfg->pin);
    }
}

/**
 * @brief       Read port
 * @param[in]   pGpioCfg    Pointer to port config variable
 * @retval      Gpio_Level
 */
Gpio_Level GPIO_ReadPort(Gpio_GpioCfg_St* pGpioCfg)
{
    return (*(pGpioCfg->reg.inputReg) & pGpioCfg->pin) ? GPIO_HIGH : GPIO_LOW;
}

/**
 * @brief       GPIO initialization function
 * @retval      None
 */
void GPIO_InitFunction(void)
{
    /* Initializing all ports to eliminate wasted current consumption on unused pins */
    P1DIR   = 0xFF;
    P1OUT   = 0x00;

    P2DIR   = 0xFF;
    P2OUT   = 0x00;

    P3DIR   = 0xFF;
    P3OUT   = 0x00;

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
    Gpio_gCallbackFuncPort1Idx = 0U;
    Gpio_gCallbackFuncPort2Idx = 0U;

    (void)memset(Gpio_gCallBackFuncPort1, 0, sizeof(Gpio_HandleISRFunc_St) * GPIO_CFG_MAX_CALLBACK_FUNC1);
    (void)memset(Gpio_gCallBackFuncPort2, 0, sizeof(Gpio_HandleISRFunc_St) * GPIO_CFG_MAX_CALLBACK_FUNC2);
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

}

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
/**
 * @brief       
 * @retval      None
 */
void __attribute__((weak)) GPIO_HandlerPort1ISR(volatile uint8* interruptFlag)
{
    for (uint8 index = 0; index < GPIO_CFG_MAX_CALLBACK_FUNC1; index++)
    {
        if (Gpio_gCallBackFuncPort1[index].pin & (*interruptFlag))
        {
            Gpio_gCallBackFuncPort1[index].cb_func();
        }
    }
}

/**
 * @brief       
 * @retval      None
 */
void __attribute__((weak)) GPIO_HandlerPort2ISR(volatile uint8* interruptFlag)
{
    for (uint8 index = 0; index < GPIO_CFG_MAX_CALLBACK_FUNC2; index++)
    {
        if (Gpio_gCallBackFuncPort2[index].pin & (*interruptFlag))
        {
            Gpio_gCallBackFuncPort2[index].cb_func();
        }
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) PORT1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    GPIO_HandlerPort1ISR(&P1IFG);
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
    GPIO_HandlerPort2ISR(&P2IFG);
}
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */
