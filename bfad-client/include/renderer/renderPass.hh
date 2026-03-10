#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "types.hh"
#include "devices/devices.hh"

namespace RenderPass {
    U0 end(VkCommandBuffer cmdBuffer);
    U0 begin(GLFWwindow* window, Device::It* device, VkRenderPass renderPass, VkFramebuffer* framebuffers, VkCommandBuffer cmdBuffer, U32 imageIndex, VkSurfaceKHR windowSurface);
    U0 destroy(Device::It* device, VkRenderPass renderPass);
    VkRenderPass create(Device::It* device, VkSurfaceKHR windowSurface);
}

