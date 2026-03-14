#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

namespace ImageLayout {
    U0 transition(VkCommandBuffer cmdBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
}
