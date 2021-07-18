#include "PS2.hpp"
#include "ACPI.hpp"
#include "APIC.hpp"
#include "KernelOut.hpp"
#include "inout.hpp"
#include "interrupt.hpp"
#include "stdint.h"

namespace Kernel {

static constexpr uint16_t dataPort = 0x60;
static constexpr uint16_t scRegister = 0x64;// status controll

static bool disabled;
static bool secondPort;
static uint8_t useablePorts;
static uint16_t portType[2];
static uint64_t ps2Error;
static void (*interruptOverride[2])(uint8_t);

static constexpr uint16_t queueSize = 256;

struct Ps2DataQueue {
    uint16_t enqueueIndex;
    uint16_t dequeueIndex;
    uint8_t queue[queueSize];
};

enum class Ps2PortState {
    NONE,
    PRE_IDENTIFY,
    IDENTIFY,
    POST_IDENTIFY,
    DONE
};

static Ps2PortState portState[2]{};
static Ps2DataQueue portQueue[2]{};

static uint8_t readData(uint16_t port) {
    while (!(inb(scRegister) & 0b1)) {}
    return inb(port);
}

static void writeData(uint16_t port, uint8_t data) {
    while (inb(scRegister) & 0b10) {}
    outb(port, data);
}

static void sendToDevice(uint8_t port, uint8_t data) {
    if (port == 1) {
        writeData(scRegister, 0xD4);// send to port 2
    }
    writeData(dataPort, data);// reset and selftest
}

static void onReceive(uint8_t port) {
    uint8_t data = inb(dataPort);
    switch (portState[port]) {
        case Ps2PortState::NONE:
            if (data == 0xFA) {
                return;
            }

            if (data != 0xAA) {
                kout << "PS/2 Port " << (uint64_t) (port + 1) << " reset error: 0x" << Hex(data) << '\n';
                break;
            }
            sendToDevice(port, 0xF5);// disable scan
            portState[port] = Ps2PortState::PRE_IDENTIFY;
            break;
        case Ps2PortState::PRE_IDENTIFY:
            if (data != 0xFA) {
                kout << "PS/2 Port " << (uint64_t) (port + 1) << " did not ACK disable scanning\n";
                break;
            }
            sendToDevice(port, 0xF2);// identify
            portState[port] = Ps2PortState::IDENTIFY;
            break;
        case Ps2PortState::IDENTIFY:
            if (data == 0xFA) {
                break;
            }
            portType[port] <<= 8;
            portType[port] |= data;
            if (data != 0xAB) {
                sendToDevice(port, 0xF4);// enable scan
                portState[port] = Ps2PortState::POST_IDENTIFY;
            }
            break;
        case Ps2PortState::POST_IDENTIFY:
            if (data != 0xFA) {
                kout << "PS/2 Port " << (uint64_t) (port + 1) << " did not ACK enable scanning\n";
                break;
            }
            //kout << "PS/2 Port " << (uint64_t) (port + 1) << " device type: " << Hex(portType[port]) << '\n';
            portState[port] = Ps2PortState::DONE;
            break;
        case Ps2PortState::DONE: {
            if (data == 0xFA) {
                return;
            }
            Ps2DataQueue* queue = &portQueue[port];
            uint16_t nextEnqueue = queue->enqueueIndex + 1;
            nextEnqueue %= queueSize;
            if (nextEnqueue == queue->dequeueIndex) {
                // queue full
                ps2Error |= (0b1 << (port * 8 + 1));// overflow
            } else {
                queue->queue[queue->enqueueIndex] = data;
                queue->enqueueIndex = nextEnqueue;
            }
            break;
        }
        default:
            break;
    }
}

void PS2::disable() {
    disabled = true;
}

uint8_t PS2::getPorts() {
    return useablePorts;
}
uint16_t PS2::getType(uint8_t port) {
    if (port > 2) {
        ps2Error |= ((uint64_t) 0b1 << 32);// invalid port
        return 0;
    }
    if (!(useablePorts & (0b1 << port))) {
        ps2Error |= ((uint64_t) 0b1 << 32);// invalid port
        return 0;
    }
    return portType[port];
}
bool PS2::isEmpty(uint8_t port) {
    if (port > 2) {
        ps2Error |= ((uint64_t) 0b1 << 32);// invalid port
        return 0;
    }
    if (!(useablePorts & (0b1 << port))) {
        ps2Error |= ((uint64_t) 0b1 << 32);// invalid port
        return 0;
    }
    return portQueue[port].dequeueIndex == portQueue[port].enqueueIndex;
}
uint8_t PS2::read(uint8_t port) {
    if (port > 2) {
        ps2Error |= ((uint64_t) 0b1 << 32);// invalid port
        return 0;
    }
    if (!(useablePorts & (0b1 << port))) {
        ps2Error |= ((uint64_t) 0b1 << 32);// invalid port
        return 0;
    }
    if (isEmpty(port)) {
        ps2Error |= (0b1 << (port * 8 + 2));// underflow
        return 0;
    }
    uint8_t data = portQueue[port].queue[portQueue[port].dequeueIndex];
    ++portQueue[port].dequeueIndex;
    portQueue[port].dequeueIndex %= queueSize;
    return data;
}

uint64_t PS2::getError() {
    uint64_t error = ps2Error;
    ps2Error = 0;
    return error;
}

void PS2::write(uint8_t port, uint8_t data) {
    if (port > 2) {
        ps2Error |= ((uint64_t) 0b1 << 32);// invalid port
        return;
    }
    if (!(useablePorts & (0b1 << port))) {
        ps2Error |= ((uint64_t) 0b1 << 32);// invalid port
        return;
    }
    sendToDevice(port, data);
}
void PS2::setInterruptOverride(uint8_t port, void (*fn)(uint8_t)) {
    if (port > 2) {
        ps2Error |= ((uint64_t) 0b1 << 32);// invalid port
        return;
    }
    if (!(useablePorts & (0b1 << port))) {
        ps2Error |= ((uint64_t) 0b1 << 32);// invalid port
        return;
    }
    interruptOverride[port] = fn;
}

void PS2::init() {
    useablePorts = 0;
    if (disabled) {
        kout << "No PS/2 controller\n";
        return;
    }
    kout << "PS/2 controller found!\n";
    writeData(scRegister, 0xAD);// disable port 1
    writeData(scRegister, 0xA7);// disable port 2
    inb(dataPort);              // clear output buffer

    writeData(scRegister, 0x20);// read configuration byte
    uint8_t configByte = readData(dataPort);
    if (configByte & (0b1 << 5)) {
        secondPort = false;
    } else {
        secondPort = true;
    }
    configByte &= ~((0b1 << 0) | (0b1 << 1) | (0b1 << 6));// disable bit 0,1,6 (interrupts and translation)
    writeData(scRegister, 0x60);
    writeData(dataPort, configByte);//write config

    writeData(scRegister, 0xAA);// controller selftest
    uint8_t cstr = readData(dataPort);
    if (cstr != 0x55) {
        kout << "Invalid PS/2 controller selftest\n";
        return;
    }

    writeData(scRegister, 0x60);
    writeData(dataPort, configByte);//restore config

    writeData(scRegister, 0xAB);
    uint8_t p1str = readData(dataPort);
    if (p1str == 0x00) {
        useablePorts |= 0b1;
    }

    if (secondPort) {
        writeData(scRegister, 0xA9);
        uint8_t p2str = readData(dataPort);
        if (p2str == 0x00) {
            useablePorts |= 0b10;
        }
    }

    if (useablePorts == 0) {
        kout << "No useable PS/2 ports found\n";
        return;
    }

    if (useablePorts & 0b1) {
        // enable port 1
        Interrupt::setHandler(241, [](const Interrupt& inter) {
            if (!useablePorts & 0b1) {
                Interrupt::sendHardwareEOI(inter.interruptNumber);
                return;
            }
            if (interruptOverride[0]) {
                uint8_t data = inb(dataPort);
                interruptOverride[0](data);
            } else {
                onReceive(0);
            }
            Interrupt::sendHardwareEOI(inter.interruptNumber);
        });
    }

    if (useablePorts & 0b10) {
        Interrupt::setHandler(252, [](const Interrupt& inter) {
            if (!useablePorts & 0b10) {
                Interrupt::sendHardwareEOI(inter.interruptNumber);
                return;
            }
            if (interruptOverride[1]) {
                uint8_t data = inb(dataPort);
                interruptOverride[1](data);
            } else {
                onReceive(1);
            }
            Interrupt::sendHardwareEOI(inter.interruptNumber);
        });
    }

    // enable interrupts
    writeData(scRegister, 0x20);// read configuration byte
    configByte = readData(dataPort);
    configByte |= useablePorts;
    writeData(scRegister, 0x60);
    writeData(dataPort, configByte);// write config

    if (useablePorts & 0b1) {
        // enable port 1
        writeData(scRegister, 0xAE);// enable
        sendToDevice(0, 0xFF);
    }

    if (useablePorts & 0b10) {
        // enable port 2
        writeData(scRegister, 0xA8);// enable
        sendToDevice(1, 0xFF);
    }
}

}// namespace Kernel