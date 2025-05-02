#include <cstring>
#include <sys/mman.h>
#include <stdexcept>

#include "global.hpp"
#include "utils/get_pci_config_space.hpp"

void getPciConfigSpace(PCI_CONFIG_SPACE *pciConfigSpace) {
  // Validate input parameters
  if (!pciConfigSpace || !gPciDevices || !gDeviceSelectIndex || *gDeviceSelectIndex >= gPciDevices->size()) {
    throw std::runtime_error("Invalid PCI device selection");
  }

  // Calculate PCI configuration space address
  uint64_t address = *gBaseAddress +
                     (gPciDevices->at(*gDeviceSelectIndex).bus << 20) +
                     (gPciDevices->at(*gDeviceSelectIndex).device << 15) +
                     (gPciDevices->at(*gDeviceSelectIndex).function << 12);

  // Validate address alignment
  if (address & 0xFFF) {
    throw std::runtime_error("Invalid PCI configuration space address alignment");
  }

  // Map the PCI configuration space
  void *mapBase = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                      *gFileDescriptor, address & ~MAP_MASK);
  
  if (mapBase == (void *)-1) {
    throw std::runtime_error("Failed to map PCI configuration space");
  }

  // Calculate virtual address and copy data
  void *virtAddress = (uint8_t *)mapBase + (address & MAP_MASK);
  memcpy(pciConfigSpace, virtAddress, sizeof(PCI_CONFIG_SPACE));

  // Unmap the memory
  if (munmap(mapBase, MAP_SIZE) == -1) {
    throw std::runtime_error("Failed to unmap PCI configuration space");
  }
}