#pragma once

#include "stdint.h"

namespace Kernel {

bool hasCPUTrueRandom();
uint64_t getCPUTrueRandom();

}