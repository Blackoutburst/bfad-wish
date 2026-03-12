#pragma once

#include <vulkan/vulkan.h>

#include "context.hh"
#include "types.hh"

namespace DescriptorPool {
    VkDescriptorPool create(Context::It* ctx);
    U0 destroy(Context::It* ctx, VkDescriptorPool descriptorPool);
}
