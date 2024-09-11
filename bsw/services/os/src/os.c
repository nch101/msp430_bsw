#include "os.h"

static uint32 Os_gSystemTick;

#if (OS_CFG_TASK_1MS == STD_ENABLED)
static uint8    Os_gTask1msTick;
#endif /* (OS_CFG_TASK_1MS == STD_ENABLED) */

#if (OS_CFG_TASK_5MS == STD_ENABLED)
static uint8    Os_gTask5msTick;
#endif /* (OS_CFG_TASK_5MS == STD_ENABLED) */

#if (OS_CFG_TASK_10MS == STD_ENABLED)
static uint8    Os_gTask10msTick;
#endif /* (OS_CFG_TASK_10MS == STD_ENABLED) */

/**
 * @brief       
 * @retval      None
 */
static void OS_InitTimerTicks(void)
{
    Os_gSystemTick      = 0U;

#if (OS_CFG_TASK_1MS == STD_ENABLED)
    Os_gTask1msTick     = 0U;
#endif /* (OS_CFG_TASK_1MS == STD_ENABLED) */

#if (OS_CFG_TASK_5MS == STD_ENABLED)
    Os_gTask5msTick     = 3U;
#endif /* (OS_CFG_TASK_5MS == STD_ENABLED) */

#if (OS_CFG_TASK_10MS == STD_ENABLED)
    Os_gTask10msTick    = 7U;
#endif /* (OS_CFG_TASK_10MS == STD_ENABLED) */
}

/**
 * @brief       
 * @retval      None
 */
static void OS_InitFunction(void)
{
    OS_InitTimerTicks();

    WDT_SuspendWatchdogTimer();
    MCU_InitClock();
    GPT_InitFunction();
    MCU_InitOperatingMode();
    
#if (FLS_CFG_FUNCTION == STD_ENABLED)
    Fls_InitFunction();
#endif /* (FLS_CFG_FUNCTION == STD_ENABLED) */

    UART_InitFunction();
    GPIO_InitFunction();
    WDT_StartWatchdogTimer();
}

#if (OS_CFG_IDLE_TASK == STD_ENABLED)
/**
 * @brief       
 * @retval      None
 */
static void OS_IdleTask(void)
{
    /* Do nothing */
    _no_operation();
}
#endif /* (OS_CFG_IDLE_TASK == STD_ENABLED) */

#if (OS_CFG_BACKGROUND_TASK == STD_ENABLED)
/**
 * @brief       
 * @retval      None
 */
static void OS_BackgroundTask(void)
{

}
#endif /* (OS_CFG_BACKGROUND_TASK == STD_ENABLED) */

#if (OS_CFG_TASK_1MS == STD_ENABLED)
/**
 * @brief       
 * @retval      None
 */
static void OS_Task_1ms(void)
{

}
#endif /* (OS_CFG_TASK_1MS == STD_ENABLED) */

#if (OS_CFG_TASK_5MS == STD_ENABLED)
/**
 * @brief       
 * @retval      None
 */
static void OS_Task_5ms(void)
{

}
#endif /* (OS_CFG_TASK_5MS == STD_ENABLED) */

#if (OS_CFG_TASK_10MS == STD_ENABLED)
/**
 * @brief       
 * @retval      None
 */
static void OS_Task_10ms(void)
{
#if (FLS_CFG_FUNCTION == STD_ENABLED)
    Fls_MainFunction();
#endif /* (FLS_CFG_FUNCTION == STD_ENABLED) */
}
#endif /* (OS_CFG_TASK_10MS == STD_ENABLED) */

/**
 * @brief       
 * @retval      None
 */
void __attribute__((weak)) OS_InitApplicationTask(void)
{

}

/**
 * @brief       
 * @retval      None
 */
void __attribute__((weak)) OS_ApplicationTask(void)
{

}

/**
 * @brief       
 * @retval      None
 */
void OS_ProcessTiming(void)
{
    Os_gSystemTick++;

#if (OS_CFG_TASK_1MS == STD_ENABLED)
    Os_gTask1msTick++;
#endif /* (OS_CFG_TASK_1MS == STD_ENABLED) */

#if (OS_CFG_TASK_5MS == STD_ENABLED)
    Os_gTask5msTick++;
#endif /* (OS_CFG_TASK_5MS == STD_ENABLED) */

#if (OS_CFG_TASK_10MS == STD_ENABLED)
    Os_gTask10msTick++;
#endif /* (OS_CFG_TASK_10MS == STD_ENABLED) */

}

/**
 * @brief       
 * @retval      None
 */
void OS_EnableAllInterrupts(void)
{
    _enable_interrupts();
}

/**
 * @brief       
 * @retval      None
 */
void OS_DisableAllInterrupts(void)
{
    _disable_interrupts();
}

/**
 * @brief       
 * @retval      None
 */
OS_MainFunction()
{
    OS_InitFunction();
    OS_InitApplicationTask();

    while (1U)
    {
        WDT_ClearWatchdogTimer();

#if (OS_CFG_BACKGROUND_TASK == STD_ENABLED)
        OS_BackgroundTask();
#endif /* (OS_CFG_BACKGROUND_TASK == STD_ENABLED) */

#if (OS_CFG_TASK_1MS == STD_ENABLED)
        if (Os_gTask1msTick >= 1U)
        {
            Os_gTask1msTick = 0;
            OS_Task_1ms();
        }
#endif /* (OS_CFG_TASK_1MS == STD_ENABLED) */

#if (OS_CFG_TASK_5MS == STD_ENABLED)
        if (Os_gTask5msTick >= 5U)
        {
            Os_gTask5msTick = 0;
            OS_Task_5ms();
        }
#endif /* (OS_CFG_TASK_5MS == STD_ENABLED) */

#if (OS_CFG_TASK_10MS == STD_ENABLED)
        if (Os_gTask10msTick >= 10U)
        {
            Os_gTask10msTick = 0;
            OS_Task_10ms();
        }
#endif /* (OS_CFG_TASK_10MS == STD_ENABLED) */

        OS_ApplicationTask();
    }

    return 0;
}
