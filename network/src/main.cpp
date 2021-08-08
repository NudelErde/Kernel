#include "setup.hpp"

int main() {
    uint64_t pciCount = getPciDeviceCount();
    uint64_t* pciDeviceList = (uint64_t*) malloc(sizeof(uint64_t) * pciCount);
    getPciDeviceList(pciDeviceList, pciCount);

    for (uint64_t i = 0; i < pciCount; ++i) {
        uint64_t busDeviceFunction = pciDeviceList[i];
        uint32_t header[16];
        getPciDeviceHeader(busDeviceFunction, (uint8_t*) header);
    }

    return 0;
}