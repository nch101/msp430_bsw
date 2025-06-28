#include "project_cfg.h"

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma DATA_SECTION(PROJECT_DATA, ".infoB")
#pragma RETAIN(PROJECT_DATA)
const PrjCfgType_t PROJECT_DATA =

#elif defined(__GNUC__)
const PrjCfgType_t PROJECT_DATA __attribute__((section(".infoB"))) =
#else
#error Compiler not supported!
#endif
{
    .item = 
    {
        .dummy =
        {
            .value = 10U
        },
    }
};
