#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

namespace RenderPass {
    U0 end(VkCommandBuffer cmdBuffer);
    U0 begin(Context::It* ctx, VkRenderPass renderPass, VkFramebuffer* framebuffers, VkCommandBuffer cmdBuffer, U32 imageIndex);
    U0 destroy(Context::It* ctx, VkRenderPass renderPass);
    VkRenderPass create(Context::It* ctx);
}

