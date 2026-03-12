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

        vkResetCommandBuffer(cmdBuffer, 0);
        vkBeginCommandBuffer(cmdBuffer, &createInfo);
    }

    U0 destroy(Context::It* ctx, VkCommandBuffer cmdBuffer) {
        vkFreeCommandBuffers(ctx->device->logical, ctx->cmdPool, 1, &cmdBuffer);
    }

    VkCommandBuffer create(Context::It* ctx) {
        VkCommandBuffer cmdBuffer;

        VkCommandBufferAllocateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        createInfo.pNext = NULL;
        createInfo.commandPool = ctx->cmdPool;
        createInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        createInfo.commandBufferCount = 1;

        vkAllocateCommandBuffers(ctx->device->logical, &createInfo, &cmdBuffer);
        
        return cmdBuffer;
    }
}

