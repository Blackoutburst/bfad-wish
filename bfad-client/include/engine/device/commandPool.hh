#pragma once

#include <vulkan/vulkan.h>

#include "engine/types.hh"
#include "engine/context.hh"

namespace CommandPool {
    U0 destroy(Context::It* ctx, VkCommandPool cmdPool);
    VkCommandPool create(Context::It* ctx);
}
