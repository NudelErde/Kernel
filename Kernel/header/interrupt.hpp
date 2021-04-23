#pragma once

#include "stdint.h"
#include "interruptExceptionHandler.hpp"
#include "process.hpp"

namespace Kernel {

    struct Interrupt{
        public:
            typedef void(*Handler)(const Interrupt&);

            uint64_t errorCode;
            bool hasErrorCode;
            bool isHardwareInterrupt;
            uint8_t interruptNumber;
            void* stackFrame;

            static void setHandler(uint8_t interruptNumber, Handler handler);
            static Handler getHandler(uint8_t interruptNumber);

            static void setDefaultHandler(Handler handler);
            static Handler getDefaultHandler();

            static void init();

            static void setHardwareInterruptMask(uint8_t irq, bool enable);
            static void sendHardwareEOI(uint8_t irq);
        private:
            static void setInterrupt(uint8_t interruptID, void(*func)());

            friend void Kernel::initExceptionHandlers();
            friend void Kernel::Process::init();
    };
}