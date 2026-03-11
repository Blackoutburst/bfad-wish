#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

namespace ImageView {
    struct It {
        U32 currentFrame;
        U32 imageIndex;
        VkImageView* imageView;
        VkImage* swapChainImages;
        U32 swapChainImagesCount;
        VkSemaphore* pSemaphore;
        VkSemaphore* rSemaphore;
    };

    U0 destroy(Context::It* ctx, ImageView::It* imageView);
    ImageView::It* create(Context::It* ctx, VkSwapchainKHR swapchain);
}

