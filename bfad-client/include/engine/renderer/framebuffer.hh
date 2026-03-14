#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "types.hh"
#include "engine/context.hh"
#include "engine/renderer/imageView.hh"

namespace Framebuffer {

    struct It {
        VkFramebuffer* handles;
        VkExtent2D extends;
        U32 bufferCount;
    };

    U0 destroy(Context::It* ctx, Framebuffer::It* framebuffers);
    Framebuffer::It* create(Context::It* ctx, VkSwapchainKHR swapchain, VkRenderPass renderPass, ImageView::It* imageView);
}
