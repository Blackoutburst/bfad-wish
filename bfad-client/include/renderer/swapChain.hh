#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

#include "renderer/imageView.hh"

namespace Swapchain {
    VkImage* getImages(Context::It* ctx, VkSwapchainKHR swapchain);
    U32 getImagesCount(Context::It* ctx, VkSwapchainKHR swapchain);
    VkSurfaceFormatKHR getFormat(Context::It* ctx);
    VkPresentModeKHR getPresentMode(Context::It* ctx);
    VkSurfaceCapabilitiesKHR getCapabilities(Context::It* ctx);
    VkExtent2D extend(Context::It* ctx);
    U0 destroy(Context::It* ctx, VkSwapchainKHR swapchain);
    VkSwapchainKHR create(Context::It* ctx);
}
