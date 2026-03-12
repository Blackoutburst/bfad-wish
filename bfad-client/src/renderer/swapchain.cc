#include <stdlib.h>

#include "renderer/swapchain.hh"
#include "renderer/windowSurface.hh"
#include "devices/logicalDevice.hh"
#include "devices/queueFamilies.hh"
#include "utils/math.hh"

namespace Swapchain {
    VkImage* getImages(Context::It* ctx, VkSwapchainKHR swapchain) {
        U32 count = getImagesCount(ctx, swapchain);
        VkImage* data = (VkImage*)malloc(sizeof(VkSurfaceFormatKHR) * count);
        vkGetSwapchainImagesKHR(ctx->device->logical, swapchain, &count, data);

        return data;
    }

    U32 getImagesCount(Context::It* ctx, VkSwapchainKHR swapchain) {
        U32 count = 0;
        vkGetSwapchainImagesKHR(ctx->device->logical, swapchain, &count, NULL);

        return count;
    }

    VkSurfaceFormatKHR getFormat(Context::It* ctx) {
        U32 count = 0;
        
        vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->device->physical, ctx->surface, &count, NULL);
        VkSurfaceFormatKHR* data = (VkSurfaceFormatKHR*)malloc(sizeof(VkSurfaceFormatKHR) * count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->device->physical, ctx->surface, &count, data);

        for (U32 i = 0; i < count; i++) {
            if (data[i].format == VK_FORMAT_B8G8R8A8_SRGB && data[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return data[i];
            }
        }

        return data[0];
    }

    VkPresentModeKHR getPresentMode(Context::It* ctx) {
        U32 count = 0;
        
        vkGetPhysicalDeviceSurfacePresentModesKHR(ctx->device->physical, ctx->surface, &count, NULL);
        VkPresentModeKHR* data = (VkPresentModeKHR*)malloc(sizeof(VkSurfaceFormatKHR) * count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(ctx->device->physical, ctx->surface, &count, data);

        for (U32 i = 0; i < count; i++) {
            if (data[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
                return data[i];
            }
        }

        return data[0];
    }

    VkSurfaceCapabilitiesKHR getCapabilities(Context::It* ctx) {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ctx->device->physical, ctx->surface, &capabilities);

        return capabilities;
    }

    VkExtent2D extend(Context::It* ctx) {
        VkSurfaceCapabilitiesKHR capabilities = getCapabilities(ctx);

        if (capabilities.currentExtent.width != 0xFFFFFFFF)
            return capabilities.currentExtent;

        I32 width = 0;
        I32 height = 0;
        glfwGetFramebufferSize(ctx->window, &width, &height);

        return {
            clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
        };
    }

    U0 setViewport(Context::It* ctx, VkCommandBuffer cmdBuffer) {
        VkExtent2D ext = extend(ctx);

        VkViewport viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (F32)ext.width;
        viewport.height = (F32)ext.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

        VkRect2D scissor;
        scissor.offset = {0, 0};
        scissor.extent = ext;
        vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);
    }

    U0 destroy(Context::It* ctx, VkSwapchainKHR swapchain) {
        vkDestroySwapchainKHR(ctx->device->logical, swapchain, NULL);
    }

    VkSwapchainKHR create(Context::It* ctx) {
        VkSwapchainKHR swapchain;
        VkSurfaceCapabilitiesKHR capabilities = getCapabilities(ctx);
        VkSurfaceFormatKHR surfaceFormat = getFormat(ctx);
        VkPresentModeKHR presentMode = getPresentMode(ctx);
        VkExtent2D extent = extend(ctx);

        U32 imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
            imageCount = capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.surface = ctx->surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        U32 queues[2] = { 
            QueueFamilies::getType(ctx->device->physical, VK_QUEUE_GRAPHICS_BIT), 
            LogicalDevice::getSurfaceSupport(ctx->device->physical, ctx->surface) 
        };
        
        U8 sameQueue = queues[0] == queues[1];

        if (sameQueue) {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = NULL;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queues;
        }

        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        vkCreateSwapchainKHR(ctx->device->logical, &createInfo, NULL, &swapchain);

        return swapchain;
    }
}
