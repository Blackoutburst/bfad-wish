#include "utils/fence.hh"

namespace Fence {
    VkFence create(Device::It* device) {
        VkFence fence;
        VkFenceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        vkCreateFence(device->logical, &createInfo, NULL, &fence);

        return fence;
    }

    U0 destroy(Device::It* device, VkFence fence) {
        vkDestroyFence(device->logical, fence, NULL);
    }

    U0 wait(Device::It* device, VkFence fence) {
        vkWaitForFences(device->logical, 1, &fence, VK_TRUE, UINT64_MAX);
    }
    
    U0 reset(Device::It* device, VkFence fence) {
        vkResetFences(device->logical, 1, &fence);
    }
}
