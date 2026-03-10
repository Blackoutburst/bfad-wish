#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"

namespace LogicalDevice {
    VkQueue getQueue(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkQueueFlagBits queueType);
    VkQueue getPresentationQueue(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR windowSurface);
    U32 getSurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR windowSurface);
    U0 destroy(VkDevice device);
    VkDevice make(VkPhysicalDevice physicalDevice, VkSurfaceKHR windowSurface);
    VkDevice create(VkPhysicalDevice physicalDevice, VkSurfaceKHR windowSurface);
}

