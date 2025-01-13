#ifndef DIAG_H
#define DIAG_H

#include "diag_cfg.h"

#if (BSW_CFG_DIAGNOSTIC_FUNCTION == STD_ENABLED)
typedef void (*Diag_ServiceProcessorType)(void);

typedef struct __attribute__((packed)) Diag_MsgFields
{
    uint8   u8Len;
    uint8   u8ServiceID;
    uint8   u8Data[DIAG_CFG_MAX_DATA_LEN];
} Diag_MsgFields;

typedef union Diag_MsgStruct
{
    uint8           u8Data[DIAG_CFG_MAX_DATA_LEN + 2U];
    Diag_MsgFields  sField;
} Diag_MsgStruct;

typedef struct Diag_SessionAttributeType
{
    Diag_ServiceProcessorType   vSrvProcessor;
} Diag_SessionAttributeType;


typedef enum __attribute__((packed)) Diag_SessionType
{
    DIAG_SECURE_SESSION,
    DIAG_FULL_ACCESS_SESSION,
} Diag_SessionType;


extern void Diag_InitFunction(void);

#endif /* (BSW_CFG_DIAGNOSTIC_FUNCTION == STD_ENABLED) */
#endif /* DIAG_H */
