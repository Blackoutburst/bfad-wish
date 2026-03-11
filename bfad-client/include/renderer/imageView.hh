#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "devices/devices.hh"

namespace ImageView {
    struct It {
        VkImageView* imageView;
        VkImage* swapChainImages;
        U32 swapChainImagesCount;
    };

    U0 destroy(Device::It* d, ImageView::It* imageView, VkSwapchainKHR swapchain);
    ImageView::It* create(Device::It* d, VkSwapchainKHR swapchain, VkSurfaceKHR windowSurface);
}

