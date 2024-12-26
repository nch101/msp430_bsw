#include "timer.h"
#include "debouncing.h"

#if (BSW_CFG_DEBOUNCING_FUNCTION == STD_ENABLED)
static Debounce_DebounceAttributes  Debounce_aDevices[DEBOUNCE_MAX_DEVICE_ID_PRESENT];

/**
 * @brief       Debounces device
 * @param[in]   eDeviceID       DeviceID
 * @param[in]   eNewState       Current device state
 * @retval      None
 */
static void Debounce_DebounceDevice(const Debounce_DeviceID eDeviceID, const Gpio_LevelType eNewState)
{
    if (eNewState != Debounce_aDevices[eDeviceID].eOldState)
    {
        Debounce_aDevices[eDeviceID].eOldState          = eNewState;
        Timer_SetTimer(TIMER_DEBOUNCE, DEBOUNCE_CFG_DEBOUNCING_TIME);
    }
    else 
    {
        if (Timer_GetTimerStatus(TIMER_DEBOUNCE) == TIMER_EXPIRED)
        {
            Debounce_aDevices[eDeviceID].eStableState   = eNewState;
        }
    }
}

/**
 * @brief       Configure device
 * @param[in]   eDeviceID       DeviceID
 * @param[in]   pGpioPin        Pointer to GPIO Pin configuration
 * @retval      None
 */
void Debounce_ConfigureDevice(const Debounce_DeviceID eDeviceID, Gpio_GpioCfgType* const pGpioPin)
{
    Debounce_aDevices[eDeviceID].pGpioCfgPtr    = pGpioPin;
    Debounce_aDevices[eDeviceID].eOldState      = (pGpioPin->ePull == GPIO_PULL_UP) ? GPIO_HIGH : GPIO_LOW;
    Debounce_aDevices[eDeviceID].eStableState   = Debounce_aDevices[eDeviceID].eOldState;
}

/**
 * @brief       Read device state
 * @param[in]   eDeviceID           DeviceID
 * @retval      GPIO_LOW
 * @retval      GPIO_HIGH
 */
Gpio_LevelType Debounce_ReadDeviceState(const Debounce_DeviceID eDeviceID)
{
    Gpio_LevelType      eGpioLevel;

    eGpioLevel  = Gpio_ReadPort(Debounce_aDevices[eDeviceID].pGpioCfgPtr);
    Debounce_DebounceDevice(eDeviceID, eGpioLevel);

    return Debounce_aDevices[eDeviceID].eStableState;
}
#endif /* (BSW_CFG_DEBOUNCING_FUNCTION == STD_ENABLED) */
