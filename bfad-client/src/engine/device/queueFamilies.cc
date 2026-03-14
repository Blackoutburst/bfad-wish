#include <stdlib.h>

#include "engine/device/queueFamilies.hh"

namespace QueueFamilies {
    U32 count(VkPhysicalDevice device) {
        U32 count = 0;

        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, NULL);

        return count;
    }


    U8 hasType(VkPhysicalDevice device, VkQueueFlagBits type) {
        U32 queueCount = count(device);

        VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queues);

        for (U32 i = 0; i < queueCount; i++) {
            if (queues[i].queueFlags & type) return 1;
        }

        return 0;
    }

    U32 getType(VkPhysicalDevice device, VkQueueFlagBits type) {
        U32 queueCount = count(device);
        
        VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queueCount);

        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queues);

        for (U32 i = 0; i < queueCount; i++) {
            if (queues[i].queueFlags & type) return i;
        }

        return 0;
    }
}
