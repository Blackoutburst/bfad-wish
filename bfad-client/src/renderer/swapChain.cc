#include <stdlib.h>
#include "renderer/swapChain.hh"
#include "renderer/windowSurface.hh"
#include "devices/logicalDevice.hh"
#include "devices/queueFamilies.hh"
#include "utils/math.hh"

namespace SwapChain {
    VkImage* getImages(Device::It* device, VkSwapchainKHR swapchain) {
        U32 count = getImagesCount(device, swapchain);
        VkImage* data = (VkImage*)malloc(sizeof(VkSurfaceFormatKHR) * count);
        vkGetSwapchainImagesKHR(device->logical, swapchain, &count, data);

        return data;
    }

    U32 getImagesCount(Device::It* device, VkSwapchainKHR swapchain) {
        U32 count = 0;
        vkGetSwapchainImagesKHR(device->logical, swapchain, &count, NULL);

        return count;
    }

    VkSurfaceFormatKHR getFormat(Device::It* device, VkSurfaceKHR windowSurface) {
        U32 count = 0;
        
        vkGetPhysicalDeviceSurfaceFormatsKHR(device->physical, windowSurface, &count, NULL);
        VkSurfaceFormatKHR* data = (VkSurfaceFormatKHR*)malloc(sizeof(VkSurfaceFormatKHR) * count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device->physical, windowSurface, &count, data);

        for (U32 i = 0; i < count; i++) {
            if (data[i].format == VK_FORMAT_B8G8R8A8_SRGB && data[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return data[i];
            }
        }

        return data[0];
    }

    VkPresentModeKHR getPresentMode(Device::It* device, VkSurfaceKHR windowSurface) {
        U32 count = 0;
        
        vkGetPhysicalDeviceSurfacePresentModesKHR(device->physical, windowSurface, &count, NULL);
        VkPresentModeKHR* data = (VkPresentModeKHR*)malloc(sizeof(VkSurfaceFormatKHR) * count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device->physical, windowSurface, &count, data);

        for (U32 i = 0; i < count; i++) {
            if (data[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
                return data[i];
            }
        }

        return data[0];
    }

    VkSurfaceCapabilitiesKHR getCapabilities(Device::It* device, VkSurfaceKHR windowSurface) {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->physical, windowSurface, &capabilities);

        return capabilities;
    }

    VkExtent2D extend(GLFWwindow* window, Device::It* device, VkSurfaceKHR windowSurface) {
        VkSurfaceCapabilitiesKHR capabilities = getCapabilities(device, windowSurface);

        if (capabilities.currentExtent.width != 0xFFFFFFFF)
            return capabilities.currentExtent;

        I32 width = 0;
        I32 height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        return {
            clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
        };
    }

    U0 destroy(Device::It* device, VkSwapchainKHR swapchain) {
        vkDestroySwapchainKHR(device->logical, swapchain, NULL);
    }

    VkSwapchainKHR create(GLFWwindow* window, Device::It* device, VkSurfaceKHR windowSurface) {
        VkSwapchainKHR swapchain;
        VkSurfaceCapabilitiesKHR capabilities = getCapabilities(device, windowSurface);
        VkSurfaceFormatKHR surfaceFormat = getFormat(device, windowSurface);
        VkPresentModeKHR presentMode = getPresentMode(device, windowSurface);
        VkExtent2D extent = extend(window, device, windowSurface);

        U32 imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
            imageCount = capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.surface = windowSurface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        U32 queues[2] = { 
            QueueFamilies::getType(device->physical, VK_QUEUE_GRAPHICS_BIT), 
            LogicalDevice::getSurfaceSupport(device->physical, windowSurface) 
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

        vkCreateSwapchainKHR(device->logical, &createInfo, NULL, &swapchain);

        return swapchain;
    }
}
