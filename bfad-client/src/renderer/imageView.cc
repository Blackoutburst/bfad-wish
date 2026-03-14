#include <stdlib.h>

#include "renderer/imageView.hh"
#include "renderer/swapchain.hh"
#include "utils/semaphore.hh"
#include "utils/buffer.hh"

namespace ImageView {
    U0 destroy(Context::It* ctx, ImageView::It* imageView) {
        for (U32 i = 0; i < imageView->swapChainImagesCount; i++) {
            vkDestroyImageView(ctx->device->logical, imageView->handle[i], NULL);
            Semaphore::destroy(ctx, imageView->pSemaphore[i]);
            Semaphore::destroy(ctx, imageView->rSemaphore[i]);
        }

        vkDestroyImageView(ctx->device->logical, imageView->depthImageView, NULL);
        vkDestroyImage(ctx->device->logical, imageView->depthImage, NULL);
        vkFreeMemory(ctx->device->logical, imageView->depthImageMemory, NULL);

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

        createDepthImage(ctx, parent);
        
        return parent;
    }

    U0 createDepthImage(Context::It* ctx, ImageView::It* imageView) {
        VkExtent2D extends = Swapchain::extend(ctx);

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        VkImageCreateInfo imageInfo;
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.pNext = NULL;
        imageInfo.flags = 0;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = VK_FORMAT_D32_SFLOAT;
        imageInfo.extent = { extends.width, extends.height, 1 };
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.queueFamilyIndexCount = 0;
        imageInfo.pQueueFamilyIndices = NULL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        vkCreateImage(ctx->device->logical, &imageInfo, NULL, &depthImage);

        VkMemoryRequirements memReqs;
        vkGetImageMemoryRequirements(ctx->device->logical, depthImage, &memReqs);

        VkMemoryAllocateInfo allocInfo;
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = NULL;
        allocInfo.allocationSize = memReqs.size;
        allocInfo.memoryTypeIndex = Buffer::findMemoryType(ctx, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkAllocateMemory(ctx->device->logical, &allocInfo, NULL, &depthImageMemory);
        vkBindImageMemory(ctx->device->logical, depthImage, depthImageMemory, 0);

        VkImageViewCreateInfo viewInfo;
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.pNext = NULL;
        viewInfo.flags = 0;
        viewInfo.image = depthImage;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = VK_FORMAT_D32_SFLOAT;
        viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        vkCreateImageView(ctx->device->logical, &viewInfo, NULL, &depthImageView);

        imageView->depthImage = depthImage;
        imageView->depthImageMemory = depthImageMemory;
        imageView->depthImageView = depthImageView;
    }

    VkImageView makeView(Context::It* ctx, VkImage image, VkFormat format, VkImageAspectFlags aspectMask) {
        VkImageView view;

        VkImageViewCreateInfo viewInfo;
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.pNext = NULL;
        viewInfo.flags = 0;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.subresourceRange.aspectMask = aspectMask;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        vkCreateImageView(ctx->device->logical, &viewInfo, NULL, &view);

        return view;
    }
}

