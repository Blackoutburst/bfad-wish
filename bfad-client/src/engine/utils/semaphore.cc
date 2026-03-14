#include "engine/utils/semaphore.hh"

namespace Semaphore {
    VkSemaphore create(Context::It* ctx) {
        VkSemaphore semaphore;
        VkSemaphoreCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;

        vkCreateSemaphore(ctx->device->logical, &createInfo, NULL, &semaphore);

        return semaphore;
    }

    U0 destroy(Context::It* ctx, VkSemaphore semaphore) {
        vkDestroySemaphore(ctx->device->logical, semaphore, NULL);
    }
}
