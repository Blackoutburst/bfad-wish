#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

namespace Shader {
    U0 destroy(Context::It* ctx, VkShaderModule shader);
    VkShaderModule create(Context::It* ctx, const I8* filePath);
}
