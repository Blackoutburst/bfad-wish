#include <stdlib.h>

#include "renderer/imageView.hh"
#include "renderer/swapchain.hh"

namespace ImageView {
    U0 destroy(Context::It* ctx, ImageView::It* imageView, VkSwapchainKHR swapchain) {
        U32 swapChainImagesCount = Swapchain::getImagesCount(ctx, swapchain);

        for (U32 i = 0; i < swapChainImagesCount; i++) {
            vkDestroyImageView(ctx->device->logical, imageView->imageView[i], NULL);
        }

        free(imageView->swapChainImages);
        free(imageView->imageView);
        free(imageView);
    }

    ImageView::It* create(Context::It* ctx, VkSwapchainKHR swapchain) {
        ImageView::It* parent = (ImageView::It*)malloc(sizeof(ImageView::It));
        VkImageView* imageView;
        U32 swapChainImagesCount = Swapchain::getImagesCount(ctx, swapchain);
        VkImage* swapChainImages = Swapchain::getImages(ctx, swapchain);
        VkSurfaceFormatKHR surfaceFormat = Swapchain::getFormat(ctx);
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
            vkCreateImageView(ctx->device->logical, &createInfo, NULL, &imageView[i]);
        }

        parent->imageView = imageView;
        parent->swapChainImages = swapChainImages;
        parent->swapChainImagesCount = swapChainImagesCount;
        
        return parent;
    }
}

