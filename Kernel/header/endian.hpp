#pragma once

#include "stdint.h"

namespace Kernel {

//x86_64 uses little endian

template<typename Type>
static inline Type toLittleEndian(Type value) {
    return value;
}

template<typename Type>
static inline Type toBigEndian(Type value) {
    union Tmp {
        Type value;
        char raw[sizeof(Type)];
    };
    Tmp tmp{};
    tmp.value = value;
    Tmp result{};
    for (uint8_t i = 0; i < sizeof(Type); ++i) {
        result.raw[sizeof(Type) - 1 - i] = tmp.raw[i];
    }
    return result.value;
}

template<typename Type>
static inline Type fromLittleEndian(Type value) {
    return value;
}

template<typename Type>
static inline Type fromBigEndian(Type value) {
    union Tmp {
        Type value;
        char raw[sizeof(Type)];
    };
    Tmp tmp{};
    tmp.value = value;
    Tmp result{};
    for (uint8_t i = 0; i < sizeof(Type); ++i) {
        result.raw[sizeof(Type) - 1 - i] = tmp.raw[i];
    }
    return result.value;
}

}// namespace Kernel
