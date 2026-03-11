#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

namespace CommandBuffer {
    U0 begin(VkCommandBuffer cmdBuffer);
    U0 end(VkCommandBuffer cmdBuffer);
    U0 destroy(Context::It* ctx, VkCommandBuffer cmdBuffer, VkCommandPool cmdPool);
    VkCommandBuffer create(Context::It* ctx, VkCommandPool cmdPool);
}
