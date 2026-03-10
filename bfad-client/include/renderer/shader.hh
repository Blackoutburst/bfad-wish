#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "devices/devices.hh"

namespace Shader {
    U0 destroy(Device::It* device, VkShaderModule shader);
    VkShaderModule create(Device::It* device, const I8* filePath);
}
