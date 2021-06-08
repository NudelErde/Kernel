#include "serial.hpp"
#include "inout.hpp"
#include "print.hpp"

namespace Kernel {

Serial::Serial(uint16_t address) {
    Serial::address = address;
}
void Serial::start() {
    //Baudrate 9600 (divider = 12)
    uint16_t divider = 3;
    outb(address + 1, 0b00000000);//interrupt disable
    outb(address + 3, 0b10000000);// DLAB on
    outb(address + 0, divider & 0xFFu);
    outb(address + 1, (divider >> 8) & 0xFFu);
    outb(address + 3, 0b00000011);//8 bits | 1 stopbit | parity none
    outb(address + 2, 0b11000111);//Enable FIFO, clear them, with 14-byte threshold
    outb(address + 4, 0b00011110);//DTR on | RTS on | enable interrupts | loopback

    outb(address + 0, 0xAB);
    uint8_t data = inb(address + 0);

    if (data != 0xAB) {
        setError(0x01);
        return;
    }

    outb(address + 4, 0x0F);//DTR on | RTS on | enable interrupts
}
void Serial::write(uint8_t data) {
    outb(address + 0, data);
    checkHardwareError();
}
uint8_t Serial::read() {
    uint8_t value = inb(address + 0);
    checkHardwareError();
    return value;
}

bool Serial::outputBufferEmpty() {
    return (inb(address + 5) >> 5) & 0b1;
}
bool Serial::inputBufferEmpty() {
    return !(inb(address + 5) & 0b1);
}

void Serial::setError(uint32_t error) {
    Serial::error = error;
}

uint32_t Serial::getError() {
    return error;
}

void Serial::checkHardwareError() {
    uint8_t err = inb(address + 5) & 0b10011110;
    if (err != 0) {
        setError(err);
    }
}

void Serial::printHex(uint64_t hex, uint8_t size) {
    printBaseN(hex, 16, size);
}

void Serial::print(const char* str) {
    for (; *str; ++str) {
        print(*str);
    }
}

void Serial::print(char c) {
    while (!outputBufferEmpty())
        ;
    write(c);
}

void Serial::printBaseN(uint64_t number, uint8_t base, uint8_t size) {
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

    for (uint8_t i = 0; i < size - index + 1; ++i) {
        print('0');
    }

    buffer[63] = 0;// always terminated pls
    for (char* ptr = buffer + index - 1; *ptr; --ptr) {
        print(*ptr);
    }
}

}// namespace Kernel