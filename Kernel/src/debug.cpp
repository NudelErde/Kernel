#include "debug.hpp"
#include "KernelOut.hpp"
#include "process.hpp"
#include "tss.hpp"
#include "interrupt.hpp"

struct PrintPointer{
    uint64_t pointer;
    PrintPointer(uint64_t pointer): pointer(pointer){}
};

extern uint8_t kernel_start;
extern uint8_t boot_end;
extern uint8_t code_end;
extern uint8_t kernel_end;

Kernel::KernelOut& operator<<(Kernel::KernelOut& out, PrintPointer pp) {
    using namespace Kernel;
    uint64_t pointer = pp.pointer;
    out << Hex(pointer);
    if(pointer < (uint64_t)(&kernel_start)) {
        out << " (low data)";
    } else if(pointer <= (uint64_t)(&boot_end)) {
        out << " (boot)";
    } else if(pointer <= (uint64_t)(&code_end)) {
        out << " (kernel code)";
    } else if(pointer <= (uint64_t)(&kernel_end)) {
        out << " (kernel data)";
    } else {
        out << " (high data)";
    }
    TSS* tss = getTss();
    if(tss) {
        if (pointer <= tss->ist1 && pointer > tss->ist1 - stackPageCount * pageSize) {
            out << " (Interrupt stack)";
        } else if (pointer <= tss->ist2 && pointer > tss->ist2 - stackPageCount * pageSize) {
            out << " (Panic stack)";
        } else if (pointer <= tss->rsp0 && pointer > tss->rsp0 - stackPageCount * pageSize) {
            out << " (Ring 0 stack)";
        }
    }
    if(Thread::isInProgram()) {
        uint64_t threadBaseAddress = Thread::getCurrent()->getStackBaseAddress();
        if(pointer >= threadBaseAddress && pointer < threadBaseAddress + stackPageCount * pageSize) {
            out << " (Thread stack)";
        }
    }
    return out;
}

struct PrintSegment{
    uint16_t segmentSelector;
    PrintSegment(uint16_t segmentSelector): segmentSelector(segmentSelector){}
};

Kernel::KernelOut& operator<<(Kernel::KernelOut& out, PrintSegment ps) {
    out << (uint64_t)(ps.segmentSelector >> 3);
    out << " (" << ((ps.segmentSelector & 0b100)?'L':'G') << "DT) (Ring ";
    out << (uint64_t)(ps.segmentSelector & 0b11) << ')';
    return out;
}

namespace Kernel {

static struct {
    typedef bool(*Callback)();

