#include "engine/utils/fence.hh"

namespace Fence {
    VkFence create(Context::It* ctx) {
        VkFence fence;
        VkFenceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        vkCreateFence(ctx->device->logical, &createInfo, NULL, &fence);

        return fence;
    }

    U0 destroy(Context::It* ctx, VkFence fence) {
        vkDestroyFence(ctx->device->logical, fence, NULL);
    }

    U0 wait(Context::It* ctx, VkFence fence) {
        vkWaitForFences(ctx->device->logical, 1, &fence, VK_TRUE, UINT64_MAX);
    }
    
    U0 reset(Context::It* ctx, VkFence fence) {
        vkResetFences(ctx->device->logical, 1, &fence);
    }
}
