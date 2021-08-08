#include "VGA.hpp"
#include "KernelOut.hpp"
#include "inout.hpp"
#include "units.hpp"

namespace Kernel {

static struct {
    uint64_t address;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t bpp;
    uint8_t type;
} frameBufferInfo;

void clearFrameVideo640x480(VGA* vga) {
    uint8_t* frameBuffer = vga->getFrameBuffer();
    for (uint8_t i = 0; i < 4; ++i) {
        vga->setPlane(i);
        for (uint64_t i = 0; i < 0xFFFF; ++i) {
            frameBuffer[i] = 0;
        }
    }
}

void setPixelVideo640x480(VGA* vga, uint64_t x, uint64_t y, uint8_t color) {
    uint8_t* frameBuffer = vga->getFrameBuffer();

    uint64_t offset = (uint64_t) (x / 8 + (640 / 8) * y);

    x = x & 7;

    uint8_t mask = (uint8_t) (0x80 >> x);
    uint8_t planeMask = 1;

    for (uint8_t planeIndex = 0; planeIndex < 4; ++planeIndex) {
        vga->setPlane(planeIndex);

        if ((planeMask & color) != 0) {
            frameBuffer[offset] = (uint8_t) (frameBuffer[offset] | mask);
        } else {
            frameBuffer[offset] = (uint8_t) (frameBuffer[offset] & ~mask);
        }

        planeMask <<= 1;
    }
}

constexpr static VGA::Mode video640x480{
        setPixelVideo640x480,
        clearFrameVideo640x480,
        0x43,
        {0x03, 0x01, 0x08, 0x00, 0x06},
        {0x5F, 0x4F, 0x50, 0x54, 0x80, 0x0B, 0x3E, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3, 0xFF},
        {0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F, 0xFF},
        {{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}, 0x01, 0x00, 0x0F, 0x00, 0x00},
        640,
        480};

void VGA::readFrameBufferInfo(uint8_t* ptr) {
    frameBufferInfo.address = *(uint64_t*) (ptr + 8);
    frameBufferInfo.pitch = *(uint32_t*) (ptr + 16);
    frameBufferInfo.width = *(uint32_t*) (ptr + 20);
    frameBufferInfo.height = *(uint32_t*) (ptr + 24);
    frameBufferInfo.bpp = *(uint8_t*) (ptr + 28);
    frameBufferInfo.type = *(uint8_t*) (ptr + 29);
}

void VGA::loadVGAMode(const Mode& mode) {
    setBitsPort('MISC', 0, mode.miscellaneousOutput, 0b11101111);
    //Sequencer Registers
    setBitsPort('SEQR', 0, mode.sequencer.reset, 0b00000011);
    setBitsPort('SEQR', 1, mode.sequencer.clockingMode, 0b00111101);
    setBitsPort('SEQR', 2, mode.sequencer.mapMask, 0b00001111);
    setBitsPort('SEQR', 3, mode.sequencer.characterMapSelect, 0b00111111);
    setBitsPort('SEQR', 4, mode.sequencer.sequencerMemoryMode, 0b00001110);

    // blank and unlock
    setBitsPort('CRTC', 3, 0b10000000, 0b10000000);
    setBitsPort('CRTC', 0x11, 0b0, 0b10000000);

    uint8_t endHorizontalBlanking = readPort('CRTC', 3) | 0x80;
    uint8_t verticalRetraceEnd = readPort('CRTC', 0x11) & ~0x80;

    // write crtc

    setBitsPort('CRTC', 0x00, mode.crtc.horizontalTotal, 0xFF);
    setBitsPort('CRTC', 0x01, mode.crtc.endHorizontalDisplay, 0xFF);
    setBitsPort('CRTC', 0x02, mode.crtc.startHorizontalBlanking, 0xFF);
    setBitsPort('CRTC', 0x03, endHorizontalBlanking, 0xFF);
    setBitsPort('CRTC', 0x04, mode.crtc.startHorizontalRetrace, 0xFF);
    setBitsPort('CRTC', 0x05, mode.crtc.endHorizontalRetrace, 0xFF);
    setBitsPort('CRTC', 0x06, mode.crtc.verticalTotal, 0xFF);
    setBitsPort('CRTC', 0x07, mode.crtc.overflow, 0xFF);
    setBitsPort('CRTC', 0x08, mode.crtc.presetRowScan, 0xFF);
    setBitsPort('CRTC', 0x09, mode.crtc.maximumScanLine, 0xFF);
    setBitsPort('CRTC', 0x0A, mode.crtc.cursorStart, 0xFF);
    setBitsPort('CRTC', 0x0B, mode.crtc.cursorEnd, 0xFF);
    setBitsPort('CRTC', 0x0C, mode.crtc.startAddressHigh, 0xFF);
    setBitsPort('CRTC', 0x0F, mode.crtc.startAddressLow, 0xFF);
    setBitsPort('CRTC', 0x0E, mode.crtc.cursorLocationHigh, 0xFF);
    setBitsPort('CRTC', 0x0F, mode.crtc.cursorLocationLow, 0xFF);
    setBitsPort('CRTC', 0x10, mode.crtc.verticalRetraceStart, 0xFF);
    setBitsPort('CRTC', 0x11, verticalRetraceEnd, 0xFF);
    setBitsPort('CRTC', 0x12, mode.crtc.verticalDisplayEnd, 0xFF);
    setBitsPort('CRTC', 0x13, mode.crtc.offset, 0xFF);
    setBitsPort('CRTC', 0x14, mode.crtc.underlineLocation, 0xFF);
    setBitsPort('CRTC', 0x15, mode.crtc.startVerticalBlanking, 0xFF);
    setBitsPort('CRTC', 0x16, mode.crtc.endVerticalBlanking, 0xFF);
    setBitsPort('CRTC', 0x17, mode.crtc.CRTCModeControl, 0xFF);
    setBitsPort('CRTC', 0x18, mode.crtc.lineCompare, 0xFF);

    // write graphics
    setBitsPort('GRAR', 0x00, mode.graphics.setReset, 0x0F);
    setBitsPort('GRAR', 0x01, mode.graphics.enableSetReset, 0x0F);
    setBitsPort('GRAR', 0x02, mode.graphics.colorCompare, 0x0F);
    setBitsPort('GRAR', 0x03, mode.graphics.dataRotate, 0x1F);
    setBitsPort('GRAR', 0x04, mode.graphics.readMapSelect, 0b11);
    setBitsPort('GRAR', 0x05, mode.graphics.graphicsMode, 0b01111011);
    setBitsPort('GRAR', 0x06, mode.graphics.miscGraphics, 0x0F);
    setBitsPort('GRAR', 0x07, mode.graphics.colorDontCare, 0x0F);
    setBitsPort('GRAR', 0x08, mode.graphics.bitMask, 0xFF);

    //write attributes
    for (uint8_t i = 0; i < 16; ++i) {
        writePort('ATTR', i, mode.attributes.palette[i]);
    }
    writePort('ATTR', 0x10, mode.attributes.attributeModeControl);
    writePort('ATTR', 0x11, mode.attributes.overscanColor);
    writePort('ATTR', 0x12, mode.attributes.colorPlaneEnable);
    writePort('ATTR', 0x13, mode.attributes.horizontalPixelPanning);
    writePort('ATTR', 0x14, mode.attributes.colorSelect);

    // unblank (and lock)
    outb(0x3C0, inb(0x3C0) | 1 << 5);

    setPixel = mode.setPixel;
    clearFrame = mode.clearFrame;
    width = mode.width;
    height = mode.height;
}

void VGA::drawPixel(uint64_t x, uint64_t y, uint8_t color) {
    if (setPixel) setPixel(this, x, y, color);
}

void VGA::clear() {
    if (clearFrame) clearFrame(this);
}

void VGA::setPlane(uint8_t plane) {
    plane &= 3;
    setBitsPort('GRAR', 0x04, plane, 0b11);
    setBitsPort('SEQR', 0x02, 1 << plane, 0b1111);
}
uint8_t* VGA::getFrameBuffer() {
    return (uint8_t*) 0xA0000;
}

void VGA::drawImage(uint64_t xBase, uint64_t yBase, uint64_t width, uint64_t height, const uint8_t* image, uint64_t scaleX, uint64_t scaleY, uint8_t transparentColor) {
    for (uint64_t offsetY = 0; offsetY < height; ++offsetY) {
        for (uint64_t offsetX = 0; offsetX < width; ++offsetX) {
            uint64_t x = offsetX + xBase;
            uint64_t y = offsetY + yBase;
            uint64_t index = (offsetX / 2) + ((width / 2) * offsetY);
            uint8_t color = (image[index] >> (((x ^ 0b1) & 0b1) * 4)) & 0xF;
            if (color != transparentColor) {
                for (uint64_t i = 0; i < scaleY; ++i) {
                    for (uint64_t j = 0; j < scaleX; ++j) {
                        drawPixel(x * scaleX + j, y * scaleY + i, color);
                    }
                }
            }
        }
    }
}

VGA* VGA::openController(PCI* device, const PCICommonHeader& header) {
    device->load();
    VGA* instance = new VGA();
    instance->loadVGAMode(video640x480);
    instance->clear();
    for (uint8_t i = 0; i < 200; ++i) {
        for (uint8_t j = 0; j < 200; ++j) {
            instance->drawPixel(j, i, (i / 20 + j / 20) % 16);
        }
    }
    return instance;
}

uint64_t VGA::readPort(uint32_t port, uint64_t index) {
    switch (port) {
        // attributes:
        case 'ATTR': {
            readPort('STA1', 0);
            outb(0x3C0, (uint8_t) index);
            uint8_t value = inb(0x3C1);
            return value;
        }
        // external register:
        case 'STA1': {
            setBitsPort('MISC', 0, 1, 0b1);
            uint8_t value = inb(0x3DA);
            return value;
        }
        case 'STA0': {
            uint8_t value = inb(0x3C2);
            return value;
        }
        case 'FCR': {
            uint8_t value = inb(0x3CA);
            return value;
        }
        case 'MISC': {
            uint8_t value = inb(0x3CC);
            return value;
        }
        case 'CRTC': {
            setBitsPort('MISC', 0, 1, 0b1);
            outb(0x3D4, (uint8_t) index);
            return inb(0x3D5);
        }
        case 'SEQR': {
            outb(0x3c4, (uint8_t) index);
            return inb(0x3C5);
        }
        case 'GRAR': {
            outb(0x3CE, (uint8_t) index);
            return inb(0x3CF);
        }
        case 'DAC': {
            outb(0x3C7, (uint8_t) index);
            uint64_t value = inb(0x3C9) & ((1 << 6) - 1);
            value |= (inb(0x3C9) & ((1 << 6) - 1)) >> 8;
            value |= (inb(0x3C9) & ((1 << 6) - 1)) >> 16;
            return value;
        }
        default: {
            asm("int3");
        }
    }
    return 0;
}

void VGA::setBitsPort(uint32_t port, uint64_t index, uint64_t data, uint64_t bits) {
    switch (port) {
        // attributes:
        case 'ATTR': {
            readPort('STA1', 0);
            outb(0x3C0, (uint8_t) index);
            uint8_t value = inb(0x3C1);
            value &= ~(uint8_t) bits;
            outb(0x3C0, value | (uint8_t) (bits & data));
            return;
        }
        // external register:
        case 'FCR': {
            setBitsPort('MISC', 0, 1, 0b1);
            uint8_t value = inb(0x3CA);
            value &= ~((uint8_t) bits);
            outb(0x3DA, value | (uint8_t) (bits & data));
            return;
        }
        case 'MISC': {
            uint8_t value = inb(0x3CC);
            value &= ~((uint8_t) bits);
            outb(0x3C2, value | (uint8_t) (bits & data));
            return;
        }
        case 'CRTC': {
            setBitsPort('MISC', 0, 1, 0b1);
            outb(0x3D4, (uint8_t) index);
            uint8_t value = inb(0x3D5);
            value &= ~((uint8_t) bits);
            outb(0x3D5, value | (uint8_t) (bits & data));
            return;
        }
        case 'SEQR': {
            outb(0x3C4, (uint8_t) index);
            uint8_t value = inb(0x3C5);
            value &= ~((uint8_t) bits);
            outb(0x3C5, value | (uint8_t) (bits & data));
            return;
        }
        case 'GRAR': {
            outb(0x3CE, (uint8_t) index);
            uint8_t value = inb(0x3CF);
            value &= ~((uint8_t) bits);
            outb(0x3CF, value | (uint8_t) (bits & data));
            return;
        }
        default: {
            asm("int3");
        }
    }
}

void VGA::writePort(uint32_t port, uint64_t index, uint64_t data) {
    switch (port) {
        // attributes:
        case 'ATTR': {
            readPort('STA1', 0);
            outb(0x3C0, (uint8_t) index);
            outb(0x3C0, (uint8_t) data);
            return;
        }
        // external register:
        case 'FCR': {
            setBitsPort('MISC', 0, 1, 0b1);
            outb(0x3DA, (uint8_t) data);
            return;
        }
        case 'MISC': {
            outb(0x3C2, (uint8_t) data);
            return;
        }
        case 'CRTC': {
            setBitsPort('MISC', 0, 1, 0b1);
            outb(0x3D4, (uint8_t) index);
            outb(0x3D5, (uint8_t) data);
            return;
        }
        case 'SEQR': {
            outb(0x3C4, (uint8_t) index);
            outb(0x3C5, (uint8_t) data);
            return;
        }
        case 'GRAR': {
            outb(0x3CE, (uint8_t) index);
            outb(0x3CF, (uint8_t) data);
            return;
        }
        case 'DAC': {
            outb(0x3C8, (uint8_t) index);
            outb(0x3C9, (uint8_t) (data & ((1 << 6) - 1)));
            outb(0x3C9, (uint8_t) ((data >> 8) & ((1 << 6) - 1)));
            outb(0x3C9, (uint8_t) ((data >> 16) & ((1 << 6) - 1)));
            return;
        }
        default: {
            asm("int3");
        }
    }
}

uint64_t VGA::getStatus() {
    return 1llu << 63;
};
uint64_t VGA::getArgSize(uint8_t argNum) {
    constexpr uint64_t argSizes[]{0, 1, 1};
    return argNum < (sizeof(argSizes) / sizeof(uint64_t)) ? argSizes[argNum] : 0;
};
void VGA::handleDriverCall(uint8_t argNum, void* arg) {
    switch (argNum) {
        case 1:
            *(uint64_t*) arg = width;
            return;
        case 2:
            *(uint64_t*) arg = height;
            return;
        case 3: {
            VGA::Mode* mode = (VGA::Mode*) arg;
            loadVGAMode(*mode);
            return;
        }
        case 4: {
            struct BufferDraw {
                uint64_t start;
                uint64_t bufferSize;
                uint8_t buffer[1Ki];
            } __attribute__((packed));
            BufferDraw* draw = (BufferDraw*) arg;
            //todo: optimise
            for (uint64_t i = 0; i < draw->bufferSize; ++i) {
                uint64_t x = (i + draw->start) % width;
                uint64_t y = (i + draw->start) / height;
                drawPixel(x, y, draw->buffer[i]);
            }
            return;
        }
    }
};

}// namespace Kernel