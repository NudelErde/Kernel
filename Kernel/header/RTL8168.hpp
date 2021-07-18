#pragma once

#include "Ethernet.hpp"

namespace Kernel {
class RTL8168 : public Ethernet {
public:
    RTL8168(PCI* dev);
};

}// namespace Kernel