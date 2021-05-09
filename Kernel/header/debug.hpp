#pragma once

#include "stdint.h"

namespace Kernel {
    void printDebugInfo(void* stackPointer);
    void debugHandler(void* stackPointer);
    void openDebug();
}