#ifndef PROJECT_CFG_H
#define PROJECT_CFG_H

#include "bsw_cfg.h"

typedef struct __attribute__((packed)) Project_DummyType_t
{
    uint8   value;
} Project_DummyType_t;

typedef union PrjCfgType_t
{
    uint8 u8Data[BSW_MAX_PROJECT_CONFIG];

    struct __attribute__((packed)) 
    {
        Project_DummyType_t     dummy;
    } item;
} PrjCfgType_t;

extern const PrjCfgType_t   PROJECT_DATA;

#endif /* PROJECT_CFG_H */
