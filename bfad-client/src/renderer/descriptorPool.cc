#include "renderer/descriptorPool.hh"

#define UNIFORM_BUFFER_COUNT 2
#define SAMPLER_COUNT 1

namespace DescriptorPool {

    VkDescriptorPool create(Context::It* ctx) {
        VkDescriptorPoolSize poolSizes[2];
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = UNIFORM_BUFFER_COUNT;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = SAMPLER_COUNT;

        VkDescriptorPoolCreateInfo poolInfo;
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.pNext = NULL;
        poolInfo.flags = 0;
        poolInfo.maxSets = UNIFORM_BUFFER_COUNT + SAMPLER_COUNT;
        poolInfo.poolSizeCount = 2;
        poolInfo.pPoolSizes = poolSizes;

        VkDescriptorPool descriptorPool;
        vkCreateDescriptorPool(ctx->device->logical, &poolInfo, NULL, &descriptorPool);

        return descriptorPool;
    }
    
    U0 destroy(Context::It* ctx, VkDescriptorPool descriptorPool) {
        vkDestroyDescriptorPool(ctx->device->logical, descriptorPool, NULL);
    }
}
