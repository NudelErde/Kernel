#pragma once

#include "VGA.hpp"

namespace Kernel {

class VideoTextField {
public:
    VideoTextField() {}
    VideoTextField(VGA* vga, uint64_t baseX, uint64_t baseY, uint64_t width, uint64_t height, uint8_t scale = 1, uint8_t transparentColor = 0xFF, uint8_t* colorMap = nullptr);

    void setChar(uint64_t x, uint64_t y, char ch);
    void print(char ch);
    void printLine();

    uint64_t getWidth() const;
    uint64_t getHeight() const;

    struct CharBitmap {
        uint8_t bitmap[8 * 8 / 2];
    };

    inline void mapColor(uint8_t from, uint8_t to) {
        if (from < 0x10 && to < 0x10) colorMap[from] = to;
    }

    inline void setTransparentColor(uint8_t transparent) { transparentColor = transparent; }
    inline uint8_t getTransparentColor() const { return transparentColor; }

    inline void setFont(const CharBitmap* f) { font = f; }
    inline const CharBitmap* getFont() const { return font; }

private:
    VGA* vga;
    struct {
        uint64_t x;
        uint64_t y;
        uint64_t width;
        uint64_t height;
    } base;
    struct {
        uint64_t x;
        uint64_t y;
    } cursor;
    uint64_t scale;
    const CharBitmap* font;
    char* textBuffer;
    uint8_t colorMap[0x10];
    uint8_t transparentColor;
};

}// namespace Kernel