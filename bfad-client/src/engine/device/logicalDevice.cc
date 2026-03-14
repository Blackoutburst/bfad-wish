#include <stdlib.h>
#include <string.h>

#include "engine/logger.h"

#include "engine/device/logicalDevice.hh"
#include "engine/device/queueFamilies.hh"
#include "engine/device/physicalDevice.hh"
#include "engine/renderer/windowSurface.hh"

#define WANTED_EXTENSIONS_COUNT 3
static const I8* wantedExtensions[WANTED_EXTENSIONS_COUNT] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    "VK_NVX_multiview_per_view_attributes",
    "VK_KHR_portability_subset",
};

static U32 querySupportedExtensions(VkPhysicalDevice physicalDevice, const I8** out) {
    U32 availableCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &availableCount, NULL);
    VkExtensionProperties* available = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * availableCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &availableCount, available);

    U32 count = 0;
    for (U32 i = 0; i < WANTED_EXTENSIONS_COUNT; i++) {
        for (U32 j = 0; j < availableCount; j++) {
            if (strcmp(wantedExtensions[i], available[j].extensionName) == 0) {
                out[count++] = wantedExtensions[i];
                break;
            }
        }
    }

    free(available);
    return count;
}

namespace LogicalDevice {
    U0 destroy(VkDevice device) {
        if (device != VK_NULL_HANDLE) {
            vkDestroyDevice(device, NULL);
        }
    }

    VkQueue getQueue(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkQueueFlagBits queueType) {
        VkQueue graphicsQueue;
        vkGetDeviceQueue(logicalDevice, QueueFamilies::getType(physicalDevice, queueType), 0, &graphicsQueue);

        return graphicsQueue;
    }

    VkQueue getPresentationQueue(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR windowSurface) {
        VkQueue presentationQueue;
        vkGetDeviceQueue(logicalDevice, getSurfaceSupport(physicalDevice, windowSurface), 0, &presentationQueue);

        return presentationQueue;
    }


    U32 getSurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR windowSurface) {
        U32 queueCount = QueueFamilies::count(device);
        VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queueCount);

        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queues);

        for (U32 i = 0; i < queueCount; i++) {
            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, windowSurface, &presentSupport);
            if (presentSupport) return i;
        }

        return 0;
    }

    VkDevice make(VkPhysicalDevice physicalDevice, VkSurfaceKHR windowSurface) {
        const F32 queuePriority = 1.0;
        VkDevice logicalDevice = VK_NULL_HANDLE;

        VkDeviceQueueCreateInfo graphicsQueueCreateInfo;
        graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        graphicsQueueCreateInfo.pNext = NULL;
        graphicsQueueCreateInfo.flags = 0;
        graphicsQueueCreateInfo.queueFamilyIndex = QueueFamilies::getType(physicalDevice, VK_QUEUE_GRAPHICS_BIT);
        graphicsQueueCreateInfo.queueCount = 1;
        graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;

        VkDeviceQueueCreateInfo presentQueueCreateInfo;
        presentQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        presentQueueCreateInfo.pNext = NULL;
        presentQueueCreateInfo.flags = 0;
        presentQueueCreateInfo.queueFamilyIndex = getSurfaceSupport(physicalDevice, windowSurface);
        presentQueueCreateInfo.queueCount = 1;
        presentQueueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceQueueCreateInfo queues[2] = { graphicsQueueCreateInfo, presentQueueCreateInfo };

        U8 sameQueue = QueueFamilies::getType(physicalDevice, VK_QUEUE_GRAPHICS_BIT) == getSurfaceSupport(physicalDevice, windowSurface);

        VkDeviceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.queueCreateInfoCount = sameQueue ? 1 : 2;
        createInfo.pQueueCreateInfos = sameQueue ? &graphicsQueueCreateInfo : queues;
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = NULL;
        const I8* enabledExtensions[WANTED_EXTENSIONS_COUNT];
        U32 enabledExtensionCount = querySupportedExtensions(physicalDevice, enabledExtensions);

        createInfo.enabledExtensionCount = enabledExtensionCount;
        createInfo.ppEnabledExtensionNames = enabledExtensions;
        createInfo.pEnabledFeatures = &deviceFeatures;

        vkCreateDevice(physicalDevice, &createInfo, NULL, &logicalDevice);

        return logicalDevice;
    }

    VkDevice create(VkPhysicalDevice physicalDevice, VkSurfaceKHR windowSurface) {
        VkDevice logicalDevice = VK_NULL_HANDLE;
        
        if (physicalDevice != VK_NULL_HANDLE) {
            logicalDevice = make(physicalDevice, windowSurface);
        }

        return logicalDevice;
    }
}
