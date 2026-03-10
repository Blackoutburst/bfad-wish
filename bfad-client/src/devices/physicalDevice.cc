#include <stdlib.h>
#include <string.h>

#include "utils/logger.hh"
#include "devices/physicalDevice.hh"
#include "devices/queueFamilies.hh"
#include "renderer/windowSurface.hh"
#include "renderer/rendererInstance.hh"

#define PHYSICAL_DEVICE_TYPE_COUNT 5

#define REQUIRED_EXTENSIONS_COUNT 1
static const I8* requiredExtensions[REQUIRED_EXTENSIONS_COUNT] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static const VkPhysicalDeviceType devicePriorityList[PHYSICAL_DEVICE_TYPE_COUNT] = {
    VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
    VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
    VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
    VK_PHYSICAL_DEVICE_TYPE_CPU,
    VK_PHYSICAL_DEVICE_TYPE_OTHER
};

namespace PhysicalDevice {
    VkPhysicalDeviceProperties getProperties(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);

        return properties;
    }

    const I8* typeName(VkPhysicalDeviceType type) {
        switch (type) {
            case VK_PHYSICAL_DEVICE_TYPE_OTHER: return "OTHER";
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "INTEGRATED_GPU";
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "DISCRETE_GPU";
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "VIRTUAL_GPU";
            case VK_PHYSICAL_DEVICE_TYPE_CPU: return "CPU";
            default: return "UNKOWN";
        }
    }

    U32 count(VkInstance instance) {
        U32 count = 0;

        vkEnumeratePhysicalDevices(instance, &count, NULL);

        return count;
    }

    U0 list(VkInstance instance) {
        U32 deviceCount = count(instance);
        VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * deviceCount);

        vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

        logI("Physical devices found: %i", deviceCount);

        for (U32 i = 0; i < deviceCount; i++) {
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
            logI("Device: %s | Type: %s", deviceProperties.deviceName, typeName(deviceProperties.deviceType));
        }

        free(devices);
    }

    U8 hasExtensions(VkPhysicalDevice device) {
        U32 extensionsCount = 0;
        
        vkEnumerateDeviceExtensionProperties(device, NULL, &extensionsCount, NULL);
        VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * extensionsCount);
        vkEnumerateDeviceExtensionProperties(device, NULL, &extensionsCount, extensions);

        U32 extensionsPresent = 0;
        for (U32 i = 0; i < REQUIRED_EXTENSIONS_COUNT; i++) {
        for (U32 j = 0; j < extensionsCount; j++) {
            if (!strcmp(requiredExtensions[i], extensions[j].extensionName))
                extensionsPresent++;
        }}

        return extensionsPresent == REQUIRED_EXTENSIONS_COUNT;
    }

    VkPhysicalDevice get(VkInstance instance, U8 desiredType, VkSurfaceKHR windowSurface) {
        U32 deviceCount = count(instance);
        VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * deviceCount);

        vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

        for (U32 i = 0; i < deviceCount; i++) {
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);

            if (!QueueFamilies::hasType(devices[i], VK_QUEUE_GRAPHICS_BIT)) continue;
            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(devices[i], i, windowSurface, &presentSupport);
            if (!presentSupport) continue;
            if (!hasExtensions(devices[i])) continue;

            if (deviceProperties.deviceType == desiredType) return devices[i];
        }

        return VK_NULL_HANDLE;
    }

    VkPhysicalDevice create(VkInstance instance, VkSurfaceKHR windowSurface) {
        VkPhysicalDevice device = VK_NULL_HANDLE;
        list(instance);

        VkPhysicalDeviceProperties deviceProperties;

        U32 i = 0;
        while (device == VK_NULL_HANDLE && i < PHYSICAL_DEVICE_TYPE_COUNT) {
            device = get(instance, devicePriorityList[i++], windowSurface);
        }
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        logI("GPU's selected: %s (%s)", deviceProperties.deviceName, typeName(deviceProperties.deviceType));
        
        return device;
    }
}
