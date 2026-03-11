#include <stdlib.h>

#include "renderer/imageView.hh"
#include "renderer/swapchain.hh"
#include "utils/semaphore.hh"

namespace ImageView {
    U0 destroy(Context::It* ctx, ImageView::It* imageView) {
        for (U32 i = 0; i < imageView->swapChainImagesCount; i++) {
            vkDestroyImageView(ctx->device->logical, imageView->handle[i], NULL);
            Semaphore::destroy(ctx, imageView->pSemaphore[i]);
            Semaphore::destroy(ctx, imageView->rSemaphore[i]);
        }

        free(imageView->pSemaphore);
        free(imageView->rSemaphore);
        free(imageView->swapChainImages);
        free(imageView->handle);
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

        VkSemaphore* pSemaphore = (VkSemaphore*)malloc(swapChainImagesCount * sizeof(VkSemaphore));
        VkSemaphore* rSemaphore = (VkSemaphore*)malloc(swapChainImagesCount * sizeof(VkSemaphore));

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

            pSemaphore[i] = Semaphore::create(ctx);
            rSemaphore[i] = Semaphore::create(ctx);
        }

        parent->currentFrame = 0;
        parent->imageIndex = 0;
        parent->handle = imageView;
        parent->swapChainImages = swapChainImages;
        parent->swapChainImagesCount = swapChainImagesCount;
        parent->pSemaphore = pSemaphore;
        parent->rSemaphore = rSemaphore;
        
        return parent;
    }
}

