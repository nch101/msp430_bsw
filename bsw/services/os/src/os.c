#include "os.h"

static Gpio_GpioCfgType led10;
static uint16 Os_u16SystemTick;

#if (OS_CFG_TASK_1MS == STD_ENABLED)
static uint8    Os_u8Task1msTick;
#endif /* (OS_CFG_TASK_1MS == STD_ENABLED) */

#if (OS_CFG_TASK_5MS == STD_ENABLED)
static uint8    Os_u8Task5msTick;
#endif /* (OS_CFG_TASK_5MS == STD_ENABLED) */

#if (OS_CFG_TASK_10MS == STD_ENABLED)
static uint8    Os_u8Task10msTick;
#endif /* (OS_CFG_TASK_10MS == STD_ENABLED) */

#if (OS_CFG_TASK_100MS == STD_ENABLED)
static uint8    Os_u8Task100msTick;
#endif /* (OS_CFG_TASK_100MS == STD_ENABLED) */


static void Os_InitTimerTicks(void)
{
    Os_u16SystemTick      = 0U;

    #if (OS_CFG_TASK_1MS == STD_ENABLED)
        Os_u8Task1msTick     = 0U;
    #endif /* (OS_CFG_TASK_1MS == STD_ENABLED) */

    #if (OS_CFG_TASK_5MS == STD_ENABLED)
        Os_u8Task5msTick     = 3U;
    #endif /* (OS_CFG_TASK_5MS == STD_ENABLED) */

    #if (OS_CFG_TASK_10MS == STD_ENABLED)
        Os_u8Task10msTick    = 7U;
    #endif /* (OS_CFG_TASK_10MS == STD_ENABLED) */

    #if (OS_CFG_TASK_100MS == STD_ENABLED)
        Os_u8Task100msTick    = 13U;
    #endif /* (OS_CFG_TASK_100MS == STD_ENABLED) */
}

static void Os_ProcessTiming(void)
{
    Os_u16SystemTick++;

    #if (OS_CFG_TASK_1MS == STD_ENABLED)
        Os_u8Task1msTick++;
    #endif /* (OS_CFG_TASK_1MS == STD_ENABLED) */

    #if (OS_CFG_TASK_5MS == STD_ENABLED)
        Os_u8Task5msTick++;
    #endif /* (OS_CFG_TASK_5MS == STD_ENABLED) */

    #if (OS_CFG_TASK_10MS == STD_ENABLED)
        Os_u8Task10msTick++;
    #endif /* (OS_CFG_TASK_10MS == STD_ENABLED) */

    #if (OS_CFG_TASK_100MS == STD_ENABLED)
        Os_u8Task100msTick++;
    #endif /* (OS_CFG_TASK_100MS == STD_ENABLED) */
}

static void Os_InitFunction(void)
{
    Os_InitTimerTicks();

    Wdt_SuspendWatchdogTimer();
    Mcu_InitClock();
    Gpt_InitFunction(&Os_ProcessTiming);
    Mcu_InitOperatingMode();

    #if (BSW_CFG_TIMER_FUNCTION == STD_ENABLED)
        Timer_InitFunction();
    #endif /* (BSW_CFG_TIMER_FUNCTION = STD_ENABLED) */

    #if (BSW_CFG_FLS_FUNCTION == STD_ENABLED)
        Fls_InitFunction();
    #endif /* (BSW_CFG_FLS_FUNCTION == STD_ENABLED) */

    #if (BSW_CFG_NVM_FUNCTION == STD_ENABLED)
        Nvm_InitFunction();
    #endif /* (BSW_CFG_NVM_FUNCTION == STD_ENABLED) */

    #if (BSW_CFG_LOG_FUNCTION == STD_ENABLED)
        Log_InitFunction();
    #endif /* (BSW_CFG_LOG_FUNCTION == STD_ENABLED) */

    #if (BSW_CFG_UART_FUNCTION == STD_ENABLED)
        Uart_InitFunction();
    #endif /* (BSW_CFG_UART_FUNCTION == STD_ENABLED) */

    #if (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED)
        Debug_InitFunction();
    #endif /* (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED) */

    #if (BSW_CFG_GPIO_FUNCTION == STD_ENABLED)
        Gpio_InitFunction();
    #endif /* (BSW_CFG_GPIO_FUNCTION == STD_ENABLED) */

    Wdt_StartWatchdogTimer();
}

#if (OS_CFG_IDLE_TASK == STD_ENABLED)
static void Os_IdleTask(void)
{
    /* Do nothing */
    _no_operation();
}
#endif /* (OS_CFG_IDLE_TASK == STD_ENABLED) */