    Callback callbacks[6]; // index 4 is single step; index 5 is breakpoint instruction
} data;

void onInvalidOpcode(const Interrupt& i) {
    kout << "Invalid opcode\n";
    Debug::printDebugInfo(i.stackFrame);
    asm("hlt");
}

static bool tryHandle(const Interrupt& i, uint8_t number) {
    if(number < 4) {
        if(data.callbacks[number]) {
            if(data.callbacks[number]()) {
                Debug::removeBreakpoint(number);
                return true;
            }
        }
    } else if(number == 4) {
        if(data.callbacks[number]) {
            if(data.callbacks[number]()) {
                ((uint64_t*)i.stackFrame)[2] &= ~0x100;
                return true;
            }
        }
    } else if(number == 5) {
        if(data.callbacks[number]) {
            if(data.callbacks[number]()) {
                return true;
            }
        }
    }
    return false;
}

void onBreakpoint(const Interrupt& i) {
    if(!tryHandle(i, 5)) {
        Debug::printDebugInfo(i.stackFrame);
        ((uint64_t*)i.stackFrame)[2] |= 0x100;
    }
}

void onDebug(const Interrupt& i) {
    uint64_t dr6;
    bool handled = false;
    asm("mov %%dr6, %0":"=g"(dr6)::);
    if(dr6 & 0b1 << 0) {
        handled |= tryHandle(i, 0);
    }
    if(dr6 & 0b1 << 1) {
        handled |= tryHandle(i, 1);
    }
    if(dr6 & 0b1 << 2) {
        handled |= tryHandle(i, 2);
    }
    if(dr6 & 0b1 << 3) {
        handled |= tryHandle(i, 3);
    }
    if(dr6 & 0b1 << 14) {
        handled |= tryHandle(i, 4);
    }
    dr6 &= 0b1111 | (0b111 << 13);
    asm("mov %0, %%dr6"::"a"(dr6):);

    if(!handled) {
        Debug::printDebugInfo(i.stackFrame);
    }
}

void onPureVirtual(const Interrupt& i) {
    kout << "Pure virtual function called\n";
    Debug::printDebugInfo(i.stackFrame);
    asm("hlt");
}

void Debug::init() {
    Interrupt::setHandler(6, onInvalidOpcode);
    Interrupt::setHandler(3, onBreakpoint);
    Interrupt::setHandler(1, onDebug);
    Interrupt::setHandler(33, onPureVirtual);
}

void Debug::removeBreakpoint(uint8_t debugNum) {
    switch(debugNum) {
        case 0: asm("mov %0, %%dr0"::"a"(0):); break;
        case 1: asm("mov %0, %%dr1"::"a"(0):); break;
        case 2: asm("mov %0, %%dr2"::"a"(0):); break;
        case 3: asm("mov %0, %%dr3"::"a"(0):); break;
        default: return;
    }

    uint64_t dr7;
    asm("mov %%dr7, %0":"=a"(dr7)::);
    dr7 &= ~(0b1 << (2 * debugNum + 1)); // disable
    dr7 &= ~((0b1111) << (16 + debugNum * 4)); // clear condition and length field

    asm("mov %0, %%dr7":"=a"(dr7)::);

    data.callbacks[debugNum] = nullptr;
}

void Debug::setBreakpoint(uint8_t debugNum, uint64_t address, Condition c, bool(*callback)()) {
    constexpr uint64_t debugExtensionBit = 0b1 << 3;
    uint64_t cr4;
    asm("mov %%cr4, %0":"=a"(cr4)::);
    cr4 |= debugExtensionBit;
    asm("mov %0, %%cr4"::"a"(cr4):);
    switch(debugNum) {
        case 0: asm("mov %0, %%dr0"::"a"(address):); break;
        case 1: asm("mov %0, %%dr1"::"a"(address):); break;
        case 2: asm("mov %0, %%dr2"::"a"(address):); break;
        case 3: asm("mov %0, %%dr3"::"a"(address):); break;
        default: return;
    }
    uint64_t dr7;
    asm("mov %%dr7, %0":"=a"(dr7)::);
    dr7 |= 0b1 << (2 * debugNum + 1); // enable
    dr7 &= ~((0b1111) << (16 + debugNum * 4)); // clear condition and length field
    dr7 |= ((uint8_t)c) << (16 + debugNum * 4); // condition

    data.callbacks[debugNum] = callback;

    asm("mov %0, %%dr7":"=a"(dr7)::);
}

uint64_t readHex() {
    Serial s(0x3F8);
    uint64_t num = 0;
    while(true) {
        while (s.inputBufferEmpty());
        uint8_t ch = s.read();
        const char *digits = "0123456789ABCDEF";
        if(ch >= 'a' && ch <= 'f') {
            ch += 'A' - 'a';
        }
        for (uint64_t i = 0; i < 16; ++i) {
            if (digits[i] == ch) {
                num *= 16;
                num += i;
                kout << digits[i];
            }
        }
        if (ch == '\n' || ch == '\r')
            break;
    }
    return num;
}

void Debug::printDebugInfo(void* stackPointer) {
    uint64_t* stack = (uint64_t*)stackPointer;
    uint64_t rip = stack[0];
    uint64_t rsp = stack[3];
    uint64_t flags = stack[2];
    uint64_t codeSegment = stack[1];
    uint64_t stackSegment = stack[4];
    kout << (Thread::isInProgram() ? "In program" : "In kernel") << '\n';
    if(Thread::isInProgram()) {
        kout << "Program name: " << Process::getLastLoadedProcess()->getArgumentPointer() << ':' << Process::getLastLoadedProcess()->getPID() << ':' << Thread::getCurrent()->getPID() <<'\n';
        kout << "Static program pages: \n";
        for(uint64_t i = 0; i < Process::getLastLoadedProcess()->count; ++i) {
            kout << Hex(Process::getLastLoadedProcess()->programPages[i].getVirtualAddress()) << '\n';
        }
    }
    uint64_t rbp;
    asm("mov %%rbp, %0":"=a"(rbp)::);
    kout << "Instruction pointer: 0x" << PrintPointer(rip) << '\n';
    kout << "Stack pointer:       0x" << PrintPointer(rsp) << '\n';
    kout << "Stack base pointer:  0x" << PrintPointer(rbp) << '\n';
    kout << "Code segment:          " << PrintSegment(codeSegment) << '\n';
    kout << "Stack segment:         " << PrintSegment(stackSegment) << '\n';
    const char *names[]{"Carry Flag", "Reserved", "Parity Flag", "Reserved", "Auxiliary Carry Flag", "Reserved", "Zero Flag", "Sign Flag", "Trap Flag", "Interrupt Enable Flag", "Direction Flag", "Overflow Flag", "IOPL 	I/O Privilege Level", "IOPL 	I/O Privilege Level", "Nested Task", "Reserved", "Resume Flag", "Virtual-8086 Mode", "Alignment Check / Access Control", "Virtual Interrupt Flag", "Virtual Interrupt Pending", "ID Flag "};
    constexpr uint64_t entries = sizeof(names) / sizeof(*names);

    kout << "Flags: ";
    flags ^= 0b10; // flip always 1 bit
    for(uint64_t i = 0; i < 64; ++i) {
        if(flags & 0b1) {
            if(i < entries) {
                kout << i << ':' << names[i] << ' ';
            } else {
                kout << i << ' ';
            }
        }
        flags >>= 0b1;
    }
    kout << '\n';
    while(true) {
        kout << "Address: 0x";
        uint64_t address = readHex();
        kout << "\nSize: 0x";
        uint64_t size = readHex();
        kout << "\nData: ";
        switch (size) {
            case 1:
                kout << "0x" << Hex(*(uint8_t*)address, 2) << '\n';
                break;
            case 2:
                kout << "0x" << Hex(*(uint16_t*)address, 4) << '\n';
                break;
            case 4:
                kout << "0x" << Hex(*(uint32_t*)address, 8) << '\n';
                break;
            case 8:
                kout << "0x" << Hex(*(uint64_t*)address, 16) << '\n';
                break;
            default:
                kout << '\n' <<HexDump((uint8_t*)address, size) << '\n';
                break;
        }
    }
}

// crack stuff; do not use pls

//enum class BreakCondition{
//    Execute = 0,
//    Write = 1,
//    ReadOrWrite = 3
//};
//
//struct Breakpoint{
//    uint64_t address;
//    bool enabled;
//    BreakCondition condition;
//};
//
//struct DebugModeInformation{
//    bool debugModeEnabled;
//    char inputBuffer[256];
//    uint64_t index;
//    bool exit;
//    Breakpoint breakpoint[4];
//    bool breakpoint0;
//    bool breakpoint1;
//    bool breakpoint2;
//    bool breakpoint3;
//    bool singleStep;
//    bool int3;
//};
//
//static uint64_t instructionTrace[128];
//static uint8_t traceIndex;
//static bool traceInstructions;
//static uint8_t traceReadFrom0;
//    
//
//static DebugModeInformation information;
//constexpr uint64_t debugExtensionBit = 0b1 << 3;
//static bool stringEqu(const char* a, const char* b) {
//    for(;;) {
//        if(*a == 0 && *b == 0) {
//            return true;
//        } else if(*a != *b) {
//            return false;
//        }
//        ++a;
//        ++b;
//    }
//}
//
//static void updateBreakpointInfo() {
//    uint64_t dr7;
//    asm(R"(
//        mov %%dr0, %0
//        mov %%dr1, %1
//        mov %%dr2, %2
//        mov %%dr3, %3
//        mov %%dr7, %4
//    )"
//    :
//    "=g"(information.breakpoint[0].address),
//    "=g"(information.breakpoint[1].address),
//    "=g"(information.breakpoint[2].address),
//    "=g"(information.breakpoint[3].address),
//    "=g"(dr7)
//    :
//    :);
//    information.breakpoint[0].enabled = dr7 & 0b1 << 1;
//    information.breakpoint[1].enabled = dr7 & 0b1 << 3;
//    information.breakpoint[2].enabled = dr7 & 0b1 << 5;
//    information.breakpoint[3].enabled = dr7 & 0b1 << 7;
//    information.breakpoint[0].condition = (BreakCondition)((dr7 >> 16) & 0b11);
//    information.breakpoint[1].condition = (BreakCondition)((dr7 >> 22) & 0b11);
//    information.breakpoint[2].condition = (BreakCondition)((dr7 >> 26) & 0b11);
//    information.breakpoint[3].condition = (BreakCondition)((dr7 >> 30) & 0b11);
//}
//
//static void writeBreakpointInfo() {
//    uint64_t dr7 = 0;
//    if(information.breakpoint[0].enabled)
//        dr7 |= 0b1 << 1;
//    if(information.breakpoint[1].enabled)
//        dr7 |= 0b1 << 3;
//    if(information.breakpoint[2].enabled)
//        dr7 |= 0b1 << 5;
//    if(information.breakpoint[3].enabled)
//        dr7 |= 0b1 << 7;
//
//    dr7 |= ((uint64_t)information.breakpoint[0].condition) << 16;
//    dr7 |= ((uint64_t)information.breakpoint[1].condition) << 22;
//    dr7 |= ((uint64_t)information.breakpoint[2].condition) << 26;
//    dr7 |= ((uint64_t)information.breakpoint[3].condition) << 30;
//    asm(R"(
//        mov %0, %%dr0
//        mov %1, %%dr1
//        mov %2, %%dr2
//        mov %3, %%dr3
//        mov %4, %%dr7
//    )"
//    :
//    :
//    "g"(information.breakpoint[0].address),
//    "g"(information.breakpoint[1].address),
//    "g"(information.breakpoint[2].address),
//    "g"(information.breakpoint[3].address),
//    "a"(dr7)
//    :);
//}
//
//static void printMenu() {
//    if(!information.debugModeEnabled) {
//        kout << "Debug extension not enabled\n";
//    } else {
//        kout << "Debug extension enabled\n";
//    }
//}
//
//static void getInput() {
//    Serial s(0x3F8);
//    kout << "Debug Command: ";
//    information.index = 0;
//    while(true) {
//        if(information.index == 254) {
//            return;
//        }
//        while(s.inputBufferEmpty());
//        uint8_t ch = s.read();
//        if(ch == '\n' || ch == '\r') {
//            information.inputBuffer[information.index] = 0;
//            return;
//        }
//        s.write(ch);
//        information.inputBuffer[information.index++] = ch;
//    }
//}
//
//static uint64_t readHex(Serial s) {
//    uint64_t number = 0;
//    while(true) {
//        while(s.inputBufferEmpty());
//        uint8_t ch = s.read();
//        uint8_t input;
//        if(ch >= '0' && ch <= '9') {
//            input = ch-'0';
//        } else if(ch >= 'A' && ch <= 'F') {
//            input = ch-'A' + 10;
//        } else if(ch >= 'a' && ch <= 'f') {
//            input = ch-'a' + 10;
//        } else if(ch == '\n' || ch == '\r') {
//            break;
//        } else {
//            continue;
//        }
//        number *= 16;
//        number += input;
//        kout << (char)ch;
//    }
//    return number;
//}
//
//static void handleCommand() {
//    kout << '\n';
//    if(stringEqu(information.inputBuffer, "help")) {
//        kout << "Command list: \n";
//        kout << "  help\n";
//        kout << "  enable debug\n";
//        kout << "  disable debug\n";
//        kout << "  exit\n";
//        kout << "  info\n";
//        kout << "  set\n";
//        kout << "  disable\n";
//        kout << "  printHex\n";
//    } else if(stringEqu(information.inputBuffer, "enable debug")) {
//        information.debugModeEnabled = true;
//        uint64_t cr4;
//        asm("mov %%cr4, %0":"=a"(cr4)::);
//        cr4 |= debugExtensionBit;
//        asm("mov %0, %%cr4"::"a"(cr4):);
//        kout << "Enabled debug\n";
//    } else if(stringEqu(information.inputBuffer, "disable debug")) {
//        information.debugModeEnabled = false;
//        uint64_t cr4;
//        asm("mov %%cr4, %0":"=a"(cr4)::);
//        cr4 &= ~debugExtensionBit;
//        asm("mov %0, %%cr4"::"a"(cr4):);
//        kout << "Disabled debug\n";
//    } else if(stringEqu(information.inputBuffer, "exit")) {
//        information.exit = true;
//    } else if(stringEqu(information.inputBuffer, "info")) {
//        kout << "Breakpoints at: \n";
//        updateBreakpointInfo();
//        for(uint64_t i = 0; i < 4; ++i) {
//            kout << "Breakpoint " << i << " : " << Hex(information.breakpoint[i].address) << (information.breakpoint[i].enabled ? " enabled" : " disabled");
//            switch (information.breakpoint[i].condition) {
//            case BreakCondition::Execute:
//                kout << " on execute";
//                break;
//            case BreakCondition::Write:
//                kout << " on write";
//                break;
//            case BreakCondition::ReadOrWrite:
//                kout << " on read or write";
//                break;
//            default:
//                break;
//            }
//            kout << '\n';
//        }
//    } else if(stringEqu(information.inputBuffer, "set")){
//        kout << "Select breakpoint [0-3] ";
//        uint8_t breakpoint;
//        Serial s(0x3F8);
//        while(true) {
//            while(s.inputBufferEmpty());
//            uint8_t ch = s.read();
//            if(ch >= '0' && ch <= '3') {
//                breakpoint = ch -'0';
//                break;
//            }
//        }
//        kout << (uint64_t)breakpoint << '\n';
//        kout << "Select address: ";
//        uint64_t address = readHex(Serial(0x3F8));
//        kout << '\n';
//        kout << "Select condition\n";
//        kout << " [0] Execute\n";
//        kout << " [1] Write\n";
//        kout << " [2] Read or write\n";
//        uint8_t cond;
//        while(true) {
//            while(s.inputBufferEmpty());
//            uint8_t ch = s.read();
//            if(ch >= '0' && ch <= '2') {
//                cond = ch -'0';
//                break;
//            }
//        }
//        if(cond == 2)
//            cond = 3;
//        information.breakpoint[breakpoint].address = address;
//        information.breakpoint[breakpoint].condition = (BreakCondition)cond;
//        information.breakpoint[breakpoint].enabled = true;
//        writeBreakpointInfo();
//    } else if(stringEqu(information.inputBuffer, "disable")) {
//        kout << "Select breakpoint [0-3] ";
//        uint8_t breakpoint;
//        Serial s(0x3F8);
//        while(true) {
//            while(s.inputBufferEmpty());
//            uint8_t ch = s.read();
//            if(ch >= '0' && ch <= '3') {
//                breakpoint = ch -'0';
//                break;
//            }
//        }
//        kout << (uint64_t)breakpoint << '\n';
//        information.breakpoint[breakpoint].enabled = false;
//        writeBreakpointInfo();
//    } else if(stringEqu(information.inputBuffer, "printHex")) {
//        kout << "Select address ";
//        uint64_t address = readHex(Serial(0x3F8));
//        kout <<'\n';
//        kout << "Select size ";
//        uint64_t size = readHex(Serial(0x3F8));
//        kout << '\n';
//        if(size == 0)
//            return;
//        if(size <= 8) {
//            uint64_t data = *(uint64_t*)address;
//            kout << Hex(data & ((0x100ull << ((size-1)*8)) - 1u), size*2) << '\n';
//        } else {
//            kout << HexDump((uint8_t*)address, size) << '\n';
//        }
//    } else if(stringEqu(information.inputBuffer, "printTrace")) {
//        for(uint8_t i = 0; i < 128; ++i) {
//            kout << Hex(i,2) << ": 0x" << Hex(instructionTrace[(i + (traceReadFrom0 ? 0 : traceIndex)) % 128]) << '\n';
//        }
//    }
//}
//
//static void debug() {
//    uint64_t cr4;
//    asm("mov %%cr4, %0":"=a"(cr4)::);
//    information.debugModeEnabled = cr4 & debugExtensionBit;
//    while(!information.exit) {
//        printMenu();
//        getInput();
//        handleCommand();
//    }
//    for(uint64_t i = 0; i < sizeof(DebugModeInformation); ++i) {
//        ((uint8_t*)(&information))[i] = 0;
//    }
//}
//
//void openDebug() {
//    debug();
//}
//
//static uint64_t lastCodeAddress{};
//
//void debugHandler(void* stackPointer) {
//    uint64_t dr6;
//    asm("mov %%dr6, %0":"=g"(dr6)::);
//    if(dr6 & 0b1 << 0) {
//        information.breakpoint0 = true;
//    }
//    if(dr6 & 0b1 << 1) {
//        information.breakpoint1 = true;
//    }
//    if(dr6 & 0b1 << 2) {
//        information.breakpoint2 = true;
//    }
//    if(dr6 & 0b1 << 3) {
//        information.breakpoint3 = true;
//    }
//    if(dr6 & 0b1 << 14) {
//        information.singleStep = true;
//    }
//    dr6 &= 0b1111 | (0b111 << 13);
//    asm("mov %0, %%dr6"::"a"(dr6):);
//    if(traceInstructions && information.singleStep) {
//        instructionTrace[traceIndex++] = ((uint64_t*)stackPointer)[0];
//        if(traceIndex == 128) {
//            traceReadFrom0 = false;
//            traceIndex = 0;
//        }
//        return;
//    }
//    printDebugInfo(stackPointer);
//}
//
//void printDebugInfo(void* stackPointer) {
//    uint64_t* stack = (uint64_t*)stackPointer;
//
//    if(traceInstructions) {
//        stack[2] &= ~0x100;
//    }
//    traceInstructions = false;
//    if(information.breakpoint0) kout << "Breakpoint 0\n";
//    if(information.breakpoint1) kout << "Breakpoint 1\n";
//    if(information.breakpoint2) kout << "Breakpoint 2\n";
//    if(information.breakpoint3) kout << "Breakpoint 3\n";
//    if(information.singleStep) kout << "Single step\n";
//    
//    uint64_t rip = stack[0];
//    uint64_t rsp = stack[3];
//    uint64_t flags = stack[2];
//    uint64_t codeSegment = stack[1];
//    uint64_t stackSegment = stack[4];
//    kout << (Thread::isInProgram() ? "In program" : "In kernel") << '\n';
//    if(Thread::isInProgram()) {
//        kout << "Program name: " << Process::getLastLoadedProcess()->getArgumentPointer() << ':' << Process::getLastLoadedProcess()->getPID() << ':' << Thread::getCurrent()->getPID() <<'\n';
//    }
//    kout << "Instruction pointer: 0x" << PrintPointer(rip) << '\n';
//    uint64_t diff = rip - lastCodeAddress;
//    if(lastCodeAddress) {
//        if(diff & 0b1ull<<63)
//            kout << "Instruction pointer diff: -" << (uint64_t)(((int64_t)diff) * -1)<< '\n';
//        else    
//            kout << "Instruction pointer diff: " << diff << '\n';
//    }
//    lastCodeAddress = rip;
//    kout << "Stack pointer:       0x" << PrintPointer(rsp) << '\n';
//    kout << "Code segment:        0b" << Bin(codeSegment, 16) << '\n';
//    kout << "Stack segment:       0b" << Bin(stackSegment, 16) << '\n';
//    const char *names[]{"Carry Flag", "Reserved", "Parity Flag", "Reserved", "Auxiliary Carry Flag", "Reserved", "Zero Flag", "Sign Flag", "Trap Flag", "Interrupt Enable Flag", "Direction Flag", "Overflow Flag", "IOPL 	I/O Privilege Level", "IOPL 	I/O Privilege Level", "Nested Task", "Reserved", "Resume Flag", "Virtual-8086 Mode", "Alignment Check / Access Control", "Virtual Interrupt Flag", "Virtual Interrupt Pending", "ID Flag "};
//    constexpr uint64_t entries = sizeof(names) / sizeof(*names);
//
//    kout << "Flags: ";
//    for(uint64_t i = 0; i < 64; ++i) {
//        if(flags & 0b1) {
//            if(i < entries) {
//                kout << i << ':' << names[i] << ' ';
//            } else {
//                kout << i << ' ';
//            }
//        }
//        flags >>= 0b1;
//    }
//    kout << '\n';
//    for(uint64_t i = 0; i < 32; ++i) {
//        kout << Hex(*(uint8_t*)(rip+i));
//    }
//    kout << '\n';
//    Serial s(0x3F8);
//    bool inSingleStep = stack[2] & 0x100;
//    kout << "'c' to continue";
//    if(inSingleStep) {
//        kout << " 'e' to exit single step mode";
//    } else {
//        kout << " 's' to enter single step mode";
//    }
//    kout << " 'd' to enter debug settings";
//    kout << " 't' to enter instruction trace mode";
//    kout << '\n';
//    while(true) {
//        while(s.inputBufferEmpty());
//        uint8_t ch = s.read();
//        if(ch == 'c' || ch == 'C') {
//            return;
//        } else if(ch == 'e' || ch == 'E') {
//            stack[2] &= ~0x100;
//            return;
//        } else if(ch == 's' || ch == 'S') {
//            stack[2] |= 0x100;
//            return;
//        } else if(ch == 'd' || ch == 'D') {
//            debug();
//        } else if(ch == 't' || ch == 'T') {
//            stack[2] |= 0x100;
//            for(uint8_t i = 0; i < 128; ++i) {
//                instructionTrace[i] = 0;
//            }
//            traceInstructions = true;
//            traceReadFrom0 = true;
//            return;
//        }
//    }
//    information.breakpoint0 = false;
//    information.breakpoint1 = false;
//    information.breakpoint2 = false;
//    information.breakpoint3 = false;
//    information.singleStep = false;
//}
//
}