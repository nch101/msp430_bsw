#include <string.h>
#include "gpio.h"

static Gpio_HandleISRFunc_St Gpio_gCallBackFuncPort1[GPIO_CFG_MAX_CALLBACK_FUNC1];
static Gpio_HandleISRFunc_St Gpio_gCallBackFuncPort2[GPIO_CFG_MAX_CALLBACK_FUNC2];

static uint8 Gpio_gCallbackFuncPort1Idx;
static uint8 Gpio_gCallbackFuncPort2Idx;

/**
 * @brief       Get GPIO register by group ID
 * @param[in]   group   Group ID
 * @retval      Gpio_GpioReg_St
 */
static volatile Gpio_GpioReg_St* GPIO_GetGPIOReg(uint8 group)
{
    volatile Gpio_GpioReg_St* pReg = 0U;

    switch (group)
    {
        case 1U:
        {
            pReg->inputReg    = (uint8 *) &P1IN;
            pReg->outputReg   = (uint8 *) &P1OUT;
            pReg->dirReg      = (uint8 *) &P1DIR;
            pReg->resistorReg = (uint8 *) &P1REN;
            pReg->interruptEdgeReg   = (uint8 *) &P1IES;
            pReg->interruptEnableReg = (uint8 *) &P1IE;
            break;
        }

        case 2U:
        {
            pReg->inputReg    = (uint8 *) &P2IN;
            pReg->outputReg   = (uint8 *) &P2OUT;
            pReg->dirReg      = (uint8 *) &P2DIR;
            pReg->resistorReg = (uint8 *) &P2REN;
            pReg->interruptEdgeReg   = (uint8 *) &P2IES;
            pReg->interruptEnableReg = (uint8 *) &P2IE;
            break;
        }

        case 3U:
        {
            pReg->inputReg    = (uint8 *) &P3IN;
            pReg->outputReg   = (uint8 *) &P3OUT;
            pReg->dirReg      = (uint8 *) &P3DIR;
            pReg->resistorReg = (uint8 *) &P3REN;

            break;
        }

        default:
        {
            /* Do nothing */
            break;
        }
    }

    return pReg;
}

/**
 * @brief       Port configuration function
 * @param[in]   pGpioCfg    Pointer to port config variable
 * @retval      None
 */
void GPIO_ConfigPort(Gpio_GpioCfg_St* pGpioCfg)
{
    volatile Gpio_GpioReg_St* pReg;

    pReg = GPIO_GetGPIOReg(pGpioCfg->group);

    pGpioCfg->reg = pReg;

    /* Config direction */
    if (pGpioCfg->direction == GPIO_INPUT)
    {
        *(pReg)->dirReg &= ~(pGpioCfg->pin);
    }
    else
    {
        *(pReg)->dirReg |= pGpioCfg->pin;
    }

    /* Config pull mode */
    if ((pGpioCfg->pull) == GPIO_PULL_UP)
    {
        *(pReg)->resistorReg |= pGpioCfg->pin;
        *(pReg)->outputReg   |= pGpioCfg->pin;
    }
    else if ((pGpioCfg->pull) == GPIO_PULL_DOWN)
    {
        *(pReg)->resistorReg |= pGpioCfg->pin;
        *(pReg)->outputReg   &= ~(pGpioCfg->pin);
    }
    else
    {
        *(pReg)->resistorReg &= ~(pGpioCfg->pin);
    }

    /* Config interrupt */
    if ((pGpioCfg->group == 1U) \
        || (pGpioCfg->group == 2U))
    {
        if (pGpioCfg->interrupt == GPIO_ENABLE_INTERRUPT)
        {
            /* Set corresponding port to 1 */
            *(pReg)->interruptEnableReg |= pGpioCfg->pin;

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
                *(pReg)->interruptEdgeReg |= pGpioCfg->pin;
            }
            else
            {
                /* Reset corresponding port to 0 */
                *(pReg)->interruptEdgeReg &= ~(pGpioCfg->pin);
            }
        }
        else
        {
            /* Reset corresponding port to 0 */
            *(pReg)->interruptEnableReg &= ~(pGpioCfg->pin);
        }
    }
    else
    {
        /* Hardware does not support. Do nothing */
    }
}

/**
 * @brief       Port toggling function
 * @param[in]   pGpioCfg    Pointer to port config variable
 * @retval      None
 */
void GPIO_TogglePort(Gpio_GpioCfg_St* pGpioCfg)
{
    *(pGpioCfg->reg)->outputReg ^= pGpioCfg->pin;
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
        *(pGpioCfg->reg)->outputReg |= pGpioCfg->pin;
    }
    else
    {
        *(pGpioCfg->reg)->outputReg &= ~(pGpioCfg->pin);
    }
}

/**
 * @brief       Read port
 * @param[in]   pGpioCfg    Pointer to port config variable
 * @retval      Gpio_Level
 */
Gpio_Level GPIO_ReadPort(Gpio_GpioCfg_St* pGpioCfg)
{
    return (*(pGpioCfg->reg)->inputReg & pGpioCfg->pin) ? GPIO_HIGH : GPIO_LOW;
}

/**
 * @brief       GPIO initialization function
 * @retval      None
 */
void GPIO_InitFunction(void)
{
    Gpio_gCallbackFuncPort1Idx = 0U;
    Gpio_gCallbackFuncPort2Idx = 0U;

    (void)memset(Gpio_gCallBackFuncPort1, 0, sizeof(Gpio_HandleISRFunc_St) * GPIO_CFG_MAX_CALLBACK_FUNC1);
    (void)memset(Gpio_gCallBackFuncPort2, 0, sizeof(Gpio_HandleISRFunc_St) * GPIO_CFG_MAX_CALLBACK_FUNC2);
}

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
