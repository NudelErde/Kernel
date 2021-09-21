#include "AHCI.hpp"
#include "KernelOut.hpp"
#include "SharedInterrupts.hpp"
#include "units.hpp"
#include "util.hpp"
#include "wait.hpp"

namespace Kernel {

struct FisDmaSetup {
    uint8_t fisType;// 0x41
    uint8_t portMultiplier : 4;
    uint8_t rsv0 : 1;
    bool dataDirection : 1;
    bool interrupt : 1;
    bool autoActivate : 1;
    uint8_t rsved[2];
    uint64_t DMAbufferID;
    uint32_t rsvd;
    uint32_t DMAbufOffset;
    uint32_t TransferCount;
    uint32_t resvd;
} __attribute__((packed));

struct FisH2D {
    uint8_t fisType;// 0x27
    uint8_t portMultiplier : 4;
    uint8_t rsv0 : 3;
    bool isCommand : 1;
    uint8_t command;
    uint8_t featurel;
    uint8_t lba0;
    uint8_t lba1;
    uint8_t lba2;
    uint8_t device;
    uint8_t lba3;
    uint8_t lba4;
    uint8_t lba5;
    uint8_t featureh;
    uint8_t countl;
    uint8_t counth;
    uint8_t icc;
    uint8_t control;
    uint8_t rsv1[4];
};

struct FisD2H {
    uint8_t fisType;// 0x34

    uint8_t portMultiply : 4;
    uint8_t rsv0 : 2;
    bool interrupt : 1;
    uint8_t rsv1 : 1;

    uint8_t status;
    uint8_t error;

    uint8_t lba0;
    uint8_t lba1;
    uint8_t lba2;
    uint8_t device;

    uint8_t lba3;
    uint8_t lba4;
    uint8_t lba5;
    uint8_t rsv2;

    uint8_t countLow;
    uint8_t countHigh;
    uint8_t rsv3[2];
    uint8_t rsv4[4];
} __attribute__((packed));

struct FisPIO {
    uint8_t fisType;// 0x5F
    uint8_t portMultiplier : 4;
    uint8_t rsv0 : 1;
    bool dataDirection : 1;
    bool interrupt : 1;
    uint8_t rsv1 : 1;
    uint8_t status;
    uint8_t error;
    uint8_t lba0;
    uint8_t lba1;
    uint8_t lba2;
    uint8_t device;
    uint8_t lba3;
    uint8_t lba4;
    uint8_t lba5;
    uint8_t rsv2;
    uint8_t countLow;
    uint8_t countHigh;
    uint8_t rsv3;
    uint8_t e_status;
    uint16_t transferCount;
    uint8_t rsv4[2];
} __attribute__((packed));

struct AHCI::ReceivedFIS {
    FisDmaSetup dmaSetup;
    uint8_t pad0[4];

    FisPIO pioSetup;
    uint8_t pad1[12];

    FisD2H deviceToHost;
    uint8_t pad2[4];

    // 0x58
    uint8_t sdbfis[2];// Set Device Bit FIS

    // 0x60
    uint8_t ufis[64];

    // 0xA0
    uint8_t rsv[0x100 - 0xA0];
} __attribute__((packed));

struct AHCI::Port {
    uint32_t commandListAddressLow;
    uint32_t commandListAddressHigh;
    uint32_t fisAddressLow;
    uint32_t fisAddressHigh;
    uint32_t interruptStatus;
    uint32_t interruptEnable;
    uint32_t commandAndStatus;
    uint32_t rsv0;
    uint32_t taskFileData;
    uint32_t signature;
    uint32_t sataStatus;
    uint32_t sataControl;
    uint32_t sataError;
    uint32_t sataActive;
    uint32_t commandIssue;
    uint32_t sataNotification;
    uint32_t fisBasedSwitch;
    uint32_t rsv1[11];
    uint32_t vendor[4];
} __attribute__((packed));

struct AHCI::CommandSlot {
    uint8_t commandFISLength : 5;
    bool isATAPI : 1;
    bool write : 1;
    bool prefetchable : 1;

