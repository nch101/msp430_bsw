#include "gpio.h"

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
#include <string.h>
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */


#if (GPIO_CFG_FUNCTION == STD_ENABLED)
#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
static Gpio_HandleISRFuncType Gpio_aCallBackFuncPort1[GPIO_CFG_MAX_CALLBACK_FUNC1];
static Gpio_HandleISRFuncType Gpio_aCallBackFuncPort2[GPIO_CFG_MAX_CALLBACK_FUNC2];

static uint8 Gpio_u8CbFuncPort1Idx;
static uint8 Gpio_u8CbFuncPort2Idx;
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

/**
 * @brief       Get GPIO register by group ID
 * @param[in]   u8Group         Group ID
 * @retval      Gpio_GpioRegType
 */
static Gpio_GpioRegType Gpio_GetGPIOReg(const uint8 u8Group)
{
    volatile Gpio_GpioRegType sReg = {0};

    switch (u8Group)
    {
        case 1U:
        {
            sReg.pInputRegPtr   = (uint8 *) &P1IN;
            sReg.pOutputRegPtr  = (uint8 *) &P1OUT;
            sReg.pDirRegPtr     = (uint8 *) &P1DIR;
            sReg.pResRegPtr     = (uint8 *) &P1REN;

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
            sReg.pInterruptEdgeRegPtr   = (uint8 *) &P1IES;
            sReg.pInterruptEnRegPtr     = (uint8 *) &P1IE;
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

            break;
        }

        case 2U:
        {
            sReg.pInputRegPtr   = (uint8 *) &P2IN;
            sReg.pOutputRegPtr  = (uint8 *) &P2OUT;
            sReg.pDirRegPtr     = (uint8 *) &P2DIR;
            sReg.pResRegPtr     = (uint8 *) &P2REN;

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
            sReg.pInterruptEdgeRegPtr   = (uint8 *) &P2IES;
            sReg.pInterruptEnRegPtr     = (uint8 *) &P2IE;
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

            break;
        }

        case 3U:
        {
            sReg.pInputRegPtr   = (uint8 *) &P3IN;
            sReg.pOutputRegPtr  = (uint8 *) &P3OUT;
            sReg.pDirRegPtr     = (uint8 *) &P3DIR;
            sReg.pResRegPtr     = (uint8 *) &P3REN;

            break;
        }

        default:
        {
            /* Do nothing */
            break;
        }
    }

    return sReg;
}

/**
 * @brief       Port configuration function
 * @param[in]   pGpioCfgPtr     Pointer to port config variable
 * @retval      None
 */