#if (OS_CFG_BACKGROUND_TASK == STD_ENABLED)
static void Os_BackgroundTask(void)
{

}
#endif /* (OS_CFG_BACKGROUND_TASK == STD_ENABLED) */

#if (OS_CFG_TASK_1MS == STD_ENABLED)
static void Os_Task_1ms(void)
{
#if (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED)
    Debug_MainFunction();
#endif /* (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED) */
}
#endif /* (OS_CFG_TASK_1MS == STD_ENABLED) */

#if (OS_CFG_TASK_5MS == STD_ENABLED)
static void Os_Task_5ms(void)
{

}
#endif /* (OS_CFG_TASK_5MS == STD_ENABLED) */

#if (OS_CFG_TASK_10MS == STD_ENABLED)
static void Os_Task_10ms(void)
{
#if (BSW_CFG_FLS_FUNCTION == STD_ENABLED)
    Fls_MainFunction();
#endif /* (BSW_CFG_FLS_FUNCTION == STD_ENABLED) */
}
#endif /* (OS_CFG_TASK_10MS == STD_ENABLED) */

#if (OS_CFG_TASK_100MS == STD_ENABLED)
static void Os_Task_100ms(void)
{
    Gpio_TogglePort(&led10);
    DEBUG_INFO(OS, TEST_DEBUG_INFO);
    DEBUG_INFO(OS, TEST_DEBUG_INFO);
    DEBUG_INFO(OS, TEST_DEBUG_INFO);
    DEBUG_INFO(OS, TEST_DEBUG_INFO);
#if (BSW_CFG_NVM_FUNCTION == STD_ENABLED)
    Nvm_MainFunction();
#endif /* (BSW_CFG_NVM_FUNCTION == STD_ENABLED) */

}
#endif /* (OS_CFG_TASK_100MS == STD_ENABLED) */

void __attribute__((weak)) Os_InitApplicationTask(void)
{
    led10.eGroup        = GPIO_GROUP_1;
    led10.ePin          = GPIO_PIN_0;
    led10.eDirection    = GPIO_OUTPUT;

    Gpio_ConfigPort(&led10);
}

void __attribute__((weak)) Os_ApplicationTask(void)
{

}

/**
 * @brief       Get system tick timer
 * @retval      Os_u16SystemTick
 */
uint16 Os_GetSysTick(void)
{
    return Os_u16SystemTick;
}

void Os_EnableAllInterrupts(void)
{
    _enable_interrupts();
}

void Os_DisableAllInterrupts(void)
{
    _disable_interrupts();
}

Os_MainFunction()
{
    Os_InitFunction();
    Os_InitApplicationTask();

    while (TRUE)
    {
        Wdt_ClearWatchdogTimer();

    #if (OS_CFG_BACKGROUND_TASK == STD_ENABLED)
        Os_BackgroundTask();
    #endif /* (OS_CFG_BACKGROUND_TASK == STD_ENABLED) */

    #if (OS_CFG_TASK_1MS == STD_ENABLED)
        if (Os_u8Task1msTick >= 1U)
        {
            Os_u8Task1msTick = 0;
            Os_Task_1ms();
        }
    #endif /* (OS_CFG_TASK_1MS == STD_ENABLED) */

    #if (OS_CFG_TASK_5MS == STD_ENABLED)
        if (Os_u8Task5msTick >= 5U)
        {
            Os_u8Task5msTick = 0;
            Os_Task_5ms();
        }
    #endif /* (OS_CFG_TASK_5MS == STD_ENABLED) */

    #if (OS_CFG_TASK_10MS == STD_ENABLED)
        if (Os_u8Task10msTick >= 10U)
        {
            Os_u8Task10msTick = 0;
            Os_Task_10ms();
        }
    #endif /* (OS_CFG_TASK_10MS == STD_ENABLED) */

    #if (OS_CFG_TASK_100MS == STD_ENABLED)
        if (Os_u8Task100msTick >= 100U)
        {
            Os_u8Task100msTick = 0;
            Os_Task_100ms();
        }
    #endif /* (OS_CFG_TASK_100MS == STD_ENABLED) */

        Os_ApplicationTask();

    #if ((OS_CFG_BACKGROUND_TASK == STD_DISABLED) && (OS_CFG_IDLE_TASK == STD_ENABLED))
        Os_IdleTask();
    #endif /* (OS_CFG_IDLE_TASK == STD_ENABLED) */
    }
}
