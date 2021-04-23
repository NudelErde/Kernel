#include "time.hpp"
#include "inout.hpp"

namespace Kernel {
    
static uint8_t centuryRegister;

void setCenturyRegister(uint8_t _centuryRegister) {
    centuryRegister = _centuryRegister;
}

static int isRTCUpdateInProgess() {
      outb(0x70, 0x0A);
      return (inb(0x71) & 0x80);
}

static uint8_t getRTCRegister(uint8_t reg) {
    outb(0x70, reg);
    return inb(0x71);
}

static uint8_t BCD2Bin(uint8_t BCD) {
    return ((BCD / 16) * 10) + (BCD & 0xf);
}

static uint64_t toTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    const uint64_t monthStartDay[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    uint64_t leapYear = ((year - 1) - 1968) / 4 - ((year - 1) - 1900) / 100 + ((year - 1) - 1600) / 400;

    uint64_t days = (year - 1970) * 365 + leapYear + monthStartDay[month - 1] + day - 1;

    if ((month > 2) && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))
        days += 1;

    return second + 60 * (minute + 60 * (hour + 24 * days));
}

uint64_t timeSinceEpoch() {
    if(centuryRegister == 0)
        return 0;
    
    while(isRTCUpdateInProgess()); //finish update
    uint8_t second = getRTCRegister(0x00);
    uint8_t minute = getRTCRegister(0x02);
    uint8_t hour = getRTCRegister(0x04);
    uint8_t dayOfMonth = getRTCRegister(0x07);
    uint8_t monthOfYear = getRTCRegister(0x08);
    uint8_t year = getRTCRegister(0x09);
    uint8_t century = getRTCRegister(centuryRegister);

    uint8_t registerB = getRTCRegister(0x0B);

    bool pm = hour & 0x80;

    if(!(registerB & 0x04)) {
        //crack BCD format :c
        second = BCD2Bin(second);
        minute = BCD2Bin(minute);
        hour = BCD2Bin(hour);
        dayOfMonth = BCD2Bin(dayOfMonth);
        monthOfYear = BCD2Bin(monthOfYear);
        year = BCD2Bin(year);
        century = BCD2Bin(century);
    }

    if(!(registerB & 0x02) && pm) {
        hour += 12;
    }

    uint16_t realYear = century * 100 + year;

    return toTime(year, monthOfYear, dayOfMonth, hour, minute, second);
}

}