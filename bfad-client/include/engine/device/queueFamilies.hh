#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"

namespace QueueFamilies {
    U32 count(VkPhysicalDevice device);
    U8 hasType(VkPhysicalDevice device, VkQueueFlagBits type);
    U32 getType(VkPhysicalDevice device, VkQueueFlagBits type);
}
