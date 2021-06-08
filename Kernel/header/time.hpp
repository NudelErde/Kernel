#pragma once

#include "stdint.h"

namespace Kernel {

void setCenturyRegister(uint8_t centuryRegister);
uint64_t timeSinceEpoch();

}// namespace Kernel