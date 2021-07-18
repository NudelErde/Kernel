#pragma once

#include "PCI.hpp"
#include "stdint.h"

namespace Kernel {

class VGA {
public:
    struct Mode {
        void (*setPixel)(VGA*, uint64_t, uint64_t, uint8_t);
        void (*clearFrame)(VGA*);
        uint8_t miscellaneousOutput;
        struct Sequencer {
            uint8_t reset;
            uint8_t clockingMode;
            uint8_t mapMask;
            uint8_t characterMapSelect;
            uint8_t sequencerMemoryMode;
        } sequencer;
        struct CRTC {
            uint8_t horizontalTotal;
            uint8_t endHorizontalDisplay;
            uint8_t startHorizontalBlanking;
            uint8_t startHorizontalRetrace;
            uint8_t endHorizontalRetrace;
            uint8_t verticalTotal;
            uint8_t overflow;
            uint8_t presetRowScan;
            uint8_t maximumScanLine;
            uint8_t cursorStart;
            uint8_t cursorEnd;
            uint8_t startAddressHigh;
            uint8_t startAddressLow;
            uint8_t cursorLocationHigh;
            uint8_t cursorLocationLow;
            uint8_t verticalRetraceStart;
            uint8_t verticalDisplayEnd;
            uint8_t offset;
            uint8_t underlineLocation;
            uint8_t startVerticalBlanking;
            uint8_t endVerticalBlanking;
            uint8_t CRTCModeControl;
            uint8_t lineCompare;
        } crtc;
        struct Graphics {
            uint8_t setReset;
            uint8_t enableSetReset;
            uint8_t colorCompare;
            uint8_t dataRotate;
            uint8_t readMapSelect;
            uint8_t graphicsMode;
            uint8_t miscGraphics;
            uint8_t colorDontCare;
            uint8_t bitMask;
        } graphics;
        struct Attributes {
            uint8_t palette[16];
            uint8_t attributeModeControl;
            uint8_t overscanColor;
            uint8_t colorPlaneEnable;
            uint8_t horizontalPixelPanning;
            uint8_t colorSelect;
        } attributes;
        uint64_t width;
        uint64_t height;
    };

    static void readFrameBufferInfo(uint8_t*);

    static VGA* openController(PCI* device, const PCICommonHeader& header);
    void loadVGAMode(const Mode& mode);
    void drawPixel(uint64_t x, uint64_t y, uint8_t color);
    void drawImage(uint64_t x, uint64_t y, uint64_t width, uint64_t height, const uint8_t* image, uint64_t scaleX = 1, uint64_t scaleY = 1, uint8_t transparentColor = 0xFF);
    void clear();

    void setPlane(uint8_t plane);
    uint8_t* getFrameBuffer();

    inline uint64_t getWidth() const { return width; }
    inline uint64_t getHeight() const { return height; }

private:
    static uint64_t readPort(uint32_t port, uint64_t index);
    static void writePort(uint32_t port, uint64_t index, uint64_t data);
    static void setBitsPort(uint32_t port, uint64_t index, uint64_t data, uint64_t bits);

    void (*setPixel)(VGA*, uint64_t, uint64_t, uint8_t);
    void (*clearFrame)(VGA*);
    uint64_t width;
    uint64_t height;
};

}// namespace Kernel