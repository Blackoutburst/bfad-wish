#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "devices/devices.hh"

namespace CommandBuffer {
    U0 begin(VkCommandBuffer cmdBuffer);
    U0 end(VkCommandBuffer cmdBuffer);
    U0 destroy(VkCommandBuffer cmdBuffer, VkCommandPool cmdPool, Device::It* device);
    VkCommandBuffer create(VkCommandPool cmdPool, Device::It* device);
}
