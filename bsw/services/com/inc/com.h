#ifndef COM_H
#define COM_H
#include "timer.h"
#include "com_cfg.h"

#if (BSW_CFG_COM_FUNCTION == STD_ENABLED)
/* Smallest enum value has highest priority.*/
typedef enum __attribute__((packed)) Com_SessionType
{
    COM_DEFAULT_SESSION     = 0,
#if (BSW_CFG_DIAGNOSTIC_FUNCTION == STD_ENABLED)
    COM_DIAGNOSTIC_SESSION,
#endif /* (BSW_CFG_DIAGNOSTIC_FUNCTION == STD_ENABLED) */
#if (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED)
    COM_DEBUG_SESSION,
#endif /* (BSW_CFG_DEBUG_FUNCTION == STD_ENABLED) */
    COM_MAX_SESSION_PRESENT,
} Com_SessionType;

typedef enum __attribute__((packed)) Com_StateType
{
    COM_IDLE,
    COM_RECEIVING,
    COM_SENDING,
    COM_TIMEOUT,
    COM_COMPLETED,
} Com_StateType;

typedef Std_BooleanType (*Com_SwitchConditionChecker)(void);
typedef Std_BooleanType (*Com_RxDataChecker)(void);
typedef void (*Com_ServiceProcessorType)(void);

typedef struct Com_TxBufferType
{
    uint8   u8Len;
    uint8   aData[COM_CFG_MAX_TX_DATA_LEN];
} Com_TxBufferType;

typedef struct Com_ServiceType 
{
    Com_SwitchConditionChecker  eConditionChecker;      /* Session identification function */
    Com_RxDataChecker           eRxDataChecker;         /* Checking for valid Rx data function */
    Com_ServiceProcessorType    vSrvProcessor;          /* Processing service function */
    uint16                      u16SessionTimeout;      /* Session timeout */
    Timer_TimerID               eSessionTimerID;        /* Session timer ID */
} Com_ServiceType;

extern Std_StatusType Com_TransmitData(uint8 const * const pDataPtr, uint8 const u8DataLength);
extern void Com_GetRxData(uint8** const pDataOut, uint8* const pDataLen);
extern void Com_ExitCurrentRxSession(void);
extern Std_StatusType Com_RegisterNewRxComSession(const Com_SessionType sSessionID, Com_ServiceType const * const sSessionProcessor);
extern void Com_MainFunction(void);
extern void Com_InitFunction(void);

#endif /* (BSW_CFG_COM_FUNCTION == STD_ENABLED) */
#endif /* COM_H */
