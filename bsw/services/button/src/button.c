#include "timer.h"
#include "debug.h"
#include "button.h"

#if (BSW_CFG_BUTTON_FUNCTION == STD_ENABLED)
static Button_ButtonPrototype Button[BUTTON_MAX_BUTTON_ID_PRESENT];

/**
 * @brief       Debounces button
 * @param[in]   eButtonID       ButtonID
 * @param[in]   eNewState       Current button state
 * @retval      None
 */
static void Button_DebounceButton(const Button_ButtonID eButtonID, const Gpio_LevelType eNewState)
{
    switch (Button[eButtonID].status)
    {
        case BUTTON_STABLE:
        {
            if (eNewState != Button[eButtonID].stableState)
            {
                Timer_SetTimer(TIMER_DEBOUNCE, BUTTON_CFG_DEBOUNCING_TIME);
                Button[eButtonID].status = BUTTON_DEBOUNCING;
            }

            break;
        }

        case BUTTON_DEBOUNCING:
        {
            if (Timer_GetTimerStatus(TIMER_DEBOUNCE) == TIMER_EXPIRED)
            {
                Button[eButtonID].stableState    = eNewState;
                Button[eButtonID].status         = BUTTON_STABLE;
            }

            break;
        }

        default:
        {
            /* Do nothing */
            break;
        }
    }
}

/**
 * @brief       Configure button
 * @param[in]   eButtonID       ButtonID
 * @param[in]   pGpioPin        Pointer to GPIO Pin configuration
 * @param[in]   eActiveState    Active state
 * @retval      None
 */
void Button_ConfigButton( \
    const Button_ButtonID eButtonID, \
    Gpio_GpioCfgType* const pGpioPin, \
    const Gpio_LevelType eActiveState)
{
    Button[eButtonID].pGpioCfgPtr   = pGpioPin;
    Button[eButtonID].activeState   = eActiveState;
    Button[eButtonID].status        = BUTTON_STABLE;

    if (eActiveState == GPIO_HIGH)
    {
        Button[eButtonID].stableState   = GPIO_LOW;
    }
    else
    {
        Button[eButtonID].stableState   = GPIO_HIGH;
    }
}

/**
 * @brief       Read button state
 * @param[in]   eButtonID       ButtonID
 * @retval      BUTTON_INACTIVE     Button is not active
 * @retval      BUTTON_ACTIVE       Button is active
 */
Button_StateType Button_ReadButton(const Button_ButtonID eButtonID)
{
    Gpio_LevelType  eGpioLevel;

    eGpioLevel  = Gpio_ReadPort(Button[eButtonID].pGpioCfgPtr);

    Button_DebounceButton(eButtonID, eGpioLevel);

    if (Button[eButtonID].stableState == Button[eButtonID].activeState)
    {
        return BUTTON_ACTIVE;
    }
    else
    {
        return BUTTON_INACTIVE;
    }
}
#endif /* (BSW_CFG_BUTTON_FUNCTION == STD_ENABLED) */
