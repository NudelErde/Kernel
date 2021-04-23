#include "interrupt.hpp"
#include "stdint.h"
#include "stddef.h"
#include "inout.hpp"
#include "print.hpp"

extern uint8_t asmInterruptTable[] asm("asmInterruptTable");

namespace Kernel {

static void PIC_remap(int offset1, int offset2)
{
    constexpr uint32_t PIC1 = 0x20;
    constexpr uint32_t PIC2 = 0xA0;
    constexpr uint32_t PIC1_COMMAND = PIC1;
    constexpr uint32_t PIC1_DATA = (PIC1+1);
    constexpr uint32_t PIC2_COMMAND = PIC2;
    constexpr uint32_t PIC2_DATA = (PIC2+1);

    constexpr uint32_t ICW1_ICW4 = 0x01;
    constexpr uint32_t ICW1_SINGLE = 0x02;
    constexpr uint32_t ICW1_INTERVAL4 = 0x04;
    constexpr uint32_t ICW1_LEVEL = 0x08;
    constexpr uint32_t ICW1_INIT = 0x10;
    constexpr uint32_t ICW4_8086 = 0x01;
    constexpr uint32_t ICW4_AUTO = 0x02;
    constexpr uint32_t ICW4_BUF_SLAV = 0x0;
    constexpr uint32_t ICW4_BUF_MASTE = 0x0;
    constexpr uint32_t ICW4_SFNM = 0x10;
	unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
}

static void(*interrupts[256])();

static void build_interrupt(uint8_t gate[16], uint64_t address) {
    if(address == 0) {
        gate[5] |= 0b00001110; // set type to interrupt gate
        gate[5] |= 0b00000000; // set to not present
        gate[5] |= 0b00000000; // set ring level 0 / kernel level access
        
        gate[ 2] = 0x08; // dont know dont ask pls
        return;
    }
    gate[5] |= 0b00001110; // set type to interrupt gate
    gate[5] |= 0b10000000; // set present
    gate[5] |= 0b00000000; // set ring level 0 / kernel level access
    
    gate[ 2] = 0x08; // dont know dont ask pls

    gate[ 0] = address & 0xFF;
    gate[ 1] = (address >>  8) & 0xFF;
    gate[ 6] = (address >> 16) & 0xFF;
    gate[ 7] = (address >> 24) & 0xFF;
    gate[ 8] = (address >> 32) & 0xFF;
    gate[ 9] = (address >> 40) & 0xFF;
    gate[10] = (address >> 48) & 0xFF;
    gate[11] = (address >> 56) & 0xFF;
}

static void build_interrupt_table(uint8_t table[16 * 256], void* interruptHandler[256]) {
    for(size_t i = 0; i < 256; ++i) {
        if(interruptHandler[i] != nullptr) {
            build_interrupt(table + (16 * i), (uint64_t)interruptHandler[i]);
        }
    }
}

static void noDefaultInterruptHandlerFound(const Interrupt& inter){
    using namespace Kernel;
    Print::println("No default interrupt handler");
    Print::print("Interrupt id: ");
    Print::println((int)inter.interruptNumber);
    if(inter.isHardwareInterrupt) {
        if(inter.interruptNumber > 248) {
            outb(0xA0, 0x20);
        }
        outb(0x20, 0x20);
    }
    asm("hlt");
}

static Interrupt::Handler interruptHandler[256] = {};
static Interrupt::Handler defaultInterruptHandler = noDefaultInterruptHandlerFound;

void Interrupt::setHandler(uint8_t interruptNumber, Interrupt::Handler handler) {
    interruptHandler[interruptNumber] = handler;
}
Interrupt::Handler Interrupt::getHandler(uint8_t interruptNumber) {
    return interruptHandler[interruptNumber];
}

void Interrupt::setDefaultHandler(Interrupt::Handler handler) {
    defaultInterruptHandler = handler;
}
Interrupt::Handler Interrupt::getDefaultHandler() {
    return defaultInterruptHandler;
}

void Interrupt::setInterrupt(uint8_t interruptID, void(*func)()) {
    build_interrupt(asmInterruptTable + (16*interruptID), (uint64_t)(void*)func);
}

void Interrupt::init() {
    asm("cli"); // disable interrupt
    outb(0x21, 0b11111011u); //disble master hardware interrupts except interrupts from slave
    outb(0xA1, 0b11111111u); //disable slave hardware interrupts
    void *_interruptHandler[256]{};

    build_interrupt_table(asmInterruptTable, _interruptHandler);

    PIC_remap(240, 248);

    uint8_t tableData[10];                                 // setup value for lidt
    tableData[0] = 0xFF;                                   // sizeof this shit in bytes - 1
    tableData[1] = 0x0F;                                   // 256 * 16 = 4069 | 4096 - 1 = 0x0FFF
    uint64_t tableAddress = (uint64_t)&asmInterruptTable;  //
    tableData[2] = (uint8_t)(tableAddress & 0xFF);         // store pointer of asmInterruptTable in value thing
    tableData[3] = (uint8_t)((tableAddress >> 8) & 0xFF);  //
    tableData[4] = (uint8_t)((tableAddress >> 16) & 0xFF); //
    tableData[5] = (uint8_t)((tableAddress >> 24) & 0xFF); //
    tableData[6] = (uint8_t)((tableAddress >> 32) & 0xFF); //
    tableData[7] = (uint8_t)((tableAddress >> 40) & 0xFF); //
    tableData[8] = (uint8_t)((tableAddress >> 48) & 0xFF); //
    tableData[9] = (uint8_t)((tableAddress >> 56) & 0xFF); //
    asm("lidt %0" ::"m"(tableData));                       // store value with lidt
    asm("sti");                                            // enable interrupts
}

void IRQ_set_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}
 
void IRQ_clear_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}

void Interrupt::setHardwareInterruptMask(uint8_t irq, bool enable) {
    if(!enable) {
        IRQ_set_mask(irq);
    } else {
        IRQ_clear_mask(irq);
    }
}

void Interrupt::sendHardwareEOI(uint8_t irq) {
    if(irq > 8) {
        outb(0xA0, 0x20);
    }
	outb(0x20, 0x20);
}

}