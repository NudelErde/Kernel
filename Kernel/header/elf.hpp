#pragma once
#include <stdint.h>
#include "FileSystem.hpp"

namespace Kernel {

uint64_t loadAndExecute(EXT4&, const char* path, const char* arguments);

}