    bool reset : 1;
    bool bist : 1;
    bool clearBusyOnOK : 1;
    uint8_t rsv0 : 1;
    uint8_t portMultiplier : 4;

    uint16_t prdTableLength;

    uint32_t prdByteCount;

    uint32_t commandTableBaseLow;
    uint32_t commandTableBaseHigh;

    uint32_t rsv1[4];

    inline CommandTable* getCommandTable() {
        return (CommandTable*) (commandTableBaseLow | (((uint64_t) commandTableBaseHigh) << 32));
    }
} __attribute__((packed));

struct AHCI::PrdtEntry {
    uint32_t dataBaseAddressLow;
    uint32_t dataBaseAddressHigh;
    uint32_t rsv0;
    uint32_t dataByteCount : 22;
    uint32_t rsv1 : 9;
    bool interruptEnable : 1;
} __attribute__((packed));

static constexpr uint64_t prdtEntryCount = ((pageSize / 0x10) - 64 - 16 - 48) / sizeof(AHCI::PrdtEntry);

struct AHCI::CommandTable {
    uint8_t commandFIS[64];

    uint8_t atapiCommand[16];
    uint8_t rsv[48];// Reserved

    PrdtEntry prdtEntries[prdtEntryCount];// Physical region descriptor table entries, 0 ~ 65535

