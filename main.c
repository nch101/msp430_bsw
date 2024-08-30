#include "gpt.h"
#include "mcu.h"
#include "uart.h"

int main(void)
{
    MCU_InitClock();
    GPT_InitFunction();
    UART_Init();
}
