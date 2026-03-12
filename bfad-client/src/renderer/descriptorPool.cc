#include "renderer/descriptorPool.hh"

#define UNIFORM_BUFFER_COUNT 2

namespace DescriptorPool {

    VkDescriptorPool create(Context::It* ctx) {
        VkDescriptorPoolSize poolSize;
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = UNIFORM_BUFFER_COUNT;

        VkDescriptorPoolCreateInfo poolInfo;
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.pNext = NULL;
        poolInfo.flags = 0;
        poolInfo.maxSets = UNIFORM_BUFFER_COUNT;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;

        VkDescriptorPool descriptorPool;
        vkCreateDescriptorPool(ctx->device->logical, &poolInfo, NULL, &descriptorPool);

        return descriptorPool;
    }
    
    U0 destroy(Context::It* ctx, VkDescriptorPool descriptorPool) {
        vkDestroyDescriptorPool(ctx->device->logical, descriptorPool, NULL);
    }
}