    uint8_t createPrdt(void* address, uint64_t& size);// return prdt count
} __attribute__((packed));

static_assert(sizeof(AHCI::CommandTable) == (pageSize / 0x10));

static uint8_t* currentCommandList = nullptr;

static uint8_t* allocateCommandList(uint8_t elements) {
    if (currentCommandList == nullptr) {
        uint64_t ptr = PhysicalMemoryManagment::getFreeKernelPage();
        PhysicalMemoryManagment::setUsed(ptr, true);
        currentCommandList = (uint8_t*) ptr;
    }
    uint64_t page = ((uint64_t) currentCommandList) & ~(pageSize - 1);
    uint64_t offsetInPage = ((uint64_t) currentCommandList) & (pageSize - 1);
    uint64_t requestedSize = elements * sizeof(AHCI::CommandSlot);
    if (offsetInPage + requestedSize > pageSize) {
        uint64_t ptr = PhysicalMemoryManagment::getFreeKernelPage();
        PhysicalMemoryManagment::setUsed(ptr, true);
        currentCommandList = (uint8_t*) ptr;
    }
    uint8_t* result = currentCommandList;
    currentCommandList += requestedSize;
    return result;
}

static uint8_t* currentReceivedFIS = nullptr;

static uint8_t* allocateReceivedFIS() {
    if (currentCommandList == nullptr) {
        uint64_t ptr = PhysicalMemoryManagment::getFreeKernelPage();
        PhysicalMemoryManagment::setUsed(ptr, true);
        currentCommandList = (uint8_t*) ptr;
    }
    uint64_t page = ((uint64_t) currentCommandList) & ~(pageSize - 1);
    uint64_t offsetInPage = ((uint64_t) currentCommandList) & (pageSize - 1);
    uint64_t requestedSize = sizeof(AHCI::ReceivedFIS);
    if (offsetInPage + requestedSize > pageSize) {
        uint64_t ptr = PhysicalMemoryManagment::getFreeKernelPage();
        PhysicalMemoryManagment::setUsed(ptr, true);
        currentCommandList = (uint8_t*) ptr;
    }
    uint8_t* result = currentCommandList;
    currentCommandList += requestedSize;
    return result;
}

static void onSharedInterrupt(void* ptr) {
    AHCI* self = (AHCI*) ptr;
    self->onInterrupt();
}

AHCI::AHCIDevice::AHCIDevice(void* port, AHCI* controller) : port((Port*) port), controller(controller) {}

void AHCI::AHCIDevice::start() {
    while (port->commandAndStatus & (0b1 << 15))
        ;                             // wait for bit 15 clear
    port->commandAndStatus |= 0b10001;// start
}

void AHCI::AHCIDevice::stop() {
    port->commandAndStatus &= ~0b10001;// stop;
    while (port->commandAndStatus & (0b11 << 14))
        ;// wait for bit 14 and 13 to clear
}

void AHCI::AHCIDevice::setup() {
    //reset
    port->sataControl = 0b1;
    sleep(1);
    port->sataControl = 0b0;

    for (uint32_t i = 0; i < 0xFFFF; ++i) {
        if ((port->sataStatus & 0b1111) == 3) {
            break;
        }
    }

    uint8_t devicePowerState = (port->sataStatus >> 8) & 0b1111;
    uint8_t deviceDetection = (port->sataStatus & 0b1111);
    if (deviceDetection != 0b11)// present and connected
        return;
    if (devicePowerState != 1)// active power state
        return;
    port->sataError = ~0;
    stop();

    uint8_t commandSlots = ((controller->capabilities >> 8) & 0b11111) + 1;
    commandList = (CommandSlot*) allocateCommandList(commandSlots);
    memset(commandList, 0, sizeof(CommandSlot) * commandSlots);
    receivedFIS = (ReceivedFIS*) allocateReceivedFIS();
    memset(receivedFIS, 0, sizeof(ReceivedFIS));
    if (commandSlots >= 16) {
        uint64_t firstTry = PhysicalMemoryManagment::getFreeKernelPage();
        uint64_t page = firstTry;
        bool success = false;
        do {
            if (page + pageSize < 1Gi && PhysicalMemoryManagment::isValidMemory(page + pageSize) && !PhysicalMemoryManagment::isUsed(page + pageSize)) {
                success = true;
                break;
            }

            page = PhysicalMemoryManagment::getFreeKernelPage();// next try
        } while (firstTry != page);

        if (!success) {
            kout << "AHCI driver out of memory\n";
            asm("cli\nhlt");
        }
        PhysicalMemoryManagment::setUsed(page, true);
        PhysicalMemoryManagment::setUsed(page + pageSize, true);
        commandTables = (CommandTable*) page;
    } else {
        commandTables = (CommandTable*) PhysicalMemoryManagment::getFreeKernelPage();
        PhysicalMemoryManagment::setUsed((uint64_t) commandTables, true);
    }
    memset(commandTables, 0, sizeof(CommandTable) * commandSlots);
    for (uint64_t i = 0; i < commandSlots; ++i) {
        CommandTable* table = commandTables + i;
        commandList[i].prdTableLength = prdtEntryCount;
        commandList[i].commandTableBaseLow = (uint32_t) (uint64_t) table;
        commandList[i].commandTableBaseHigh = (uint32_t) (((uint64_t) table) >> 32);
    }

    port->commandListAddressLow = (uint32_t) (uint64_t) commandList;
    port->commandListAddressHigh = (uint32_t) (((uint64_t) commandList) >> 32);

    port->fisAddressLow = (uint32_t) (uint64_t) receivedFIS;
    port->fisAddressHigh = (uint32_t) (((uint64_t) receivedFIS) >> 32);

    start();

    if (!sendIdentify()) {
        return;
    }

    uint64_t id = Device::addDevice(this);
    if (type == DeviceType::SATA && sectorCount > 0) {
        Device::setSystemDevice(id);
    }
}

uint64_t AHCI::AHCIDevice::getSectorCount() {
    return sectorCount;
}

bool AHCI::AHCIDevice::sendIdentify() {
    uint64_t slotID = findSlot();
    if (slotID >= 32)
        return false;
    CommandSlot* slot = slotID + commandList;
    CommandTable* table = slot->getCommandTable();
    FisH2D* fis = (FisH2D*) &(table->commandFIS);
    memset(fis, 0, sizeof(FisH2D));
    fis->fisType = 0x27;
    fis->command = 0xEC;
    fis->isCommand = true;
    uint8_t result[512]{};
    table->prdtEntries[0].dataByteCount = 512;
    table->prdtEntries[0].dataBaseAddressLow = (uint32_t) (uint64_t) result;
    table->prdtEntries[0].dataBaseAddressHigh = (uint32_t) (((uint64_t) result) >> 32);
    table->prdtEntries[0].interruptEnable = false;
    slot->prdByteCount = 512;
    slot->commandFISLength = sizeof(FisH2D) / sizeof(uint32_t);
    if (!waitForFinish())
        return false;
    port->commandIssue |= 0b1 << slotID;// execute command
    if (!waitForTask(slotID))
        return false;

    constexpr uint8_t IdentitfyMaxLBAExt = 200;
    constexpr uint8_t IdentitfyMaxLBA = 120;
    uint64_t LBA48SectorCount = *(uint64_t*) (result + IdentitfyMaxLBAExt);
    uint64_t LBA28SectorCount = *(uint32_t*) (result + IdentitfyMaxLBA);

    if (LBA48SectorCount != 0) {
        hasLBA48 = true;
    }

    sectorCount = LBA48SectorCount > LBA28SectorCount ? LBA48SectorCount : LBA28SectorCount;
    switch (port->signature) {
        case 0xEB140101:
            type = DeviceType::SATAPI;
            break;
        case 0xC33C0101:
            type = DeviceType::SEMB;
            break;
        case 0x96690101:
            type = DeviceType::PM;
            break;
        case 0x00000101:
            type = DeviceType::SATA;
            break;
        case 0xFFFFFFFF:
        default:
            type = DeviceType::UNKNOWN;
            break;
    }
    return true;
}

bool AHCI::AHCIDevice::waitForTask(uint32_t slot) {
    while (true) {
        if (!(port->commandIssue & (0b1 << slot)))
            return true;
        if (port->sataError) {
            return false;
        }
        if (port->taskFileData & 0b1)
            return false;// error
    }
}

bool AHCI::AHCIDevice::waitForFinish() {
    while (true) {
        if (!(port->taskFileData & (0b10001) << 3)) return true;
        if (port->taskFileData & 0b1) return false;// error
    }
}

uint64_t AHCI::AHCIDevice::findSlot() {
    uint8_t maxSlot = ((controller->capabilities >> 8) & 0b11111) + 1;
    uint32_t slots = (port->sataActive | port->commandIssue);
    for (uint64_t i = 0; i < maxSlot; i++) {
        if ((slots & 1) == 0) {
            return i;
        }
        slots >>= 1;
    }
    return ~0;
}

uint8_t AHCI::CommandTable::createPrdt(void* address, uint64_t& size) {
    uint64_t startSize = size;
    if ((uint64_t) address & 0b1) {// is not word aligned
        return ~0;
    }

    uint64_t virtualAddress = (uint64_t) address;
    uint64_t physicalAddress = MemoryPage::getPhysicalAddressFromVirtual(virtualAddress);
    for (uint64_t i = 0; i < prdtEntryCount; ++i) {
        prdtEntries[i].interruptEnable = false;
        prdtEntries[i].dataByteCount = 0;
        prdtEntries[i].dataBaseAddressLow = 0;
        prdtEntries[i].dataBaseAddressHigh = 0;
    }
    uint64_t startPage = physicalAddress & ~(pageSize - 1);

    // align to pageSize
    uint64_t nextPage = (physicalAddress + pageSize) & ~(pageSize - 1);
    uint64_t prevPage = nextPage;
    uint8_t currentEntryIndex = 0;
    uint64_t currentIterationSize = min(nextPage - physicalAddress, size);
    size -= currentIterationSize;
    virtualAddress += currentIterationSize;

    prdtEntries[currentEntryIndex].dataByteCount = currentIterationSize - 1;
    prdtEntries[currentEntryIndex].dataBaseAddressLow = (uint32_t) physicalAddress;
    prdtEntries[currentEntryIndex].dataBaseAddressHigh = (uint32_t) (physicalAddress >> 32);
    bool firstInEntry = false;
    while (size) {
        nextPage = MemoryPage::getPhysicalAddressFromVirtual(virtualAddress);
        physicalAddress = nextPage;
        currentIterationSize = min(pageSize, size);

        if ((prevPage + pageSize != nextPage || (prdtEntries[currentEntryIndex].dataByteCount + currentIterationSize >= 4Mi))) {
            // the buffer in virtual memory is discontinuous in physical memory
            // or the entry contains too many bytes
            currentEntryIndex++;
            firstInEntry = true;
            if (currentEntryIndex == prdtEntryCount) {
                uint64_t sectorAlign = (startSize - size) % 512;
                if (sectorAlign) {
                    prdtEntries[currentEntryIndex - 1].dataByteCount -= sectorAlign;
                }
                return currentEntryIndex;
            }
        }

        if (firstInEntry) {
            prdtEntries[currentEntryIndex].dataByteCount = currentIterationSize - 1;
            prdtEntries[currentEntryIndex].dataBaseAddressLow = (uint32_t) physicalAddress;
            prdtEntries[currentEntryIndex].dataBaseAddressHigh = (uint32_t) (physicalAddress >> 32);
            firstInEntry = false;
        } else {
            prdtEntries[currentEntryIndex].dataByteCount += currentIterationSize;
        }

        size -= currentIterationSize;
        virtualAddress += currentIterationSize;
        prevPage = nextPage;
    }
    return currentEntryIndex + 1;
}

void AHCI::AHCIDevice::onInterrupt() {
}

void AHCI::AHCIDevice::writeH2DCommand(uint32_t slotID, uint8_t command, uint64_t sectorIndex, uint64_t sectorCount) {
    CommandSlot* slot = commandList + slotID;
    CommandTable* table = slot->getCommandTable();
    FisH2D* fis = (FisH2D*) &(table->commandFIS);
    slot->commandFISLength = sizeof(FisH2D) / sizeof(uint32_t);
    fis->fisType = 0x27;
    fis->device = 1 << 6;// LBA mode
    fis->command = command;
    fis->isCommand = true;
    fis->lba0 = (uint8_t) sectorIndex;
    fis->lba1 = (uint8_t) (sectorIndex >> 8);
    fis->lba2 = (uint8_t) (sectorIndex >> 16);
    fis->lba3 = (uint8_t) (sectorIndex >> 24);
    fis->lba4 = (uint8_t) (sectorIndex >> 32);
    fis->lba5 = (uint8_t) (sectorIndex >> 40);
    fis->countl = (uint8_t) sectorCount;
    fis->counth = (uint8_t) (sectorCount >> 8);
}

void AHCI::AHCIDevice::read(uint64_t sectorIndex, uint64_t sectorCount, uint8_t* dest) {
    if ((uint64_t) dest & 0b1) {
        // fix alignment
        uint8_t* tmpBuffer = new uint8_t[sectorCount * 512];
        read(sectorIndex, sectorCount, tmpBuffer);
        memcpy(dest, tmpBuffer, sectorCount * 512);
        return;
    }
    controller->dev->load();
    uint32_t slotID = findSlot();
    if (slotID >= 32) {
        flush();
        slotID = findSlot();
    }

    CommandSlot* slot = commandList + slotID;
    CommandTable* table = slot->getCommandTable();

    uint64_t size = sectorCount * 512;
    uint64_t lastSize = size;

    //if nothing happens fucking die
    uint8_t i = 0;
    uint64_t prevSize = size;

    while (size) {
        //if nothing happens fucking die
        if (prevSize <= size) {
            ++i;
            if (i == 8) {
                kout << "Big sad " __FILE__ ":" << (uint64_t) __LINE__ << '\n';
            }
        }
        prevSize = size;

        slot->prdTableLength = table->createPrdt(dest, size);
        if ((uint8_t) slot->prdTableLength > prdtEntryCount) {
            kout << "Buffer not word aligned\n";
            return;
        }
        uint64_t sizeDiff = lastSize - size;
        lastSize = size;
        uint64_t usedSectors = sizeDiff / 512;
        writeH2DCommand(slotID, 0x25, sectorIndex, usedSectors);
        sectorIndex += usedSectors;

        if (!waitForFinish()) {
            kout << "Error while reading\n";
            return;
        }
        port->commandIssue |= 0b1 << slotID;
        if (!waitForTask(slotID)) {
            kout << "Error while reading\n";
            return;
        }
    }
}
void AHCI::AHCIDevice::write(uint64_t sectorIndex, uint64_t sectorCount, uint8_t* src) {
    if ((uint64_t) src & 0b1) {
        // fix alignment
        uint8_t* tmpBuffer = new uint8_t[sectorCount * 512];
        memcpy(tmpBuffer, src, sectorCount * 512);
        write(sectorIndex, sectorCount, tmpBuffer);
        return;
    }
    controller->dev->load();
    uint32_t slotID = findSlot();
    if (slotID >= 32) {
        flush();
        slotID = findSlot();
    }

    CommandSlot* slot = commandList + slotID;
    CommandTable* table = slot->getCommandTable();

    uint64_t size = sectorCount * 512;
    uint64_t lastSize = size;
    while (size) {
        slot->prdTableLength = table->createPrdt(src, size);
        if (size == ~0) {
            kout << "Buffer not word aligned\n";
            return;
        }
        uint64_t sizeDiff = lastSize - size;
        lastSize = size;
        uint64_t usedSectors = sizeDiff / 512;
        writeH2DCommand(slotID, 0x35, sectorIndex, usedSectors);
        sectorIndex += usedSectors;

        if (!waitForFinish()) {
            kout << "Error while writing\n";
            return;
        }
        port->commandIssue |= 0b1 << slotID;
        if (!waitForTask(slotID)) {
            kout << "Error while writing\n";
            return;
        }
    }
}
void AHCI::AHCIDevice::flush() {
    controller->dev->load();
    uint64_t i = 0;
    while (port->commandIssue && !(port->taskFileData & 0b1)) {
        ++i;
        if (i >= 1000000000) {
            kout << "Big sad " __FILE__ ":" << (uint64_t) __LINE__ << '\n';
            return;
        }
    }
}

void AHCI::onInterrupt() {
    uint32_t interrupts = ABAR[2];
    for (uint8_t i = 0; interrupts; ++i, interrupts >>= 1) {
        if (interrupts & 0b1) {
            if (devices[i]) {
                devices[i]->onInterrupt();
            }
        }
    }
    ABAR[2] = 0xFFFFFFFF;// write = clear, crack shit lul
}

void AHCI::reset() {
    ABAR[1] = (0b1 << 31) | 0b11;
    while (ABAR[1] & 0b1)
        ;
    ABAR[1] = (0b1 << 31) | 0b10;
}

void AHCI::tryDevice(uint8_t device) {
    if (ABAR.io)// io BAR not supported
        return;
    uint32_t* DBAR = (uint32_t*) (ABAR.virtualAddress + 0x100 + (0x80 * device));
    AHCIDevice* dev = new AHCIDevice(DBAR, this);
    devices[device] = dev;
    dev->setup();
}

void AHCI::setup() {
    ABAR[1] = (0b1 << 31) | 0b10;
    while (ABAR[1] & 0b1) {}// wait for reset
    ABAR[1] = (0b1 << 31);
    portsAvailable = ABAR[3];
    version = ABAR[4];
    capabilities = ABAR[0];
    capabilities2 = ABAR[9];

    uint32_t bohc = ABAR[10];

    if ((capabilities2 & 0b1) && (bohc & 0b1)) {
        kout << "BIOS Handoff\n";
        bohc |= 0b10;
        ABAR[10] = bohc;
        while (true) {
            if ((ABAR[10] & 0b11) == 0b10)
                break;
        }
    }

    reset();

    uint32_t ports = portsAvailable;
    kout << "AHCI ports: " << BitList(ports) << '\n';
    for (uint8_t i = 0; ports; ++i, ports >>= 1) {
        if (ports & 0b1)
            tryDevice(i);
    }
}

void AHCI::openController(PCI* dev, const PCICommonHeader& header) {
    if ((header.headerType & (~0x80)) != 0x00)
        return;
    if (!SharedInterrupt::configInterrupt(dev, header))
        return;

    dev->writeConfig(0x04, dev->readConfig(0x04) | 0b111);// enable BAR

    AHCI* controller = new AHCI(dev->bars[5]);
    controller->dev = dev;
    dev->interrupt->setData(controller);
    dev->interrupt->setInterruptFunction(onSharedInterrupt);
    controller->setup();
}

}// namespace Kernel