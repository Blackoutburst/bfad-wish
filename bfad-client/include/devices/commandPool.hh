#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "devices/devices.hh"

namespace CommandPool {
    U0 destroy(VkCommandPool cmdPool, Device::It* device);
    VkCommandPool create(Device::It* device);
}
