#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "devices/devices.hh"

namespace Semaphore {
    VkSemaphore create(Device::It* device);
    U0 destroy(Device::It* device, VkSemaphore semaphore);
}
