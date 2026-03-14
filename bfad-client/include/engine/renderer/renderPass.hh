#pragma once

#include <vulkan/vulkan.h>

#include "engine/types.hh"
#include "engine/context.hh"

#include "engine/renderer/framebuffer.hh"

namespace RenderPass {
    U0 end(VkCommandBuffer cmdBuffer);
    U0 begin(VkRenderPass renderPass, Framebuffer::It* framebuffers, VkCommandBuffer cmdBuffer, U32 imageIndex);
    U0 destroy(Context::It* ctx, VkRenderPass renderPass);
    VkRenderPass create(Context::It* ctx);
}

