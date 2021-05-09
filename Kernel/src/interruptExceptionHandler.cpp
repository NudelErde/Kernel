#include "interruptExceptionHandler.hpp"
#include "print.hpp"
#include "interrupt.hpp"

void generic_interrupt_handler(uint8_t id, void* stackFrame, uint64_t error = 0, bool hasError = false) {
    using namespace Kernel;
    Interrupt interrupt{};
    interrupt.interruptNumber = id;
    interrupt.errorCode = error;
    interrupt.hasErrorCode = hasError;
    if(id >= 240) {
        interrupt.isHardwareInterrupt = true;
    }
    interrupt.stackFrame = stackFrame;
    Kernel::Interrupt::Handler func = Interrupt::getHandler(interrupt.interruptNumber);
    if(func != nullptr) {
        func(interrupt);
    } else {
        Interrupt::getDefaultHandler()(interrupt);
    }
}

struct interrupt_frame;

__attribute__((interrupt)) void interrupt0x00(interrupt_frame* frame) { generic_interrupt_handler(0x00, frame); }
__attribute__((interrupt)) void interrupt0x01(interrupt_frame* frame) { generic_interrupt_handler(0x01, frame); }
__attribute__((interrupt)) void interrupt0x02(interrupt_frame* frame) { generic_interrupt_handler(0x02, frame); }
__attribute__((interrupt)) void interrupt0x03(interrupt_frame* frame) { generic_interrupt_handler(0x03, frame); }
__attribute__((interrupt)) void interrupt0x04(interrupt_frame* frame) { generic_interrupt_handler(0x04, frame); }
__attribute__((interrupt)) void interrupt0x05(interrupt_frame* frame) { generic_interrupt_handler(0x05, frame); }
__attribute__((interrupt)) void interrupt0x06(interrupt_frame* frame) { generic_interrupt_handler(0x06, frame); }
__attribute__((interrupt)) void interrupt0x07(interrupt_frame* frame) { generic_interrupt_handler(0x07, frame); }
__attribute__((interrupt)) void interrupt0x08(interrupt_frame* frame, uint64_t error) { generic_interrupt_handler(0x08, frame, error, true); }
__attribute__((interrupt)) void interrupt0x09(interrupt_frame* frame) { generic_interrupt_handler(0x09, frame); }
__attribute__((interrupt)) void interrupt0x0A(interrupt_frame* frame, uint64_t error) { generic_interrupt_handler(0x0A, frame, error, true); }
__attribute__((interrupt)) void interrupt0x0B(interrupt_frame* frame, uint64_t error) { generic_interrupt_handler(0x0B, frame, error, true); }
__attribute__((interrupt)) void interrupt0x0C(interrupt_frame* frame, uint64_t error) { generic_interrupt_handler(0x0C, frame, error, true); }
__attribute__((interrupt)) void interrupt0x0D(interrupt_frame* frame, uint64_t error) { generic_interrupt_handler(0x0D, frame, error, true); }
__attribute__((interrupt)) void interrupt0x0E(interrupt_frame* frame, uint64_t error) { generic_interrupt_handler(0x0E, frame, error, true); }
__attribute__((interrupt)) void interrupt0x0F(interrupt_frame* frame) { generic_interrupt_handler(0x0F, frame); }
__attribute__((interrupt)) void interrupt0x10(interrupt_frame* frame) { generic_interrupt_handler(0x10, frame); }
__attribute__((interrupt)) void interrupt0x11(interrupt_frame* frame, uint64_t error) { generic_interrupt_handler(0x11, frame, error, true); }
__attribute__((interrupt)) void interrupt0x12(interrupt_frame* frame) { generic_interrupt_handler(0x12, frame); }
__attribute__((interrupt)) void interrupt0x13(interrupt_frame* frame) { generic_interrupt_handler(0x13, frame); }
__attribute__((interrupt)) void interrupt0x14(interrupt_frame* frame) { generic_interrupt_handler(0x14, frame); }
__attribute__((interrupt)) void interrupt0x15(interrupt_frame* frame) { generic_interrupt_handler(0x15, frame); }
__attribute__((interrupt)) void interrupt0x16(interrupt_frame* frame) { generic_interrupt_handler(0x16, frame); }
__attribute__((interrupt)) void interrupt0x17(interrupt_frame* frame) { generic_interrupt_handler(0x17, frame); }
__attribute__((interrupt)) void interrupt0x18(interrupt_frame* frame) { generic_interrupt_handler(0x18, frame); }
__attribute__((interrupt)) void interrupt0x19(interrupt_frame* frame) { generic_interrupt_handler(0x19, frame); }
__attribute__((interrupt)) void interrupt0x1A(interrupt_frame* frame) { generic_interrupt_handler(0x1A, frame); }
__attribute__((interrupt)) void interrupt0x1B(interrupt_frame* frame) { generic_interrupt_handler(0x1B, frame); }
__attribute__((interrupt)) void interrupt0x1C(interrupt_frame* frame) { generic_interrupt_handler(0x1C, frame); }
__attribute__((interrupt)) void interrupt0x1D(interrupt_frame* frame) { generic_interrupt_handler(0x1D, frame); }
__attribute__((interrupt)) void interrupt0x1E(interrupt_frame* frame) { generic_interrupt_handler(0x1E, frame); }
__attribute__((interrupt)) void interrupt0x1F(interrupt_frame* frame) { generic_interrupt_handler(0x1F, frame); }
__attribute__((interrupt)) void interrupt0x20(interrupt_frame* frame) { generic_interrupt_handler(0x20, frame); }
__attribute__((interrupt)) void interrupt0x21(interrupt_frame* frame) { generic_interrupt_handler(0x21, frame); }
__attribute__((interrupt)) void interrupt0x22(interrupt_frame* frame) { generic_interrupt_handler(0x22, frame); }
__attribute__((interrupt)) void interrupt0x23(interrupt_frame* frame) { generic_interrupt_handler(0x23, frame); }
__attribute__((interrupt)) void interrupt0x24(interrupt_frame* frame) { generic_interrupt_handler(0x24, frame); }
__attribute__((interrupt)) void interrupt0x25(interrupt_frame* frame) { generic_interrupt_handler(0x25, frame); }
__attribute__((interrupt)) void interrupt0x26(interrupt_frame* frame) { generic_interrupt_handler(0x26, frame); }
__attribute__((interrupt)) void interrupt0x27(interrupt_frame* frame) { generic_interrupt_handler(0x27, frame); }
__attribute__((interrupt)) void interrupt0x28(interrupt_frame* frame) { generic_interrupt_handler(0x28, frame); }
__attribute__((interrupt)) void interrupt0x29(interrupt_frame* frame) { generic_interrupt_handler(0x29, frame); }
__attribute__((interrupt)) void interrupt0x2A(interrupt_frame* frame) { generic_interrupt_handler(0x2A, frame); }
__attribute__((interrupt)) void interrupt0x2B(interrupt_frame* frame) { generic_interrupt_handler(0x2B, frame); }
__attribute__((interrupt)) void interrupt0x2C(interrupt_frame* frame) { generic_interrupt_handler(0x2C, frame); }
__attribute__((interrupt)) void interrupt0x2D(interrupt_frame* frame) { generic_interrupt_handler(0x2D, frame); }
__attribute__((interrupt)) void interrupt0x2E(interrupt_frame* frame) { generic_interrupt_handler(0x2E, frame); }
__attribute__((interrupt)) void interrupt0x2F(interrupt_frame* frame) { generic_interrupt_handler(0x2F, frame); }
__attribute__((interrupt)) void interrupt0x30(interrupt_frame* frame) { generic_interrupt_handler(0x30, frame); }
__attribute__((interrupt)) void interrupt0x31(interrupt_frame* frame) { generic_interrupt_handler(0x31, frame); }
__attribute__((interrupt)) void interrupt0x32(interrupt_frame* frame) { generic_interrupt_handler(0x32, frame); }
__attribute__((interrupt)) void interrupt0x33(interrupt_frame* frame) { generic_interrupt_handler(0x33, frame); }
__attribute__((interrupt)) void interrupt0x34(interrupt_frame* frame) { generic_interrupt_handler(0x34, frame); }
__attribute__((interrupt)) void interrupt0x35(interrupt_frame* frame) { generic_interrupt_handler(0x35, frame); }
__attribute__((interrupt)) void interrupt0x36(interrupt_frame* frame) { generic_interrupt_handler(0x36, frame); }
__attribute__((interrupt)) void interrupt0x37(interrupt_frame* frame) { generic_interrupt_handler(0x37, frame); }
__attribute__((interrupt)) void interrupt0x38(interrupt_frame* frame) { generic_interrupt_handler(0x38, frame); }
__attribute__((interrupt)) void interrupt0x39(interrupt_frame* frame) { generic_interrupt_handler(0x39, frame); }
__attribute__((interrupt)) void interrupt0x3A(interrupt_frame* frame) { generic_interrupt_handler(0x3A, frame); }
__attribute__((interrupt)) void interrupt0x3B(interrupt_frame* frame) { generic_interrupt_handler(0x3B, frame); }
__attribute__((interrupt)) void interrupt0x3C(interrupt_frame* frame) { generic_interrupt_handler(0x3C, frame); }
__attribute__((interrupt)) void interrupt0x3D(interrupt_frame* frame) { generic_interrupt_handler(0x3D, frame); }
__attribute__((interrupt)) void interrupt0x3E(interrupt_frame* frame) { generic_interrupt_handler(0x3E, frame); }
__attribute__((interrupt)) void interrupt0x3F(interrupt_frame* frame) { generic_interrupt_handler(0x3F, frame); }
__attribute__((interrupt)) void interrupt0x40(interrupt_frame* frame) { generic_interrupt_handler(0x40, frame); }
__attribute__((interrupt)) void interrupt0x41(interrupt_frame* frame) { generic_interrupt_handler(0x41, frame); }
__attribute__((interrupt)) void interrupt0x42(interrupt_frame* frame) { generic_interrupt_handler(0x42, frame); }
__attribute__((interrupt)) void interrupt0x43(interrupt_frame* frame) { generic_interrupt_handler(0x43, frame); }
__attribute__((interrupt)) void interrupt0x44(interrupt_frame* frame) { generic_interrupt_handler(0x44, frame); }
__attribute__((interrupt)) void interrupt0x45(interrupt_frame* frame) { generic_interrupt_handler(0x45, frame); }
__attribute__((interrupt)) void interrupt0x46(interrupt_frame* frame) { generic_interrupt_handler(0x46, frame); }
__attribute__((interrupt)) void interrupt0x47(interrupt_frame* frame) { generic_interrupt_handler(0x47, frame); }
__attribute__((interrupt)) void interrupt0x48(interrupt_frame* frame) { generic_interrupt_handler(0x48, frame); }
__attribute__((interrupt)) void interrupt0x49(interrupt_frame* frame) { generic_interrupt_handler(0x49, frame); }
__attribute__((interrupt)) void interrupt0x4A(interrupt_frame* frame) { generic_interrupt_handler(0x4A, frame); }
__attribute__((interrupt)) void interrupt0x4B(interrupt_frame* frame) { generic_interrupt_handler(0x4B, frame); }
__attribute__((interrupt)) void interrupt0x4C(interrupt_frame* frame) { generic_interrupt_handler(0x4C, frame); }
__attribute__((interrupt)) void interrupt0x4D(interrupt_frame* frame) { generic_interrupt_handler(0x4D, frame); }
__attribute__((interrupt)) void interrupt0x4E(interrupt_frame* frame) { generic_interrupt_handler(0x4E, frame); }
__attribute__((interrupt)) void interrupt0x4F(interrupt_frame* frame) { generic_interrupt_handler(0x4F, frame); }
__attribute__((interrupt)) void interrupt0x50(interrupt_frame* frame) { generic_interrupt_handler(0x50, frame); }
__attribute__((interrupt)) void interrupt0x51(interrupt_frame* frame) { generic_interrupt_handler(0x51, frame); }
__attribute__((interrupt)) void interrupt0x52(interrupt_frame* frame) { generic_interrupt_handler(0x52, frame); }
__attribute__((interrupt)) void interrupt0x53(interrupt_frame* frame) { generic_interrupt_handler(0x53, frame); }
__attribute__((interrupt)) void interrupt0x54(interrupt_frame* frame) { generic_interrupt_handler(0x54, frame); }
__attribute__((interrupt)) void interrupt0x55(interrupt_frame* frame) { generic_interrupt_handler(0x55, frame); }
__attribute__((interrupt)) void interrupt0x56(interrupt_frame* frame) { generic_interrupt_handler(0x56, frame); }
__attribute__((interrupt)) void interrupt0x57(interrupt_frame* frame) { generic_interrupt_handler(0x57, frame); }
__attribute__((interrupt)) void interrupt0x58(interrupt_frame* frame) { generic_interrupt_handler(0x58, frame); }
__attribute__((interrupt)) void interrupt0x59(interrupt_frame* frame) { generic_interrupt_handler(0x59, frame); }
__attribute__((interrupt)) void interrupt0x5A(interrupt_frame* frame) { generic_interrupt_handler(0x5A, frame); }
__attribute__((interrupt)) void interrupt0x5B(interrupt_frame* frame) { generic_interrupt_handler(0x5B, frame); }
__attribute__((interrupt)) void interrupt0x5C(interrupt_frame* frame) { generic_interrupt_handler(0x5C, frame); }
__attribute__((interrupt)) void interrupt0x5D(interrupt_frame* frame) { generic_interrupt_handler(0x5D, frame); }
__attribute__((interrupt)) void interrupt0x5E(interrupt_frame* frame) { generic_interrupt_handler(0x5E, frame); }
__attribute__((interrupt)) void interrupt0x5F(interrupt_frame* frame) { generic_interrupt_handler(0x5F, frame); }
__attribute__((interrupt)) void interrupt0x60(interrupt_frame* frame) { generic_interrupt_handler(0x60, frame); }
__attribute__((interrupt)) void interrupt0x61(interrupt_frame* frame) { generic_interrupt_handler(0x61, frame); }
__attribute__((interrupt)) void interrupt0x62(interrupt_frame* frame) { generic_interrupt_handler(0x62, frame); }
__attribute__((interrupt)) void interrupt0x63(interrupt_frame* frame) { generic_interrupt_handler(0x63, frame); }
__attribute__((interrupt)) void interrupt0x64(interrupt_frame* frame) { generic_interrupt_handler(0x64, frame); }
__attribute__((interrupt)) void interrupt0x65(interrupt_frame* frame) { generic_interrupt_handler(0x65, frame); }
__attribute__((interrupt)) void interrupt0x66(interrupt_frame* frame) { generic_interrupt_handler(0x66, frame); }
__attribute__((interrupt)) void interrupt0x67(interrupt_frame* frame) { generic_interrupt_handler(0x67, frame); }
__attribute__((interrupt)) void interrupt0x68(interrupt_frame* frame) { generic_interrupt_handler(0x68, frame); }
__attribute__((interrupt)) void interrupt0x69(interrupt_frame* frame) { generic_interrupt_handler(0x69, frame); }
__attribute__((interrupt)) void interrupt0x6A(interrupt_frame* frame) { generic_interrupt_handler(0x6A, frame); }
__attribute__((interrupt)) void interrupt0x6B(interrupt_frame* frame) { generic_interrupt_handler(0x6B, frame); }
__attribute__((interrupt)) void interrupt0x6C(interrupt_frame* frame) { generic_interrupt_handler(0x6C, frame); }
__attribute__((interrupt)) void interrupt0x6D(interrupt_frame* frame) { generic_interrupt_handler(0x6D, frame); }
__attribute__((interrupt)) void interrupt0x6E(interrupt_frame* frame) { generic_interrupt_handler(0x6E, frame); }
__attribute__((interrupt)) void interrupt0x6F(interrupt_frame* frame) { generic_interrupt_handler(0x6F, frame); }
__attribute__((interrupt)) void interrupt0x70(interrupt_frame* frame) { generic_interrupt_handler(0x70, frame); }
__attribute__((interrupt)) void interrupt0x71(interrupt_frame* frame) { generic_interrupt_handler(0x71, frame); }
__attribute__((interrupt)) void interrupt0x72(interrupt_frame* frame) { generic_interrupt_handler(0x72, frame); }
__attribute__((interrupt)) void interrupt0x73(interrupt_frame* frame) { generic_interrupt_handler(0x73, frame); }
__attribute__((interrupt)) void interrupt0x74(interrupt_frame* frame) { generic_interrupt_handler(0x74, frame); }
__attribute__((interrupt)) void interrupt0x75(interrupt_frame* frame) { generic_interrupt_handler(0x75, frame); }
__attribute__((interrupt)) void interrupt0x76(interrupt_frame* frame) { generic_interrupt_handler(0x76, frame); }
__attribute__((interrupt)) void interrupt0x77(interrupt_frame* frame) { generic_interrupt_handler(0x77, frame); }
__attribute__((interrupt)) void interrupt0x78(interrupt_frame* frame) { generic_interrupt_handler(0x78, frame); }
__attribute__((interrupt)) void interrupt0x79(interrupt_frame* frame) { generic_interrupt_handler(0x79, frame); }
__attribute__((interrupt)) void interrupt0x7A(interrupt_frame* frame) { generic_interrupt_handler(0x7A, frame); }
__attribute__((interrupt)) void interrupt0x7B(interrupt_frame* frame) { generic_interrupt_handler(0x7B, frame); }
__attribute__((interrupt)) void interrupt0x7C(interrupt_frame* frame) { generic_interrupt_handler(0x7C, frame); }
__attribute__((interrupt)) void interrupt0x7D(interrupt_frame* frame) { generic_interrupt_handler(0x7D, frame); }
__attribute__((interrupt)) void interrupt0x7E(interrupt_frame* frame) { generic_interrupt_handler(0x7E, frame); }
__attribute__((interrupt)) void interrupt0x7F(interrupt_frame* frame) { generic_interrupt_handler(0x7F, frame); }
__attribute__((interrupt)) void interrupt0x80(interrupt_frame* frame) { generic_interrupt_handler(0x80, frame); }
__attribute__((interrupt)) void interrupt0x81(interrupt_frame* frame) { generic_interrupt_handler(0x81, frame); }
__attribute__((interrupt)) void interrupt0x82(interrupt_frame* frame) { generic_interrupt_handler(0x82, frame); }
__attribute__((interrupt)) void interrupt0x83(interrupt_frame* frame) { generic_interrupt_handler(0x83, frame); }
__attribute__((interrupt)) void interrupt0x84(interrupt_frame* frame) { generic_interrupt_handler(0x84, frame); }
__attribute__((interrupt)) void interrupt0x85(interrupt_frame* frame) { generic_interrupt_handler(0x85, frame); }
__attribute__((interrupt)) void interrupt0x86(interrupt_frame* frame) { generic_interrupt_handler(0x86, frame); }
__attribute__((interrupt)) void interrupt0x87(interrupt_frame* frame) { generic_interrupt_handler(0x87, frame); }
__attribute__((interrupt)) void interrupt0x88(interrupt_frame* frame) { generic_interrupt_handler(0x88, frame); }
__attribute__((interrupt)) void interrupt0x89(interrupt_frame* frame) { generic_interrupt_handler(0x89, frame); }
__attribute__((interrupt)) void interrupt0x8A(interrupt_frame* frame) { generic_interrupt_handler(0x8A, frame); }
__attribute__((interrupt)) void interrupt0x8B(interrupt_frame* frame) { generic_interrupt_handler(0x8B, frame); }
__attribute__((interrupt)) void interrupt0x8C(interrupt_frame* frame) { generic_interrupt_handler(0x8C, frame); }
__attribute__((interrupt)) void interrupt0x8D(interrupt_frame* frame) { generic_interrupt_handler(0x8D, frame); }
__attribute__((interrupt)) void interrupt0x8E(interrupt_frame* frame) { generic_interrupt_handler(0x8E, frame); }
__attribute__((interrupt)) void interrupt0x8F(interrupt_frame* frame) { generic_interrupt_handler(0x8F, frame); }
__attribute__((interrupt)) void interrupt0x90(interrupt_frame* frame) { generic_interrupt_handler(0x90, frame); }
__attribute__((interrupt)) void interrupt0x91(interrupt_frame* frame) { generic_interrupt_handler(0x91, frame); }
__attribute__((interrupt)) void interrupt0x92(interrupt_frame* frame) { generic_interrupt_handler(0x92, frame); }
__attribute__((interrupt)) void interrupt0x93(interrupt_frame* frame) { generic_interrupt_handler(0x93, frame); }
__attribute__((interrupt)) void interrupt0x94(interrupt_frame* frame) { generic_interrupt_handler(0x94, frame); }
__attribute__((interrupt)) void interrupt0x95(interrupt_frame* frame) { generic_interrupt_handler(0x95, frame); }
__attribute__((interrupt)) void interrupt0x96(interrupt_frame* frame) { generic_interrupt_handler(0x96, frame); }
__attribute__((interrupt)) void interrupt0x97(interrupt_frame* frame) { generic_interrupt_handler(0x97, frame); }
__attribute__((interrupt)) void interrupt0x98(interrupt_frame* frame) { generic_interrupt_handler(0x98, frame); }
__attribute__((interrupt)) void interrupt0x99(interrupt_frame* frame) { generic_interrupt_handler(0x99, frame); }
__attribute__((interrupt)) void interrupt0x9A(interrupt_frame* frame) { generic_interrupt_handler(0x9A, frame); }
__attribute__((interrupt)) void interrupt0x9B(interrupt_frame* frame) { generic_interrupt_handler(0x9B, frame); }
__attribute__((interrupt)) void interrupt0x9C(interrupt_frame* frame) { generic_interrupt_handler(0x9C, frame); }
__attribute__((interrupt)) void interrupt0x9D(interrupt_frame* frame) { generic_interrupt_handler(0x9D, frame); }
__attribute__((interrupt)) void interrupt0x9E(interrupt_frame* frame) { generic_interrupt_handler(0x9E, frame); }
__attribute__((interrupt)) void interrupt0x9F(interrupt_frame* frame) { generic_interrupt_handler(0x9F, frame); }
__attribute__((interrupt)) void interrupt0xA0(interrupt_frame* frame) { generic_interrupt_handler(0xA0, frame); }
__attribute__((interrupt)) void interrupt0xA1(interrupt_frame* frame) { generic_interrupt_handler(0xA1, frame); }
__attribute__((interrupt)) void interrupt0xA2(interrupt_frame* frame) { generic_interrupt_handler(0xA2, frame); }
__attribute__((interrupt)) void interrupt0xA3(interrupt_frame* frame) { generic_interrupt_handler(0xA3, frame); }
__attribute__((interrupt)) void interrupt0xA4(interrupt_frame* frame) { generic_interrupt_handler(0xA4, frame); }
__attribute__((interrupt)) void interrupt0xA5(interrupt_frame* frame) { generic_interrupt_handler(0xA5, frame); }
__attribute__((interrupt)) void interrupt0xA6(interrupt_frame* frame) { generic_interrupt_handler(0xA6, frame); }
__attribute__((interrupt)) void interrupt0xA7(interrupt_frame* frame) { generic_interrupt_handler(0xA7, frame); }
__attribute__((interrupt)) void interrupt0xA8(interrupt_frame* frame) { generic_interrupt_handler(0xA8, frame); }
__attribute__((interrupt)) void interrupt0xA9(interrupt_frame* frame) { generic_interrupt_handler(0xA9, frame); }
__attribute__((interrupt)) void interrupt0xAA(interrupt_frame* frame) { generic_interrupt_handler(0xAA, frame); }
__attribute__((interrupt)) void interrupt0xAB(interrupt_frame* frame) { generic_interrupt_handler(0xAB, frame); }
__attribute__((interrupt)) void interrupt0xAC(interrupt_frame* frame) { generic_interrupt_handler(0xAC, frame); }
__attribute__((interrupt)) void interrupt0xAD(interrupt_frame* frame) { generic_interrupt_handler(0xAD, frame); }
__attribute__((interrupt)) void interrupt0xAE(interrupt_frame* frame) { generic_interrupt_handler(0xAE, frame); }
__attribute__((interrupt)) void interrupt0xAF(interrupt_frame* frame) { generic_interrupt_handler(0xAF, frame); }
__attribute__((interrupt)) void interrupt0xB0(interrupt_frame* frame) { generic_interrupt_handler(0xB0, frame); }
__attribute__((interrupt)) void interrupt0xB1(interrupt_frame* frame) { generic_interrupt_handler(0xB1, frame); }
__attribute__((interrupt)) void interrupt0xB2(interrupt_frame* frame) { generic_interrupt_handler(0xB2, frame); }
__attribute__((interrupt)) void interrupt0xB3(interrupt_frame* frame) { generic_interrupt_handler(0xB3, frame); }
__attribute__((interrupt)) void interrupt0xB4(interrupt_frame* frame) { generic_interrupt_handler(0xB4, frame); }
__attribute__((interrupt)) void interrupt0xB5(interrupt_frame* frame) { generic_interrupt_handler(0xB5, frame); }
__attribute__((interrupt)) void interrupt0xB6(interrupt_frame* frame) { generic_interrupt_handler(0xB6, frame); }
__attribute__((interrupt)) void interrupt0xB7(interrupt_frame* frame) { generic_interrupt_handler(0xB7, frame); }
__attribute__((interrupt)) void interrupt0xB8(interrupt_frame* frame) { generic_interrupt_handler(0xB8, frame); }
__attribute__((interrupt)) void interrupt0xB9(interrupt_frame* frame) { generic_interrupt_handler(0xB9, frame); }
__attribute__((interrupt)) void interrupt0xBA(interrupt_frame* frame) { generic_interrupt_handler(0xBA, frame); }
__attribute__((interrupt)) void interrupt0xBB(interrupt_frame* frame) { generic_interrupt_handler(0xBB, frame); }
__attribute__((interrupt)) void interrupt0xBC(interrupt_frame* frame) { generic_interrupt_handler(0xBC, frame); }
__attribute__((interrupt)) void interrupt0xBD(interrupt_frame* frame) { generic_interrupt_handler(0xBD, frame); }
__attribute__((interrupt)) void interrupt0xBE(interrupt_frame* frame) { generic_interrupt_handler(0xBE, frame); }
__attribute__((interrupt)) void interrupt0xBF(interrupt_frame* frame) { generic_interrupt_handler(0xBF, frame); }
__attribute__((interrupt)) void interrupt0xC0(interrupt_frame* frame) { generic_interrupt_handler(0xC0, frame); }
__attribute__((interrupt)) void interrupt0xC1(interrupt_frame* frame) { generic_interrupt_handler(0xC1, frame); }
__attribute__((interrupt)) void interrupt0xC2(interrupt_frame* frame) { generic_interrupt_handler(0xC2, frame); }
__attribute__((interrupt)) void interrupt0xC3(interrupt_frame* frame) { generic_interrupt_handler(0xC3, frame); }
__attribute__((interrupt)) void interrupt0xC4(interrupt_frame* frame) { generic_interrupt_handler(0xC4, frame); }
__attribute__((interrupt)) void interrupt0xC5(interrupt_frame* frame) { generic_interrupt_handler(0xC5, frame); }
__attribute__((interrupt)) void interrupt0xC6(interrupt_frame* frame) { generic_interrupt_handler(0xC6, frame); }
__attribute__((interrupt)) void interrupt0xC7(interrupt_frame* frame) { generic_interrupt_handler(0xC7, frame); }
__attribute__((interrupt)) void interrupt0xC8(interrupt_frame* frame) { generic_interrupt_handler(0xC8, frame); }
__attribute__((interrupt)) void interrupt0xC9(interrupt_frame* frame) { generic_interrupt_handler(0xC9, frame); }
__attribute__((interrupt)) void interrupt0xCA(interrupt_frame* frame) { generic_interrupt_handler(0xCA, frame); }
__attribute__((interrupt)) void interrupt0xCB(interrupt_frame* frame) { generic_interrupt_handler(0xCB, frame); }
__attribute__((interrupt)) void interrupt0xCC(interrupt_frame* frame) { generic_interrupt_handler(0xCC, frame); }
__attribute__((interrupt)) void interrupt0xCD(interrupt_frame* frame) { generic_interrupt_handler(0xCD, frame); }
__attribute__((interrupt)) void interrupt0xCE(interrupt_frame* frame) { generic_interrupt_handler(0xCE, frame); }
__attribute__((interrupt)) void interrupt0xCF(interrupt_frame* frame) { generic_interrupt_handler(0xCF, frame); }
__attribute__((interrupt)) void interrupt0xD0(interrupt_frame* frame) { generic_interrupt_handler(0xD0, frame); }
__attribute__((interrupt)) void interrupt0xD1(interrupt_frame* frame) { generic_interrupt_handler(0xD1, frame); }
__attribute__((interrupt)) void interrupt0xD2(interrupt_frame* frame) { generic_interrupt_handler(0xD2, frame); }
__attribute__((interrupt)) void interrupt0xD3(interrupt_frame* frame) { generic_interrupt_handler(0xD3, frame); }
__attribute__((interrupt)) void interrupt0xD4(interrupt_frame* frame) { generic_interrupt_handler(0xD4, frame); }
__attribute__((interrupt)) void interrupt0xD5(interrupt_frame* frame) { generic_interrupt_handler(0xD5, frame); }
__attribute__((interrupt)) void interrupt0xD6(interrupt_frame* frame) { generic_interrupt_handler(0xD6, frame); }
__attribute__((interrupt)) void interrupt0xD7(interrupt_frame* frame) { generic_interrupt_handler(0xD7, frame); }
__attribute__((interrupt)) void interrupt0xD8(interrupt_frame* frame) { generic_interrupt_handler(0xD8, frame); }
__attribute__((interrupt)) void interrupt0xD9(interrupt_frame* frame) { generic_interrupt_handler(0xD9, frame); }
__attribute__((interrupt)) void interrupt0xDA(interrupt_frame* frame) { generic_interrupt_handler(0xDA, frame); }
__attribute__((interrupt)) void interrupt0xDB(interrupt_frame* frame) { generic_interrupt_handler(0xDB, frame); }
__attribute__((interrupt)) void interrupt0xDC(interrupt_frame* frame) { generic_interrupt_handler(0xDC, frame); }
__attribute__((interrupt)) void interrupt0xDD(interrupt_frame* frame) { generic_interrupt_handler(0xDD, frame); }
__attribute__((interrupt)) void interrupt0xDE(interrupt_frame* frame) { generic_interrupt_handler(0xDE, frame); }
__attribute__((interrupt)) void interrupt0xDF(interrupt_frame* frame) { generic_interrupt_handler(0xDF, frame); }
__attribute__((interrupt)) void interrupt0xE0(interrupt_frame* frame) { generic_interrupt_handler(0xE0, frame); }
__attribute__((interrupt)) void interrupt0xE1(interrupt_frame* frame) { generic_interrupt_handler(0xE1, frame); }
__attribute__((interrupt)) void interrupt0xE2(interrupt_frame* frame) { generic_interrupt_handler(0xE2, frame); }
__attribute__((interrupt)) void interrupt0xE3(interrupt_frame* frame) { generic_interrupt_handler(0xE3, frame); }
__attribute__((interrupt)) void interrupt0xE4(interrupt_frame* frame) { generic_interrupt_handler(0xE4, frame); }
__attribute__((interrupt)) void interrupt0xE5(interrupt_frame* frame) { generic_interrupt_handler(0xE5, frame); }
__attribute__((interrupt)) void interrupt0xE6(interrupt_frame* frame) { generic_interrupt_handler(0xE6, frame); }
__attribute__((interrupt)) void interrupt0xE7(interrupt_frame* frame) { generic_interrupt_handler(0xE7, frame); }
__attribute__((interrupt)) void interrupt0xE8(interrupt_frame* frame) { generic_interrupt_handler(0xE8, frame); }
__attribute__((interrupt)) void interrupt0xE9(interrupt_frame* frame) { generic_interrupt_handler(0xE9, frame); }
__attribute__((interrupt)) void interrupt0xEA(interrupt_frame* frame) { generic_interrupt_handler(0xEA, frame); }
__attribute__((interrupt)) void interrupt0xEB(interrupt_frame* frame) { generic_interrupt_handler(0xEB, frame); }
__attribute__((interrupt)) void interrupt0xEC(interrupt_frame* frame) { generic_interrupt_handler(0xEC, frame); }
__attribute__((interrupt)) void interrupt0xED(interrupt_frame* frame) { generic_interrupt_handler(0xED, frame); }
__attribute__((interrupt)) void interrupt0xEE(interrupt_frame* frame) { generic_interrupt_handler(0xEE, frame); }
__attribute__((interrupt)) void interrupt0xEF(interrupt_frame* frame) { generic_interrupt_handler(0xEF, frame); }
__attribute__((interrupt)) void interrupt0xF0(interrupt_frame* frame) { generic_interrupt_handler(0xF0, frame); }
__attribute__((interrupt)) void interrupt0xF1(interrupt_frame* frame) { generic_interrupt_handler(0xF1, frame); }
__attribute__((interrupt)) void interrupt0xF2(interrupt_frame* frame) { generic_interrupt_handler(0xF2, frame); }
__attribute__((interrupt)) void interrupt0xF3(interrupt_frame* frame) { generic_interrupt_handler(0xF3, frame); }
__attribute__((interrupt)) void interrupt0xF4(interrupt_frame* frame) { generic_interrupt_handler(0xF4, frame); }
__attribute__((interrupt)) void interrupt0xF5(interrupt_frame* frame) { generic_interrupt_handler(0xF5, frame); }
__attribute__((interrupt)) void interrupt0xF6(interrupt_frame* frame) { generic_interrupt_handler(0xF6, frame); }
__attribute__((interrupt)) void interrupt0xF7(interrupt_frame* frame) { generic_interrupt_handler(0xF7, frame); }
__attribute__((interrupt)) void interrupt0xF8(interrupt_frame* frame) { generic_interrupt_handler(0xF8, frame); }
__attribute__((interrupt)) void interrupt0xF9(interrupt_frame* frame) { generic_interrupt_handler(0xF9, frame); }
__attribute__((interrupt)) void interrupt0xFA(interrupt_frame* frame) { generic_interrupt_handler(0xFA, frame); }
__attribute__((interrupt)) void interrupt0xFB(interrupt_frame* frame) { generic_interrupt_handler(0xFB, frame); }
__attribute__((interrupt)) void interrupt0xFC(interrupt_frame* frame) { generic_interrupt_handler(0xFC, frame); }
__attribute__((interrupt)) void interrupt0xFD(interrupt_frame* frame) { generic_interrupt_handler(0xFD, frame); }
__attribute__((interrupt)) void interrupt0xFE(interrupt_frame* frame) { generic_interrupt_handler(0xFE, frame); }
__attribute__((interrupt)) void interrupt0xFF(interrupt_frame* frame) { generic_interrupt_handler(0xFF, frame); }

