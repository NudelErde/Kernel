#include "MassStorage.hpp"
#include "KernelOut.hpp"

namespace Kernel {

static uint64_t count = 0;
static uint64_t systemDevice = 0;
struct ListElement {
    Device* device;
    ListElement* next;
};
static ListElement* rootNode = nullptr;
static ListElement* lastNode = nullptr;

Device::~Device() {}

uint64_t Device::getDeviceCount() {
    return count;
}
Device* Device::getDevice(uint64_t index) {
    ListElement* node = rootNode;
    for (uint64_t i = 0; i < index; ++i) {
        if (node == nullptr)
            return nullptr;
        node = node->next;
    }
    if (node == nullptr)
        return nullptr;
    return node->device;
}
uint64_t Device::getSystemDevice() {
    return systemDevice;
}
uint64_t Device::addDevice(Device* device) {
    if (rootNode == nullptr) {
        rootNode = new ListElement();
        lastNode = rootNode;
    } else {
        lastNode->next = new ListElement();
        lastNode = lastNode->next;
    }
    lastNode->device = device;
    return count++;
}
void Device::setSystemDevice(uint64_t device) {
    systemDevice = device;
}

}// namespace Kernel