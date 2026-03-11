#include "utils/semaphore.hh"

namespace Semaphore {
    VkSemaphore create(Device::It* device) {
        VkSemaphore semaphore;
        VkSemaphoreCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        createInfo.pNext = NULL;

        vkCreateSemaphore(device->logical, &createInfo, NULL, &semaphore);

        return semaphore;
    }

    U0 destroy(Device::It* device, VkSemaphore semaphore) {
        vkDestroySemaphore(device->logical, semaphore, NULL);
    }
}