void Gpio_ConfigPort(Gpio_GpioCfgType* const pGpioCfgPtr)
{
    pGpioCfgPtr->sReg = Gpio_GetGPIOReg(pGpioCfgPtr->u8Group);

    /* Config direction */
    if (pGpioCfgPtr->eDirection == GPIO_OUTPUT)
    {
        *(pGpioCfgPtr->sReg.pDirRegPtr) |= pGpioCfgPtr->u8Pin;
    }
    else
    {
        /* Working as an input pin */
        *(pGpioCfgPtr->sReg.pDirRegPtr) &= ~(pGpioCfgPtr->u8Pin);

        /* Config pull mode */
        if ((pGpioCfgPtr->ePull) == GPIO_PULL_DOWN)
        {
            *(pGpioCfgPtr->sReg.pResRegPtr)     |= pGpioCfgPtr->u8Pin;
            *(pGpioCfgPtr->sReg.pOutputRegPtr)  &= ~(pGpioCfgPtr->u8Pin);
        }
        else
        {
            *(pGpioCfgPtr->sReg.pResRegPtr)     |= pGpioCfgPtr->u8Pin;
            *(pGpioCfgPtr->sReg.pOutputRegPtr)  |= pGpioCfgPtr->u8Pin;
        }
    }

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
    /* Config interrupt */
    if ((pGpioCfgPtr->u8Group == 1U) \
        || (pGpioCfgPtr->u8Group == 2U))
    {
        if (pGpioCfgPtr->eInterrupt == GPIO_ENABLE_INTERRUPT)
        {
            /* Set corresponding port to 1 */
            *(pGpioCfgPtr->sReg.pInterruptEnRegPtr) |= pGpioCfgPtr->u8Pin;

            if (pGpioCfgPtr->u8Group == 1U)
            {
                Gpio_aCallBackFuncPort1[Gpio_u8CbFuncPort1Idx].pCbFuncPtr = pGpioCfgPtr->pCbFuncPtr;
                Gpio_aCallBackFuncPort1[Gpio_u8CbFuncPort1Idx].u8Pin  = pGpioCfgPtr->u8Pin;
                Gpio_u8CbFuncPort1Idx++;
            }
            else
            {
                Gpio_aCallBackFuncPort2[Gpio_u8CbFuncPort2Idx].pCbFuncPtr = pGpioCfgPtr->pCbFuncPtr;
                Gpio_aCallBackFuncPort2[Gpio_u8CbFuncPort2Idx].u8Pin  = pGpioCfgPtr->u8Pin;
                Gpio_u8CbFuncPort2Idx++;
            }

            if (pGpioCfgPtr->eEdge == GPIO_HIGH_TO_LOW)
            {
                /* Set corresponding port to 1 */
                *(pGpioCfgPtr->sReg.pInterruptEdgeRegPtr) |= pGpioCfgPtr->u8Pin;
            }
            else
            {
                /* Reset corresponding port to 0 */
                *(pGpioCfgPtr->sReg.pInterruptEdgeRegPtr) &= ~(pGpioCfgPtr->u8Pin);
            }
        }
        else
        {
            /* Reset corresponding port to 0 */
            *(pGpioCfgPtr->sReg.pInterruptEnRegPtr) &= ~(pGpioCfgPtr->u8Pin);
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
 * @param[in]   pGpioCfgPtr     Pointer to port config variable
 * @retval      None
 */
void Gpio_TogglePort(const Gpio_GpioCfgType* const pGpioCfgPtr)
{
    *(pGpioCfgPtr->sReg.pOutputRegPtr) ^= pGpioCfgPtr->u8Pin;
}

/**
 * @brief       Setting port to level
 * @param[in]   pGpioCfgPtr    Pointer to port config variable
 * @param[in]   level       High, low
 * @retval      None
 */
void Gpio_SetPort(const Gpio_GpioCfgType* const pGpioCfgPtr, const Gpio_LevelType level)
{
    if (level == GPIO_HIGH)
    {
        *(pGpioCfgPtr->sReg.pOutputRegPtr)  |= pGpioCfgPtr->u8Pin;
    }
    else
    {
        *(pGpioCfgPtr->sReg.pOutputRegPtr)  &= ~(pGpioCfgPtr->u8Pin);
    }
}

/**
 * @brief       Read port
 * @param[in]   pGpioCfgPtr    Pointer to port config variable
 * @retval      Gpio_LevelType
 */
Gpio_LevelType Gpio_ReadPort(const Gpio_GpioCfgType* const pGpioCfgPtr)
{
    return (*(pGpioCfgPtr->sReg.pInputRegPtr) & pGpioCfgPtr->u8Pin) ? GPIO_HIGH : GPIO_LOW;
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

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
    Gpio_u8CbFuncPort1Idx = 0U;
    Gpio_u8CbFuncPort2Idx = 0U;

    (void)memset(Gpio_aCallBackFuncPort1, 0, sizeof(Gpio_HandleISRFuncType) * GPIO_CFG_MAX_CALLBACK_FUNC1);
    (void)memset(Gpio_aCallBackFuncPort2, 0, sizeof(Gpio_HandleISRFuncType) * GPIO_CFG_MAX_CALLBACK_FUNC2);
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

}

#if (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED)
/**
 * @brief       
 * @retval      None
 */
void __attribute__((weak)) Gpio_HandlerPort1ISR(volatile uint8* const pInterruptFlagPtr)
{
    for (uint8 index = 0; index < GPIO_CFG_MAX_CALLBACK_FUNC1; index++)
    {
        if (Gpio_aCallBackFuncPort1[index].u8Pin & (*pInterruptFlagPtr))
        {
            Gpio_aCallBackFuncPort1[index].pCbFuncPtr();
        }
    }
}

/**
 * @brief       
 * @retval      None
 */
void __attribute__((weak)) Gpio_HandlerPort2ISR(volatile uint8* const pInterruptFlagPtr)
{
    for (uint8 index = 0; index < GPIO_CFG_MAX_CALLBACK_FUNC2; index++)
    {
        if (Gpio_aCallBackFuncPort2[index].u8Pin & (*pInterruptFlagPtr))
        {
            Gpio_aCallBackFuncPort2[index].pCbFuncPtr();
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
    Gpio_HandlerPort1ISR(&P1IFG);
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
    Gpio_HandlerPort2ISR(&P2IFG);
}
#endif /* (GPIO_CFG_INTERRUPT_FUNCTION == STD_ENABLED) */

#endif /* (GPIO_CFG_FUNCTION == STD_ENABLED) */
