#ifndef DEBUG_DICTIONARY_H
#define DEBUG_DICTIONARY_H

typedef enum __attribute__((packed))
{
    FATAL,
    ERROR,
    WARN,
    INFO,
} Debug_LevelType;

typedef enum __attribute__((packed))
{
    FLS,
    GPIO,
    GPT,
    MCU,
    UART,
    WDT,
    NVM,
    OS,
    BUTTON,
} Debug_ModuleName;

typedef enum
{
    REACHED_MAX_CALLBACK_FUNCTION,
    TEST_DEBUG_FATAL,
    TEST_DEBUG_ERROR,
    TEST_DEBUG_WARN,
    TEST_DEBUG_INFO,
    DEBOUNCING_TIME_TOO_SHORT,
} Debug_MessageID;

#endif /* DEBUG_DICTIONARY_H */
