#pragma once

#include <vulkan/vulkan.h>

#include "engine/types.hh"

namespace Device {

    struct It {
        VkDevice logical;
        VkPhysicalDevice physical;
        VkPhysicalDeviceProperties properties;
        VkQueue graphicQueue;
        VkQueue presentQueue;
    };

    U0 make(Device::It* device, VkPhysicalDevice physical, VkDevice logical, VkSurfaceKHR windowSurface);
    U0 print(Device::It* device);
    U0 destroy(Device::It* device);
    Device::It* create(VkPhysicalDevice physical, VkDevice logical, VkSurfaceKHR windowSurface);
}

