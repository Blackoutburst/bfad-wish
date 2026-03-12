#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

#include "renderer/swapchain.hh"
#include "renderer/imageView.hh"

namespace RenderSystem {
    struct It {
        VkSwapchainKHR swapchain;
        ImageView::It* imageView;
        VkFramebuffer* framebuffers;
        VkRenderPass renderPass;
    };

    RenderSystem::It* create(Context::It* ctx);
    U0 destroy(Context::It* ctx, RenderSystem::It* renderSystem);
    U0 update(Context::It* ctx, RenderSystem::It* renderSystem);
    U0 submit(Context::It* ctx, RenderSystem::It* renderSystem, VkCommandBuffer cmdBuffer, VkFence fence);
    U0 present(Context::It* ctx, RenderSystem::It* renderSystem);
}
