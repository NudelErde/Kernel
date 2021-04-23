#include "ACPI.hpp"
#include "stdint.h"
#include "stddef.h"
#include "print.hpp"
#include "memory.hpp"
#include "KernelOut.hpp"
#include "time.hpp"
#include "inout.hpp"
#include "APIC.hpp"

namespace Kernel {

static bool basicStrEq(const char* a, const char* b, uint64_t len) {
    for(uint64_t index = 0; index < len; ++index) {
        if(a[index] != b[index])
            return false;
    }
    return true;
}

struct ACPISDTHeader {
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
}__attribute__((packed));

static uint64_t ioApicAddresses[10] {};
static uint8_t ioApicIndex = 0;

uint64_t* getIoAPICData(uint8_t& count) {
    count = ioApicIndex;
    return ioApicAddresses;
}

static bool ACPI_doChecksum(ACPISDTHeader *tableHeader)
{
    unsigned char sum = 0;
 
    for (int i = 0; i < tableHeader->Length; i++) {
        sum += ((char *) tableHeader)[i];
    }
 
    return sum == 0;
}

static void readFACS(uint8_t* facsPointer) {

}

static void readDSDT(uint8_t* dsdtPointer) {

}

static void readFADT(uint8_t* fadtPointer) {
    struct GenericAddressStructure {
        uint8_t AddressSpace;
        uint8_t BitWidth;
        uint8_t BitOffset;
        uint8_t AccessSize;
        uint64_t Address;
    }__attribute__((packed));
    struct FADT {
        struct   ACPISDTHeader h;
        uint32_t FirmwareCtrl;
        uint32_t Dsdt;
    
        // field used in ACPI 1.0; no longer in use, for compatibility only
        uint8_t  Reserved;
    
        uint8_t  PreferredPowerManagementProfile;
        uint16_t SCI_Interrupt;
        uint32_t SMI_CommandPort;
        uint8_t  AcpiEnable;
        uint8_t  AcpiDisable;
        uint8_t  S4BIOS_REQ;
        uint8_t  PSTATE_Control;
        uint32_t PM1aEventBlock;
        uint32_t PM1bEventBlock;
        uint32_t PM1aControlBlock;
        uint32_t PM1bControlBlock;
        uint32_t PM2ControlBlock;
        uint32_t PMTimerBlock;
        uint32_t GPE0Block;
        uint32_t GPE1Block;
        uint8_t  PM1EventLength;
        uint8_t  PM1ControlLength;
        uint8_t  PM2ControlLength;
        uint8_t  PMTimerLength;
        uint8_t  GPE0Length;
        uint8_t  GPE1Length;
        uint8_t  GPE1Base;
        uint8_t  CStateControl;
        uint16_t WorstC2Latency;
        uint16_t WorstC3Latency;
        uint16_t FlushSize;
        uint16_t FlushStride;
        uint8_t  DutyOffset;
        uint8_t  DutyWidth;
        uint8_t  DayAlarm;
        uint8_t  MonthAlarm;
        uint8_t  Century;
    
        // reserved in ACPI 1.0; used since ACPI 2.0+
        uint16_t BootArchitectureFlags;
    
        uint8_t  Reserved2;
        uint32_t Flags;
    
        // 12 byte structure; see below for details
        GenericAddressStructure ResetReg;
    
        uint8_t  ResetValue;
        uint8_t  Reserved3[3];
    
        // 64bit pointers - Available on ACPI 2.0+
        uint64_t                X_FirmwareControl;
        uint64_t                X_Dsdt;
    
        GenericAddressStructure X_PM1aEventBlock;
        GenericAddressStructure X_PM1bEventBlock;
        GenericAddressStructure X_PM1aControlBlock;
        GenericAddressStructure X_PM1bControlBlock;
        GenericAddressStructure X_PM2ControlBlock;
        GenericAddressStructure X_PMTimerBlock;
        GenericAddressStructure X_GPE0Block;
        GenericAddressStructure X_GPE1Block;
    }__attribute__((packed));
    FADT* fadt = (FADT*)fadtPointer;
    setCenturyRegister(fadt->Century);
}

static void readAPIC(void* ptr) {
    struct APICTable{
        ACPISDTHeader acpiHeader;
        uint32_t localAPICAddress;
        uint32_t flags;
    } __attribute__((packed));
    APICTable* apic = (APICTable*) ptr;
    LocalAPIC::localAPICAddress = apic->localAPICAddress;
    uint32_t remainingLength = apic->acpiHeader.Length;
    remainingLength -= sizeof(APICTable);
    uint8_t* data = (uint8_t*) ptr;
    data += sizeof(APICTable);
    while(remainingLength) {
        if(data[0] == 0) {
            uint8_t processorId = data[2];
            uint8_t apicId = data[3];
            uint32_t flags;
            memcpy(&flags, data + 4, 4);
        } else if(data[0] == 1) {
            uint8_t ioApicId = data[2];
            uint32_t ioApicAddress;
            memcpy(&ioApicAddress, data + 4, 4);
            uint32_t globalSystemInterruptBase;
            memcpy(&globalSystemInterruptBase, data + 8, 4);
            ioApicAddresses[ioApicIndex++] = ioApicAddress;
        } else if(data[0] == 2) {
            uint8_t busSource = data[2];
            uint8_t irqSource = data[3];
            uint32_t globalSystemInterrupt;
            memcpy(&globalSystemInterrupt, data + 4, 4);
            uint16_t flags;
            memcpy(&flags, data + 8, 2);
        } else if(data[0] == 4) {
            uint8_t processorId = data[0];
            uint16_t flags;
            memcpy(&flags, data + 3, 2);
            uint8_t LINT = data[5];
        } else if(data[0] == 5) {
            uint64_t address;
            memcpy(&address, data+4, 8);
        } else {
        }
        remainingLength -= data[1];
        data += data[1];
    }
}

static void processTable(uint64_t table, uint64_t doNotUnmapAddress) {
    ACPISDTHeader* h = (ACPISDTHeader*) (uint64_t)table;
    MemoryPage basePage;
    MemoryPage* pages;
    uint8_t allocatedPages = 0;

    if(table > 0x40000000) {
        allocatedPages = 1;
        basePage = MemoryPage(table, true);
        basePage.mapTo(table, true, false);
        if(basePage.getVirtualAddress() != (table & ~ 0xFFFu) ) {
            kout << "Error while mapping BasePage in ACPI processing";
            asm("hlt");
        }

        uint64_t last = table + h->Length;
        uint64_t firstPageStart = table & ~0xFFFu;
        uint64_t lastPageStart = last & ~0xFFFu;
        allocatedPages += (lastPageStart - firstPageStart) / pageSize;

        if(allocatedPages * sizeof(MemoryPage) > pageSize) {
            kout << "Table can not be allocated to virtual memory";
            asm("hlt");
        }
        uint64_t freePage = PhysicalMemoryManagment::getFreeKernelPage();
        PhysicalMemoryManagment::setUsed(freePage, true);
        uint8_t* ptr = (uint8_t*) freePage;
        pages = (MemoryPage*)(void*)ptr;
        for(uint8_t index = 1; index < allocatedPages; ++index) {
            uint64_t address = index * pageSize + firstPageStart;
            pages[index - 1] = MemoryPage(address, true);
            pages[index - 1].mapTo(address, true, false);
            if(pages[index - 1].getVirtualAddress() != (address & ~0xFFFu)) {
                kout << "Error while mapping BasePage of ";
                for(uint8_t i = 0; i < 4; ++i) {
                    kout << h->Signature[i];
                }
                asm("hlt");
            }
        }
    }

    if(basicStrEq(h->Signature, "FACP", 4)) {
        readFADT((uint8_t*)h);
    } else if(basicStrEq(h->Signature, "APIC", 4)) {
        readAPIC(h);
    }

    if(allocatedPages) {
        if(basePage.getVirtualAddress() != doNotUnmapAddress)
            basePage.unmap();
        for(uint8_t index = 1; index < allocatedPages; ++index) {
            if(pages[index-1].getVirtualAddress() != doNotUnmapAddress)
            pages[index - 1].unmap();
        }
        if(allocatedPages > 1) {
            uint64_t freePage = (uint64_t) pages;
            PhysicalMemoryManagment::setUsed(freePage, false);
        }
    }
}

static void readRSDT(uint8_t* rsdtPointer) {
    bool pageUsed = false;
    MemoryPage rsdtPage;

    if((uint64_t)rsdtPointer > 0x40000000) {
        pageUsed = true;
        rsdtPage = MemoryPage((uint64_t)rsdtPointer, true);
        rsdtPage.mapTo((uint64_t)rsdtPointer, true, false);
        if(rsdtPage.getVirtualAddress() != ((uint64_t)rsdtPointer & ~0xFFFu)) {
            kout << "Could not map RSDT in virtual memory\n";
            asm("hlt");
        }
    }
    ACPISDTHeader* header = (ACPISDTHeader*)rsdtPointer;
    uint64_t entryCount = (header->Length - sizeof(ACPISDTHeader)) / sizeof(uint32_t);
    for(uint64_t i = 0; i < entryCount; ++i) {
        uint32_t otherTable = *(uint32_t*)(rsdtPointer + sizeof(ACPISDTHeader) + i * sizeof(uint32_t));
        processTable(otherTable, ((uint64_t)rsdtPointer) & ~0xFFFu);
    }
    if(pageUsed)
        rsdtPage.unmap();
}

static void readXSDT(uint8_t* xsdtPointer) {
    bool pageUsed = false;
    MemoryPage rsdtPage;

    if((uint64_t)xsdtPointer > 0x40000000) {
        pageUsed = true;
        rsdtPage = MemoryPage((uint64_t)xsdtPointer, true);
        rsdtPage.mapTo((uint64_t)xsdtPointer, true, false);
        if(rsdtPage.getVirtualAddress() != ((uint64_t)xsdtPointer & ~0xFFFu)) {
            kout << "Could not map RSDT in virtual memory\n";
            asm("hlt");
        }
    }
    ACPISDTHeader* header = (ACPISDTHeader*)xsdtPointer;
    uint64_t entryCount = (header->Length - sizeof(ACPISDTHeader)) / sizeof(uint64_t);
    for(uint64_t i = 0; i < entryCount; ++i) {
        uint64_t otherTable = *(uint64_t*)(xsdtPointer + sizeof(ACPISDTHeader) + i * sizeof(uint64_t));
        processTable(otherTable, ((uint64_t)xsdtPointer) & ~0xFFFu);
    }
    if(pageUsed)
        rsdtPage.unmap();
}

void readACPITables() {
    if(getXSDTPointer())
        readXSDT((uint8_t*)getXSDTPointer());
    else if(getRSDTPointer())
        readRSDT((uint8_t*)getRSDTPointer());
}

}