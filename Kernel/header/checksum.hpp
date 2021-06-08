#pragma once

#include "stdint.h"

namespace Kernel {

uint32_t crc32c(uint32_t initCrc, const uint8_t* buf, uint64_t len);
void crc32c_init();

}// namespace Kernel