#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"

namespace PhysicalDevice {
    VkPhysicalDeviceProperties getProperties(VkPhysicalDevice device);
    const I8* typeName(VkPhysicalDeviceType type);
    U8 hasExtensions(VkPhysicalDevice device);
    U32 count(VkInstance instance);
    U0 list(VkInstance instance);
    VkPhysicalDevice get(VkInstance instance, U8 desiredType, VkSurfaceKHR windowSurface);
    VkPhysicalDevice create(VkInstance instance, VkSurfaceKHR windowSurface);
}

