#pragma once
#include "stdint.h"

namespace Kernel{

    void readACPITables();
    uint64_t* getIoAPICData(uint8_t& count);

}
