#include <stdlib.h>

#include "logger.hh"

#include "engine/device/device.hh"
#include "engine/device/logicalDevice.hh"
#include "engine/device/physicalDevice.hh"
#include "engine/renderer/rendererInstance.hh"

namespace Device {
    U0 print(Device::It* device) {
        if (device == NULL) {
            logI("Device: NULL");
        } else {
            logI("Device: %s (%s)", device->properties.deviceName, PhysicalDevice::typeName(device->properties.deviceType));
        }
    }

    U0 destroy(Device::It* device) {
        LogicalDevice::destroy(device->logical);

        if (device != NULL) {
            free(device);
        }
    }

    U0 make(Device::It* device, VkPhysicalDevice physical, VkDevice logical, VkSurfaceKHR windowSurface) {
        device->physical = physical;
        device->logical = logical;
        device->properties = PhysicalDevice::getProperties(device->physical);
        device->graphicQueue = LogicalDevice::getQueue(device->physical, device->logical, VK_QUEUE_GRAPHICS_BIT);
        device->presentQueue = LogicalDevice::getPresentationQueue(device->physical, device->logical, windowSurface);
    }

    Device::It* create(VkPhysicalDevice physical, VkDevice logical, VkSurfaceKHR windowSurface) {
        if (physical != VK_NULL_HANDLE && logical != VK_NULL_HANDLE) {
            Device::It* device = (Device::It*)malloc(sizeof(Device::It));
            
            make(device, physical, logical, windowSurface);

            return device;
        }

        return NULL;
    }
}


