#include <stdlib.h>

#include "renderer/imageView.hh"
#include "renderer/swapChain.hh"

namespace ImageView {
    U0 destroy(Device::It* d, VkImageView* imageView, VkSwapchainKHR swapchain) {
        VkDevice device = d->logical;
        U32 swapChainImagesCount = SwapChain::getImagesCount(d, swapchain);

        for (U32 i = 0; i < swapChainImagesCount; i++) {
            vkDestroyImageView(device, imageView[i], NULL);
        }

        free(imageView);
    }

    VkImageView* create(Device::It* d, VkSwapchainKHR swapchain, VkSurfaceKHR windowSurface) {
        VkImageView* imageView;
        VkDevice device = d->logical;
        U32 swapChainImagesCount = SwapChain::getImagesCount(d, swapchain);
        VkImage* swapChainImages = SwapChain::getImages(d, swapchain);
        VkSurfaceFormatKHR surfaceFormat = SwapChain::getFormat(d, windowSurface);
        VkFormat format = surfaceFormat.format;

        imageView = (VkImageView*)malloc(sizeof(VkImageView) * swapChainImagesCount);

        for (U32 i = 0; i < swapChainImagesCount; i++) {
            VkImageViewCreateInfo createInfo;
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.pNext = NULL;
            createInfo.flags = 0;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = format;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            vkCreateImageView(device, &createInfo, NULL, &imageView[i]);
        }

        return imageView;
    }
}

