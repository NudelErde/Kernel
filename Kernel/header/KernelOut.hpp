#pragma once
#include "serial.hpp"
#include <stdint.h>

#define KTODO(name) ::Kernel::kout << "TODO: " << name << " in " << __FILE__ << ':' << (uint64_t) __LINE__ << '\n';

namespace Kernel {
class KernelOut;
class KernelOutSetSerial;
class KernelOutSetDisplay;
}// namespace Kernel


Kernel::KernelOut& operator<<(Kernel::KernelOut& out, const Kernel::KernelOutSetSerial& koss);
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, const Kernel::KernelOutSetDisplay& kosd);

namespace Kernel {

class HexDump {
public:
    inline HexDump(const uint8_t* buffer, uint16_t size) : buffer(buffer), size(size) {}

public:
    const uint8_t* buffer;
    uint16_t size;
};

class KernelOutSetSerial {
private:
    KernelOutSetSerial(const Serial& serial);
    Serial serial;
    friend KernelOutSetSerial setSerial(const Serial& serial);
    friend Kernel::KernelOut& ::operator<<(Kernel::KernelOut& out, const Kernel::KernelOutSetSerial& koss);
};

KernelOutSetSerial setSerial(const Serial& serial);

class KernelOutSetDisplay {
private:
    friend KernelOutSetDisplay setDisplay();
};

KernelOutSetDisplay setDisplay();

class KernelOut {
public:
    static void init();

    KernelOut();
    void print(char c);
    void print(const char* str);

    friend Kernel::KernelOut& ::operator<<(Kernel::KernelOut& out, const Kernel::KernelOutSetSerial& koss);
    friend Kernel::KernelOut& ::operator<<(Kernel::KernelOut& out, const Kernel::KernelOutSetDisplay& kosd);

private:
    bool isVGA;
    Serial serial;
};

struct Hex {
    inline Hex(uint64_t hex, uint8_t digits = 1) : hex(hex), digits(digits) {}
    template<typename T>
    Hex(T hex, uint8_t digits = 1) : hex((uint64_t) hex), digits(digits) {}

    uint64_t hex;
    uint8_t digits;
};

struct BitList {
    inline BitList(uint64_t bits) : bits(bits) {}
    uint64_t bits;
};

struct Bin {
    inline Bin(uint64_t bin, uint8_t digits = 1) : bin(bin), digits(digits) {}

    uint64_t bin;
    uint8_t digits;
};

extern KernelOut kout;
}// namespace Kernel

Kernel::KernelOut& operator<<(Kernel::KernelOut& out, uint64_t n);
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, Kernel::Hex n);
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, Kernel::BitList n);
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, Kernel::Bin n);
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, char n);
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, const char* n);
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, const Kernel::KernelOutSetSerial& koss);
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, const Kernel::KernelOutSetDisplay& kosd);
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, const Kernel::HexDump& dump);

inline Kernel::KernelOut& operator<<(Kernel::KernelOut& out, bool n) {
    if (n)
        return out << "true";
    else
        return out << "false";
}
inline Kernel::KernelOut& operator<<(Kernel::KernelOut& out, int64_t n) {
    if (n < 0)
        out.print('-');
    return out << (uint64_t) (n * -1);
}