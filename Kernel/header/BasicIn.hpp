#pragma once
#include "stdint.h"

namespace Kernel {

class Input {
public:
    enum class InputMode {
        NONE = 0,
        SERIAL = 1,
        PS2 = 2,
        USB_KEYBOARD = 3
    };

    static void init(InputMode mode);
    static uint8_t readBlocking();
    static bool isEmpty();
};

}// namespace Kernel