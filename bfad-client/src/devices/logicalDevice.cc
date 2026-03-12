#include <stdlib.h>

#include "devices/logicalDevice.hh"
#include "devices/queueFamilies.hh"
#include "devices/physicalDevice.hh"
#include "renderer/windowSurface.hh"
#include "utils/logger.hh"

#define REQUIRED_EXTENSIONS_COUNT 2
static const I8* requiredExtensions[REQUIRED_EXTENSIONS_COUNT] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    "VK_NVX_multiview_per_view_attributes",
    //"VK_KHR_portability_subset",
};

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
        createInfo.enabledExtensionCount = REQUIRED_EXTENSIONS_COUNT;
        createInfo.ppEnabledExtensionNames = requiredExtensions;
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
