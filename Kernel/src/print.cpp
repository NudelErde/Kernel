#include "print.hpp"
#include "print.h"

namespace Kernel {

void Print::print(const char* str) {
    print_str(str);
}

void Print::print(char c) {
    print_char(c);
}

void Print::print(int i) {
    if (i < 0) {
        print_char('-');
        i *= -1;
    }
    printBaseN(i, 10);
}

void Print::printHex(uint64_t hex) {
    printBaseN(hex, 16);
}

void Print::printBaseN(uint64_t number, uint8_t base) {
    const char* digits = "0123456789ABCDEF";
    if (base > 16) {
        base = 16;
    }
    if (base < 2) {
        base = 2;
    }

    char buffer[64]{};
    int index;
    for (index = 1; number > 0; ++index) {
        buffer[index] = digits[number % base];
        number /= base;
    }

    if (index == 1) {
        print_char('0');
        return;
    }

    buffer[63] = 0;// always terminated pls
    for (char* ptr = buffer + index - 1; *ptr; --ptr) {
        print_char(*ptr);
    }
}

void Print::println(const char* str) {
    print(str);
    print('\n');
}

void Print::println(char c) {
    print(c);
    print('\n');
}

void Print::println(int i) {
    print(i);
    print('\n');
}

void Print::printHexln(uint64_t hex) {
    printHex(hex);
    print('\n');
}

void Print::setColor(Color foreground, Color background) {
    print_set_color((uint8_t) foreground, (uint8_t) background);
}

void Print::println() {
    print('\n');
}

void Print::clear() {
    print_clear();
}

}// namespace Kernel