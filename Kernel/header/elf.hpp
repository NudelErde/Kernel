#pragma once
#include "FileSystem.hpp"
#include <stdint.h>

namespace Kernel {

uint64_t loadAndExecute(EXT4&, const char* path, const char* arguments, uint64_t parentPid, bool loadWithDebug);

}