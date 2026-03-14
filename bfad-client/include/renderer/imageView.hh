#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

namespace ImageView {
    struct It {
        VkImageView* handle;
        U32 currentFrame;
        U32 imageIndex;
        VkImage* swapChainImages;
        U32 swapChainImagesCount;
        VkSemaphore* pSemaphore;
        VkSemaphore* rSemaphore;
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;
    };

    U0 destroy(Context::It* ctx, ImageView::It* imageView);
    ImageView::It* create(Context::It* ctx, VkSwapchainKHR swapchain);
    U0 createDepthImage(Context::It* ctx, ImageView::It* imageView);
    VkImageView makeView(Context::It* ctx, VkImage image, VkFormat format, VkImageAspectFlags aspectMask);
}

