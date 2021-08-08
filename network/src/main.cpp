#include "setup.hpp"

void writeNormal(char ch);

void printHex(uint64_t hex, uint8_t num = 1) {
    const char* numToDigit = "0123456789ABCDEF";
    uint8_t nibble;
    bool hasStarted = false;
    for (int i = 0; i < sizeof(uint64_t) * 2; ++i) {
        nibble = (uint8_t) (hex >> 60) & 0xF;
        if (nibble)
            hasStarted = true;
        if (hasStarted || num >= sizeof(uint64_t) * 2 - i)
            writeNormal(numToDigit[nibble]);
        hex <<= 4;
    }
}

void printString(const char* str) {
    for (; *str; ++str) {
        writeNormal(*str);
    }
}

void writeNormal(char ch) {
    write(ch);
    //callIPM(1, 1, ch);
}

int main() {
    uint64_t pciCount = getPciDeviceCount();
    uint64_t* pciDeviceList = (uint64_t*) malloc(sizeof(uint64_t) * pciCount);
    getPciDeviceList(pciDeviceList, pciCount);

    printString("Device count: ");
    printHex(pciCount);
    printString("\n");

    for (uint64_t i = 0; i < pciCount; ++i) {
        uint64_t busDeviceFunction = pciDeviceList[i];
        uint32_t header[16];
        getPciDeviceHeader(busDeviceFunction, (uint8_t*) header);

        uint8_t devRevision = (uint8_t) (header[2]);
        uint8_t devProgif = (uint8_t) (header[2] >> 8);
        uint8_t devSubclass = (uint8_t) (header[2] >> 16);
        uint8_t devClass = (uint8_t) (header[2] >> 24);
        printString("\nType: ");
        printHex(devClass, 2);
        printString(":");
        printHex(devSubclass, 2);
        printString(":");
        printHex(devProgif, 2);
    }

    return 0;
}