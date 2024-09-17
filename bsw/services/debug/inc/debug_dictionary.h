#ifndef DEBUG_DICTIONARY_H
#define DEBUG_DICTIONARY_H

typedef enum __attribute__((packed))
{
    DEBUG_FATAL,
    DEBUG_ERROR,
    DEBUG_WARN,
    DEBUG_INFO,
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
} Debug_ModuleName;

typedef enum
{
    TEST_DEBUG_FATAL,
    TEST_DEBUG_ERROR,
    TEST_DEBUG_WARN,
    TEST_DEBUG_INFO,
} Debug_MessageID;

#endif /* DEBUG_DICTIONARY_H */
