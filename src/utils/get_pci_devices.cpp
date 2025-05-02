#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>

#include "utils/get_pci_devices.hpp"
#include "utils/hex_string.hpp"

void getPciDevices(vector<PCI_DEVICE> *pciDevices) {
  pciDevices->clear();
  const uint16_t domain = 0;
  
  // Debug: Open file for writing PCI device info
  std::ofstream debug_file("pci_devices_debug.txt");
  debug_file << "PCI Devices found by rw-tui:\n";
  debug_file << "Format: Bus:Device.Function VendorID:DeviceID\n";
  debug_file << "----------------------------------------\n";

  // Read all PCI devices from sysfs
  std::string sysfs_path = "/sys/bus/pci/devices/";
  for (const auto& entry : std::filesystem::directory_iterator(sysfs_path)) {
    std::string path = entry.path().filename().string();
    if (path.length() < 7) continue; // Skip if path is too short
    
    // Parse bus:device.function from path (format: 0000:00:00.0)
    std::string bus_str = path.substr(5, 2);
    std::string device_str = path.substr(8, 2);
    std::string function_str = path.substr(11, 1);
    
    uint16_t bus = std::stoul(bus_str, nullptr, 16);
    uint8_t device = std::stoul(device_str, nullptr, 16);
    uint8_t function = std::stoul(function_str, nullptr, 16);

    // Read vendor ID
    std::ifstream vendor_file(entry.path() / "vendor");
    if (!vendor_file) continue;
    std::string vendorId;
    vendor_file >> vendorId;
    vendor_file.close();

    // Read device ID
    std::ifstream device_file(entry.path() / "device");
    if (!device_file) continue;
    std::string deviceId;
    device_file >> deviceId;
    device_file.close();

    PCI_DEVICE item = {
        .bus = bus,
        .device = device,
        .function = function,
        .vendorId = (uint16_t)stoul(vendorId, nullptr, 16),
        .deviceId = (uint16_t)stoul(deviceId, nullptr, 16),
    };

    pciDevices->push_back(item);
  }

  // Sort devices by bus, device, and function numbers
  std::sort(pciDevices->begin(), pciDevices->end(), 
    [](const PCI_DEVICE& a, const PCI_DEVICE& b) {
      if (a.bus != b.bus) return a.bus < b.bus;
      if (a.device != b.device) return a.device < b.device;
      return a.function < b.function;
    });

  // Write sorted devices to debug file
  for (const auto& item : *pciDevices) {
    debug_file << hexString(item.bus, 2, false, false) << ":"
              << hexString(item.device, 2, false, false) << "."
              << hexString(item.function, 1, false, false) << " "
              << hexString(item.vendorId, 4, false, false) << ":"
              << hexString(item.deviceId, 4, false, false) << "\n";
  }
  
  debug_file.close();
}