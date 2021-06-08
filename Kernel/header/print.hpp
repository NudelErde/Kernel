#pragma once

#include <stdint.h>

namespace Kernel {

class Print {
public:
    enum class Color {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        CYAN = 3,
        RED = 4,
        MAGENTA = 5,
        BROWN = 6,
        LIGHT_GRAY = 7,
        DARK_GRAY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 10,
        LIGHT_CYAN = 11,
        LIGHT_RED = 12,
        PINK = 13,
        YELLOW = 14,
        WHITE = 15,
    };


    static void print(const char* str);
    static void print(char c);
    static void print(int i);
    static void printHex(uint64_t hex);
    static void printBaseN(uint64_t number, uint8_t base);

    static void println(const char* str);
    static void println(char c);
    static void println(int i);
    static void printHexln(uint64_t hex);
    static void println();

    static void clear();

    static void setColor(Color foreground, Color background);
};

}// namespace Kernel