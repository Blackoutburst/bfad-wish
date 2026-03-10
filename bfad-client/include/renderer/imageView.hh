#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "devices/devices.hh"

namespace ImageView {
    U0 destroy(Device::It* d, VkImageView* imageView, VkSwapchainKHR swapchain);
    VkImageView* create(Device::It* d, VkSwapchainKHR swapchain, VkSurfaceKHR windowSurface);
}

