#include "devices/commandPool.hh"
#include "devices/queueFamilies.hh"

namespace CommandPool {
    U0 destroy(VkCommandPool cmdPool, Device::It* device) {
        vkDestroyCommandPool(device->logical, cmdPool, NULL);
    }

    VkCommandPool create(Device::It* device) {
        VkCommandPool cmdPool;
        U32 graphicQueue = QueueFamilies::getType(device->physical, VK_QUEUE_GRAPHICS_BIT);

        VkCommandPoolCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        createInfo.queueFamilyIndex = graphicQueue;

        vkCreateCommandPool(device->logical, &createInfo, NULL, &cmdPool);

        return cmdPool;
    }
}

