#include "gpt.h"

/**
 * @brief       GPT init function
 * @retval      None
 */
void GPT_InitFunction(void)
{
    /* Configure Timer A */
    TACCTL0 = GPT_CFG_TIMER_A_CLK_SRC_REG   \
                | GPT_CFG_TIMER_A_MODE_REG  \
                | ID_0          /* Divide by 1 */               \
                | TAIE          /* Enable timer A interrupt */  \
                ;
    /* Configure Timer A counter */
    TA0CCR0 = GPT_CFG_TIMER_A_COUNT;
}

/**
 * @brief       GPT driver start timer function
 * @retval      None
 */
void GPT_StartTimer(void)
{
    /* Start Timer A */
    TACCTL0 = GPT_CFG_TIMER_A_MODE_REG      \
                | TACLR \
                ;
    /* Configure Timer A counter */
    TA0CCR0 = GPT_CFG_TIMER_A_COUNT;
}

/**
 * @brief       GPT driver stop timer function
 * @retval      None
 */
void GPT_StopTimer(void)
{
    /* Stop Timer A */
    TACCTL0 = MC_0;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TimerA_ISR (void)
#else
#error Compiler not supported!
#endif
{
    /* Call OS Timing Handling */
}
