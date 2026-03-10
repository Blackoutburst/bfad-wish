#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "types.hh"
#include "window/window.hh"
#include "devices/devices.hh"

namespace SwapChain {
    VkImage* getImages(Device::It* device, VkSwapchainKHR swapchain);
    U32 getImagesCount(Device::It* device, VkSwapchainKHR swapchain);
    VkSurfaceFormatKHR getFormat(Device::It* device, VkSurfaceKHR windowSurface);
    VkPresentModeKHR getPresentMode(Device::It* device, VkSurfaceKHR windowSurface);
    VkSurfaceCapabilitiesKHR getCapabilities(Device::It* device, VkSurfaceKHR windowSurface);
    VkExtent2D extend(GLFWwindow* window, Device::It* device, VkSurfaceKHR windowSurface);
    U0 destroy(Device::It* device, VkSwapchainKHR swapchain);
    VkSwapchainKHR create(GLFWwindow* window, Device::It* device, VkSurfaceKHR windowSurface);
}
