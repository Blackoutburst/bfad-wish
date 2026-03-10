#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "types.hh"
#include "devices/devices.hh"

namespace Framebuffer {
    U0 destroy(Device::It* device, VkSwapchainKHR swapchain, VkFramebuffer* framebuffer);
    VkFramebuffer* create(GLFWwindow* window, Device::It* device, VkSwapchainKHR swapchain, VkRenderPass renderPass, VkImageView* imageView, VkSurfaceKHR windowSurface);
}
