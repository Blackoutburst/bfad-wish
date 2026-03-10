#include "devices/commandBuffer.hh"


namespace CommandBuffer {
    U0 end(VkCommandBuffer cmdBuffer) {
        vkEndCommandBuffer(cmdBuffer);
    }

    U0 begin(VkCommandBuffer cmdBuffer) {
        VkCommandBufferBeginInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.pInheritanceInfo = NULL;

        vkBeginCommandBuffer(cmdBuffer, &createInfo);
    }

    U0 destroy(VkCommandBuffer cmdBuffer, VkCommandPool cmdPool, Device::It* device) {
        vkFreeCommandBuffers(device->logical, cmdPool, 1, &cmdBuffer);
    }

    VkCommandBuffer create(VkCommandPool cmdPool, Device::It* device) {
        VkCommandBuffer cmdBuffer;

        VkCommandBufferAllocateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        createInfo.pNext = NULL;
        createInfo.commandPool = cmdPool;
        createInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        createInfo.commandBufferCount = 1;

        vkAllocateCommandBuffers(device->logical, &createInfo, &cmdBuffer);
        
        return cmdBuffer;
    }
}

