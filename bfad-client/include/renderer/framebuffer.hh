#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "types.hh"
#include "context.hh"

namespace Framebuffer {
    U0 destroy(Context::It* ctx, VkSwapchainKHR swapchain, VkFramebuffer* framebuffer);
    VkFramebuffer* create(Context::It* ctx, VkSwapchainKHR swapchain, VkRenderPass renderPass, VkImageView* imageView);
}
