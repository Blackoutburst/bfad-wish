#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

namespace Fence {
    VkFence create(Context::It* ctx);
    U0 destroy(Context::It* ctx, VkFence fence);
    U0 wait(Context::It* ctx, VkFence fence);
    U0 reset(Context::It* ctx, VkFence fence);
}
