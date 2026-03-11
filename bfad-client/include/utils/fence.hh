#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "devices/devices.hh"

namespace Fence {
    VkFence create(Device::It* device);
    U0 destroy(Device::It* device, VkFence fence);
    U0 wait(Device::It* device, VkFence fence);
    U0 reset(Device::It* device, VkFence fence);
}