namespace Kernel {

void initExceptionHandlers() {
    Interrupt::setInterrupt(0x00, (void(*)())&interrupt0x00, 0);
    Interrupt::setInterrupt(0x01, (void(*)())&interrupt0x01, 0);
    Interrupt::setInterrupt(0x02, (void(*)())&interrupt0x02, 0);
    Interrupt::setInterrupt(0x03, (void(*)())&interrupt0x03, 0);
    Interrupt::setInterrupt(0x04, (void(*)())&interrupt0x04, 0);
    Interrupt::setInterrupt(0x05, (void(*)())&interrupt0x05, 0);
    Interrupt::setInterrupt(0x06, (void(*)())&interrupt0x06, 0);
    Interrupt::setInterrupt(0x07, (void(*)())&interrupt0x07, 0);
    Interrupt::setInterrupt(0x08, (void(*)())&interrupt0x08, 0);
    Interrupt::setInterrupt(0x09, (void(*)())&interrupt0x09, 0);
    Interrupt::setInterrupt(0x0A, (void(*)())&interrupt0x0A, 0);
    Interrupt::setInterrupt(0x0B, (void(*)())&interrupt0x0B, 0);
    Interrupt::setInterrupt(0x0C, (void(*)())&interrupt0x0C, 0);
    Interrupt::setInterrupt(0x0D, (void(*)())&interrupt0x0D, 0);
    Interrupt::setInterrupt(0x0E, (void(*)())&interrupt0x0E, 0);
    Interrupt::setInterrupt(0x0F, (void(*)())&interrupt0x0F, 0);
    Interrupt::setInterrupt(0x10, (void(*)())&interrupt0x10, 0);
    Interrupt::setInterrupt(0x11, (void(*)())&interrupt0x11, 0);
    Interrupt::setInterrupt(0x12, (void(*)())&interrupt0x12, 0);
    Interrupt::setInterrupt(0x13, (void(*)())&interrupt0x13, 0);
    Interrupt::setInterrupt(0x14, (void(*)())&interrupt0x14, 0);
    Interrupt::setInterrupt(0x15, (void(*)())&interrupt0x15, 0);
    Interrupt::setInterrupt(0x16, (void(*)())&interrupt0x16, 0);
    Interrupt::setInterrupt(0x17, (void(*)())&interrupt0x17, 0);
    Interrupt::setInterrupt(0x18, (void(*)())&interrupt0x18, 0);
    Interrupt::setInterrupt(0x19, (void(*)())&interrupt0x19, 0);
    Interrupt::setInterrupt(0x1A, (void(*)())&interrupt0x1A, 0);
    Interrupt::setInterrupt(0x1B, (void(*)())&interrupt0x1B, 0);
    Interrupt::setInterrupt(0x1C, (void(*)())&interrupt0x1C, 0);
    Interrupt::setInterrupt(0x1D, (void(*)())&interrupt0x1D, 0);
    Interrupt::setInterrupt(0x1E, (void(*)())&interrupt0x1E, 0);
    Interrupt::setInterrupt(0x1F, (void(*)())&interrupt0x1F, 0);
    Interrupt::setInterrupt(0x20, (void(*)())&interrupt0x20, 0);
    Interrupt::setInterrupt(0x21, (void(*)())&interrupt0x21, 0);
    Interrupt::setInterrupt(0x22, (void(*)())&interrupt0x22, 0);
    Interrupt::setInterrupt(0x23, (void(*)())&interrupt0x23, 0);
    Interrupt::setInterrupt(0x24, (void(*)())&interrupt0x24, 0);
    Interrupt::setInterrupt(0x25, (void(*)())&interrupt0x25, 0);
    Interrupt::setInterrupt(0x26, (void(*)())&interrupt0x26, 0);
    Interrupt::setInterrupt(0x27, (void(*)())&interrupt0x27, 0);
    Interrupt::setInterrupt(0x28, (void(*)())&interrupt0x28, 0);
    Interrupt::setInterrupt(0x29, (void(*)())&interrupt0x29, 0);
    Interrupt::setInterrupt(0x2A, (void(*)())&interrupt0x2A, 0);
    Interrupt::setInterrupt(0x2B, (void(*)())&interrupt0x2B, 0);
    Interrupt::setInterrupt(0x2C, (void(*)())&interrupt0x2C, 0);
    Interrupt::setInterrupt(0x2D, (void(*)())&interrupt0x2D, 0);
    Interrupt::setInterrupt(0x2E, (void(*)())&interrupt0x2E, 0);
    Interrupt::setInterrupt(0x2F, (void(*)())&interrupt0x2F, 0);
    Interrupt::setInterrupt(0x30, (void(*)())&interrupt0x30, 0);
    Interrupt::setInterrupt(0x31, (void(*)())&interrupt0x31, 0);
    Interrupt::setInterrupt(0x32, (void(*)())&interrupt0x32, 0);
    Interrupt::setInterrupt(0x33, (void(*)())&interrupt0x33, 0);
    Interrupt::setInterrupt(0x34, (void(*)())&interrupt0x34, 0);
    Interrupt::setInterrupt(0x35, (void(*)())&interrupt0x35, 0);
    Interrupt::setInterrupt(0x36, (void(*)())&interrupt0x36, 0);
    Interrupt::setInterrupt(0x37, (void(*)())&interrupt0x37, 0);
    Interrupt::setInterrupt(0x38, (void(*)())&interrupt0x38, 0);
    Interrupt::setInterrupt(0x39, (void(*)())&interrupt0x39, 0);
    Interrupt::setInterrupt(0x3A, (void(*)())&interrupt0x3A, 0);
    Interrupt::setInterrupt(0x3B, (void(*)())&interrupt0x3B, 0);
    Interrupt::setInterrupt(0x3C, (void(*)())&interrupt0x3C, 0);
    Interrupt::setInterrupt(0x3D, (void(*)())&interrupt0x3D, 0);
    Interrupt::setInterrupt(0x3E, (void(*)())&interrupt0x3E, 0);
    Interrupt::setInterrupt(0x3F, (void(*)())&interrupt0x3F, 0);
    Interrupt::setInterrupt(0x40, (void(*)())&interrupt0x40, 0);
    Interrupt::setInterrupt(0x41, (void(*)())&interrupt0x41, 0);
    Interrupt::setInterrupt(0x42, (void(*)())&interrupt0x42, 0);
    Interrupt::setInterrupt(0x43, (void(*)())&interrupt0x43, 0);
    Interrupt::setInterrupt(0x44, (void(*)())&interrupt0x44, 0);
    Interrupt::setInterrupt(0x45, (void(*)())&interrupt0x45, 0);
    Interrupt::setInterrupt(0x46, (void(*)())&interrupt0x46, 0);
    Interrupt::setInterrupt(0x47, (void(*)())&interrupt0x47, 0);
    Interrupt::setInterrupt(0x48, (void(*)())&interrupt0x48, 0);
    Interrupt::setInterrupt(0x49, (void(*)())&interrupt0x49, 0);
    Interrupt::setInterrupt(0x4A, (void(*)())&interrupt0x4A, 0);
    Interrupt::setInterrupt(0x4B, (void(*)())&interrupt0x4B, 0);
    Interrupt::setInterrupt(0x4C, (void(*)())&interrupt0x4C, 0);
    Interrupt::setInterrupt(0x4D, (void(*)())&interrupt0x4D, 0);
    Interrupt::setInterrupt(0x4E, (void(*)())&interrupt0x4E, 0);
    Interrupt::setInterrupt(0x4F, (void(*)())&interrupt0x4F, 0);
    Interrupt::setInterrupt(0x50, (void(*)())&interrupt0x50, 0);
    Interrupt::setInterrupt(0x51, (void(*)())&interrupt0x51, 0);
    Interrupt::setInterrupt(0x52, (void(*)())&interrupt0x52, 0);
    Interrupt::setInterrupt(0x53, (void(*)())&interrupt0x53, 0);
    Interrupt::setInterrupt(0x54, (void(*)())&interrupt0x54, 0);
    Interrupt::setInterrupt(0x55, (void(*)())&interrupt0x55, 0);
    Interrupt::setInterrupt(0x56, (void(*)())&interrupt0x56, 0);
    Interrupt::setInterrupt(0x57, (void(*)())&interrupt0x57, 0);
    Interrupt::setInterrupt(0x58, (void(*)())&interrupt0x58, 0);
    Interrupt::setInterrupt(0x59, (void(*)())&interrupt0x59, 0);
    Interrupt::setInterrupt(0x5A, (void(*)())&interrupt0x5A, 0);
    Interrupt::setInterrupt(0x5B, (void(*)())&interrupt0x5B, 0);
    Interrupt::setInterrupt(0x5C, (void(*)())&interrupt0x5C, 0);
    Interrupt::setInterrupt(0x5D, (void(*)())&interrupt0x5D, 0);
    Interrupt::setInterrupt(0x5E, (void(*)())&interrupt0x5E, 0);
    Interrupt::setInterrupt(0x5F, (void(*)())&interrupt0x5F, 0);
    Interrupt::setInterrupt(0x60, (void(*)())&interrupt0x60, 0);
    Interrupt::setInterrupt(0x61, (void(*)())&interrupt0x61, 0);
    Interrupt::setInterrupt(0x62, (void(*)())&interrupt0x62, 0);
    Interrupt::setInterrupt(0x63, (void(*)())&interrupt0x63, 0);
    Interrupt::setInterrupt(0x64, (void(*)())&interrupt0x64, 0);
    Interrupt::setInterrupt(0x65, (void(*)())&interrupt0x65, 0);
    Interrupt::setInterrupt(0x66, (void(*)())&interrupt0x66, 0);
    Interrupt::setInterrupt(0x67, (void(*)())&interrupt0x67, 0);
    Interrupt::setInterrupt(0x68, (void(*)())&interrupt0x68, 0);
    Interrupt::setInterrupt(0x69, (void(*)())&interrupt0x69, 0);
    Interrupt::setInterrupt(0x6A, (void(*)())&interrupt0x6A, 0);
    Interrupt::setInterrupt(0x6B, (void(*)())&interrupt0x6B, 0);
    Interrupt::setInterrupt(0x6C, (void(*)())&interrupt0x6C, 0);
    Interrupt::setInterrupt(0x6D, (void(*)())&interrupt0x6D, 0);
    Interrupt::setInterrupt(0x6E, (void(*)())&interrupt0x6E, 0);
    Interrupt::setInterrupt(0x6F, (void(*)())&interrupt0x6F, 0);
    Interrupt::setInterrupt(0x70, (void(*)())&interrupt0x70, 0);
    Interrupt::setInterrupt(0x71, (void(*)())&interrupt0x71, 0);
    Interrupt::setInterrupt(0x72, (void(*)())&interrupt0x72, 0);
    Interrupt::setInterrupt(0x73, (void(*)())&interrupt0x73, 0);
    Interrupt::setInterrupt(0x74, (void(*)())&interrupt0x74, 0);
    Interrupt::setInterrupt(0x75, (void(*)())&interrupt0x75, 0);
    Interrupt::setInterrupt(0x76, (void(*)())&interrupt0x76, 0);
    Interrupt::setInterrupt(0x77, (void(*)())&interrupt0x77, 0);
    Interrupt::setInterrupt(0x78, (void(*)())&interrupt0x78, 0);
    Interrupt::setInterrupt(0x79, (void(*)())&interrupt0x79, 0);
    Interrupt::setInterrupt(0x7A, (void(*)())&interrupt0x7A, 0);
    Interrupt::setInterrupt(0x7B, (void(*)())&interrupt0x7B, 0);
    Interrupt::setInterrupt(0x7C, (void(*)())&interrupt0x7C, 0);
    Interrupt::setInterrupt(0x7D, (void(*)())&interrupt0x7D, 0);
    Interrupt::setInterrupt(0x7E, (void(*)())&interrupt0x7E, 0);
    Interrupt::setInterrupt(0x7F, (void(*)())&interrupt0x7F, 0);
    Interrupt::setInterrupt(0x80, (void(*)())&interrupt0x80, 0);
    Interrupt::setInterrupt(0x81, (void(*)())&interrupt0x81, 0);
    Interrupt::setInterrupt(0x82, (void(*)())&interrupt0x82, 0);
    Interrupt::setInterrupt(0x83, (void(*)())&interrupt0x83, 0);
    Interrupt::setInterrupt(0x84, (void(*)())&interrupt0x84, 0);
    Interrupt::setInterrupt(0x85, (void(*)())&interrupt0x85, 0);
    Interrupt::setInterrupt(0x86, (void(*)())&interrupt0x86, 0);
    Interrupt::setInterrupt(0x87, (void(*)())&interrupt0x87, 0);
    Interrupt::setInterrupt(0x88, (void(*)())&interrupt0x88, 0);
    Interrupt::setInterrupt(0x89, (void(*)())&interrupt0x89, 0);
    Interrupt::setInterrupt(0x8A, (void(*)())&interrupt0x8A, 0);
    Interrupt::setInterrupt(0x8B, (void(*)())&interrupt0x8B, 0);
    Interrupt::setInterrupt(0x8C, (void(*)())&interrupt0x8C, 0);
    Interrupt::setInterrupt(0x8D, (void(*)())&interrupt0x8D, 0);
    Interrupt::setInterrupt(0x8E, (void(*)())&interrupt0x8E, 0);
    Interrupt::setInterrupt(0x8F, (void(*)())&interrupt0x8F, 0);
    Interrupt::setInterrupt(0x90, (void(*)())&interrupt0x90, 0);
    Interrupt::setInterrupt(0x91, (void(*)())&interrupt0x91, 0);
    Interrupt::setInterrupt(0x92, (void(*)())&interrupt0x92, 0);
    Interrupt::setInterrupt(0x93, (void(*)())&interrupt0x93, 0);
    Interrupt::setInterrupt(0x94, (void(*)())&interrupt0x94, 0);
    Interrupt::setInterrupt(0x95, (void(*)())&interrupt0x95, 0);
    Interrupt::setInterrupt(0x96, (void(*)())&interrupt0x96, 0);
    Interrupt::setInterrupt(0x97, (void(*)())&interrupt0x97, 0);
    Interrupt::setInterrupt(0x98, (void(*)())&interrupt0x98, 0);
    Interrupt::setInterrupt(0x99, (void(*)())&interrupt0x99, 0);
    Interrupt::setInterrupt(0x9A, (void(*)())&interrupt0x9A, 0);
    Interrupt::setInterrupt(0x9B, (void(*)())&interrupt0x9B, 0);
    Interrupt::setInterrupt(0x9C, (void(*)())&interrupt0x9C, 0);
    Interrupt::setInterrupt(0x9D, (void(*)())&interrupt0x9D, 0);
    Interrupt::setInterrupt(0x9E, (void(*)())&interrupt0x9E, 0);
    Interrupt::setInterrupt(0x9F, (void(*)())&interrupt0x9F, 0);
    Interrupt::setInterrupt(0xA0, (void(*)())&interrupt0xA0, 0);
    Interrupt::setInterrupt(0xA1, (void(*)())&interrupt0xA1, 0);
    Interrupt::setInterrupt(0xA2, (void(*)())&interrupt0xA2, 0);
    Interrupt::setInterrupt(0xA3, (void(*)())&interrupt0xA3, 0);
    Interrupt::setInterrupt(0xA4, (void(*)())&interrupt0xA4, 0);
    Interrupt::setInterrupt(0xA5, (void(*)())&interrupt0xA5, 0);
    Interrupt::setInterrupt(0xA6, (void(*)())&interrupt0xA6, 0);
    Interrupt::setInterrupt(0xA7, (void(*)())&interrupt0xA7, 0);
    Interrupt::setInterrupt(0xA8, (void(*)())&interrupt0xA8, 0);
    Interrupt::setInterrupt(0xA9, (void(*)())&interrupt0xA9, 0);
    Interrupt::setInterrupt(0xAA, (void(*)())&interrupt0xAA, 0);
    Interrupt::setInterrupt(0xAB, (void(*)())&interrupt0xAB, 0);
    Interrupt::setInterrupt(0xAC, (void(*)())&interrupt0xAC, 0);
    Interrupt::setInterrupt(0xAD, (void(*)())&interrupt0xAD, 0);
    Interrupt::setInterrupt(0xAE, (void(*)())&interrupt0xAE, 0);
    Interrupt::setInterrupt(0xAF, (void(*)())&interrupt0xAF, 0);
    Interrupt::setInterrupt(0xB0, (void(*)())&interrupt0xB0, 0);
    Interrupt::setInterrupt(0xB1, (void(*)())&interrupt0xB1, 0);
    Interrupt::setInterrupt(0xB2, (void(*)())&interrupt0xB2, 0);
    Interrupt::setInterrupt(0xB3, (void(*)())&interrupt0xB3, 0);
    Interrupt::setInterrupt(0xB4, (void(*)())&interrupt0xB4, 0);
    Interrupt::setInterrupt(0xB5, (void(*)())&interrupt0xB5, 0);
    Interrupt::setInterrupt(0xB6, (void(*)())&interrupt0xB6, 0);
    Interrupt::setInterrupt(0xB7, (void(*)())&interrupt0xB7, 0);
    Interrupt::setInterrupt(0xB8, (void(*)())&interrupt0xB8, 0);
    Interrupt::setInterrupt(0xB9, (void(*)())&interrupt0xB9, 0);
    Interrupt::setInterrupt(0xBA, (void(*)())&interrupt0xBA, 0);
    Interrupt::setInterrupt(0xBB, (void(*)())&interrupt0xBB, 0);
    Interrupt::setInterrupt(0xBC, (void(*)())&interrupt0xBC, 0);
    Interrupt::setInterrupt(0xBD, (void(*)())&interrupt0xBD, 0);
    Interrupt::setInterrupt(0xBE, (void(*)())&interrupt0xBE, 0);
    Interrupt::setInterrupt(0xBF, (void(*)())&interrupt0xBF, 0);
    Interrupt::setInterrupt(0xC0, (void(*)())&interrupt0xC0, 0);
    Interrupt::setInterrupt(0xC1, (void(*)())&interrupt0xC1, 0);
    Interrupt::setInterrupt(0xC2, (void(*)())&interrupt0xC2, 0);
    Interrupt::setInterrupt(0xC3, (void(*)())&interrupt0xC3, 0);
    Interrupt::setInterrupt(0xC4, (void(*)())&interrupt0xC4, 0);
    Interrupt::setInterrupt(0xC5, (void(*)())&interrupt0xC5, 0);
    Interrupt::setInterrupt(0xC6, (void(*)())&interrupt0xC6, 0);
    Interrupt::setInterrupt(0xC7, (void(*)())&interrupt0xC7, 0);
    Interrupt::setInterrupt(0xC8, (void(*)())&interrupt0xC8, 0);
    Interrupt::setInterrupt(0xC9, (void(*)())&interrupt0xC9, 0);
    Interrupt::setInterrupt(0xCA, (void(*)())&interrupt0xCA, 0);
    Interrupt::setInterrupt(0xCB, (void(*)())&interrupt0xCB, 0);
    Interrupt::setInterrupt(0xCC, (void(*)())&interrupt0xCC, 0);
    Interrupt::setInterrupt(0xCD, (void(*)())&interrupt0xCD, 0);
    Interrupt::setInterrupt(0xCE, (void(*)())&interrupt0xCE, 0);
    Interrupt::setInterrupt(0xCF, (void(*)())&interrupt0xCF, 0);
    Interrupt::setInterrupt(0xD0, (void(*)())&interrupt0xD0, 0);
    Interrupt::setInterrupt(0xD1, (void(*)())&interrupt0xD1, 0);
    Interrupt::setInterrupt(0xD2, (void(*)())&interrupt0xD2, 0);
    Interrupt::setInterrupt(0xD3, (void(*)())&interrupt0xD3, 0);
    Interrupt::setInterrupt(0xD4, (void(*)())&interrupt0xD4, 0);
    Interrupt::setInterrupt(0xD5, (void(*)())&interrupt0xD5, 0);
    Interrupt::setInterrupt(0xD6, (void(*)())&interrupt0xD6, 0);
    Interrupt::setInterrupt(0xD7, (void(*)())&interrupt0xD7, 0);
    Interrupt::setInterrupt(0xD8, (void(*)())&interrupt0xD8, 0);
    Interrupt::setInterrupt(0xD9, (void(*)())&interrupt0xD9, 0);
    Interrupt::setInterrupt(0xDA, (void(*)())&interrupt0xDA, 0);
    Interrupt::setInterrupt(0xDB, (void(*)())&interrupt0xDB, 0);
    Interrupt::setInterrupt(0xDC, (void(*)())&interrupt0xDC, 0);
    Interrupt::setInterrupt(0xDD, (void(*)())&interrupt0xDD, 0);
    Interrupt::setInterrupt(0xDE, (void(*)())&interrupt0xDE, 0);
    Interrupt::setInterrupt(0xDF, (void(*)())&interrupt0xDF, 0);
    Interrupt::setInterrupt(0xE0, (void(*)())&interrupt0xE0, 0);
    Interrupt::setInterrupt(0xE1, (void(*)())&interrupt0xE1, 0);
    Interrupt::setInterrupt(0xE2, (void(*)())&interrupt0xE2, 0);
    Interrupt::setInterrupt(0xE3, (void(*)())&interrupt0xE3, 0);
    Interrupt::setInterrupt(0xE4, (void(*)())&interrupt0xE4, 0);
    Interrupt::setInterrupt(0xE5, (void(*)())&interrupt0xE5, 0);
    Interrupt::setInterrupt(0xE6, (void(*)())&interrupt0xE6, 0);
    Interrupt::setInterrupt(0xE7, (void(*)())&interrupt0xE7, 0);
    Interrupt::setInterrupt(0xE8, (void(*)())&interrupt0xE8, 0);
    Interrupt::setInterrupt(0xE9, (void(*)())&interrupt0xE9, 0);
    Interrupt::setInterrupt(0xEA, (void(*)())&interrupt0xEA, 0);
    Interrupt::setInterrupt(0xEB, (void(*)())&interrupt0xEB, 0);
    Interrupt::setInterrupt(0xEC, (void(*)())&interrupt0xEC, 0);
    Interrupt::setInterrupt(0xED, (void(*)())&interrupt0xED, 0);
    Interrupt::setInterrupt(0xEE, (void(*)())&interrupt0xEE, 0);
    Interrupt::setInterrupt(0xEF, (void(*)())&interrupt0xEF, 0);
    Interrupt::setInterrupt(0xF0, (void(*)())&interrupt0xF0, 0);
    Interrupt::setInterrupt(0xF1, (void(*)())&interrupt0xF1, 0);
    Interrupt::setInterrupt(0xF2, (void(*)())&interrupt0xF2, 0);
    Interrupt::setInterrupt(0xF3, (void(*)())&interrupt0xF3, 0);
    Interrupt::setInterrupt(0xF4, (void(*)())&interrupt0xF4, 0);
    Interrupt::setInterrupt(0xF5, (void(*)())&interrupt0xF5, 0);
    Interrupt::setInterrupt(0xF6, (void(*)())&interrupt0xF6, 0);
    Interrupt::setInterrupt(0xF7, (void(*)())&interrupt0xF7, 0);
    Interrupt::setInterrupt(0xF8, (void(*)())&interrupt0xF8, 0);
    Interrupt::setInterrupt(0xF9, (void(*)())&interrupt0xF9, 0);
    Interrupt::setInterrupt(0xFA, (void(*)())&interrupt0xFA, 0);
    Interrupt::setInterrupt(0xFB, (void(*)())&interrupt0xFB, 0);
    Interrupt::setInterrupt(0xFC, (void(*)())&interrupt0xFC, 0);
    Interrupt::setInterrupt(0xFD, (void(*)())&interrupt0xFD, 0);
    Interrupt::setInterrupt(0xFE, (void(*)())&interrupt0xFE, 0);
    Interrupt::setInterrupt(0xFF, (void(*)())&interrupt0xFF, 0);
}

}