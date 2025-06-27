#include "project_cfg.h"

const PrjCfgType_t PROJECT_DATA __attribute__((section(".infoB"))) =
{
    .item = 
    {
        .dummy =
        {
            .value = 10U
        },
    }
};
