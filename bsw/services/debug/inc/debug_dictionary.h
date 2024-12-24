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
} Debug_MessageID;

#endif /* DEBUG_DICTIONARY_H */
