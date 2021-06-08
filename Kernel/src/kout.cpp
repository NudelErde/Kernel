#include "KernelOut.hpp"
#include "print.hpp"

namespace Kernel {
KernelOut kout;

void KernelOut::init() {
    kout = KernelOut();
}

KernelOut::KernelOut() : isVGA(true), serial(0) {}

void KernelOut::print(char c) {
    if (isVGA) {
        Print::print(c);
    } else {
        serial.print(c);
    }
}

void KernelOut::print(const char* str) {
    for (; *str; ++str) {
        print(*str);
    }
}

KernelOutSetSerial::KernelOutSetSerial(const Serial& serial) : serial(serial) {}

KernelOutSetSerial setSerial(const Serial& serial) {
    return KernelOutSetSerial(serial);
}

KernelOutSetDisplay setDisplay() {
    return KernelOutSetDisplay();
}
};// namespace Kernel

static void printBase(Kernel::KernelOut& out, uint64_t number, uint64_t base, int size) {
    const char* digits = "0123456789ABCDEF";
    char buffer[64]{};

    int index;
    for (index = 1; number > 0; ++index) {
        buffer[index] = digits[number % base];
        number /= base;
    }

    for (int i = 0; i < size - index + 1; ++i) {
        out << '0';
    }

    buffer[63] = 0;// always terminated pls
    for (char* ptr = buffer + index - 1; *ptr; --ptr) {
        out << *ptr;
    }
}

Kernel::KernelOut& operator<<(Kernel::KernelOut& out, const Kernel::KernelOutSetSerial& koss) {
    out.isVGA = false;
    out.serial = koss.serial;
    return out;
}
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, const Kernel::KernelOutSetDisplay& kosd) {
    out.isVGA = true;
    return out;
}
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, char n) {
    out.print(n);
    return out;
}
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, const char* n) {
    out.print(n);
    return out;
}

Kernel::KernelOut& operator<<(Kernel::KernelOut& out, Kernel::Hex n) {
    printBase(out, n.hex, 16, n.digits);
    return out;
}
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, Kernel::BitList n) {
    uint64_t num = n.bits;
    out << '[';
    bool first = true;
    for (uint64_t i = 0; num; ++i, num >>= 1) {
        if (num & 0b1) {
            if (!first) {
                out << ", ";
            }
            first = false;
            out << i;
        }
    }
    out << ']';
    return out;
}
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, Kernel::Bin n) {
    printBase(out, n.bin, 2, n.digits);
    return out;
}
Kernel::KernelOut& operator<<(Kernel::KernelOut& out, uint64_t number) {
    printBase(out, number, 10, 1);
    return out;
}

Kernel::KernelOut& operator<<(Kernel::KernelOut& out, const Kernel::HexDump& dump) {
    using namespace Kernel;
    kout << "     | ";
    for (uint8_t i = 0; i < 16; ++i) {
        kout << "*" << Hex(i) << (i % 16 == 15 ? "\n" : " | ");
    }
    for (uint16_t i = 0; i < dump.size; ++i) {
        if (i % 16 == 0) {
            kout << Hex(i, 4) << " | ";
        }
        kout << Hex(dump.buffer[i] & 0xFF, 2) << " | ";
        if (i % 16 == 15) {
            for (uint16_t j = i - 15; j <= i; j++) {
                char ch = (char) dump.buffer[j];
                if (ch >= 0x20 && ch <= 0x7E) {
                    kout << ch;
                } else {
                    kout << '.';
                }
            }
            kout << '\n';
        }
    }
    return kout;
}