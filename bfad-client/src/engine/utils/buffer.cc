#include <stdlib.h>
#include <string.h>

#include "engine/utils/buffer.hh"

namespace Buffer {
    U32 findMemoryType(Context::It* ctx, U32 typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(ctx->device->physical, &memProps);

        for (U32 i = 0; i < memProps.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProps.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        return UINT32_MAX;
    }

    Buffer::It* create(Context::It* ctx, U32 size, VkBufferUsageFlagBits usage) {
        VkBufferCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.size = size;
        createInfo.usage = usage;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = NULL;

        Buffer::It* buffer = (Buffer::It*)malloc(sizeof(Buffer::It));
        vkCreateBuffer(ctx->device->logical, &createInfo, NULL, &buffer->handle);

        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(ctx->device->logical, buffer->handle, &memReqs);

        VkMemoryAllocateInfo allocInfo;
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = NULL;
        allocInfo.allocationSize = memReqs.size;
        allocInfo.memoryTypeIndex = findMemoryType(ctx, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        vkAllocateMemory(ctx->device->logical, &allocInfo, NULL, &buffer->memory);
        vkBindBufferMemory(ctx->device->logical, buffer->handle, buffer->memory, 0);

        return buffer;
    }

    U0 upload(Context::It* ctx, Buffer::It* buffer, U0* data, U32 size) {
        U0* mapped;
        vkMapMemory(ctx->device->logical, buffer->memory, 0, size, 0, &mapped);
        memcpy(mapped, data, size);
        vkUnmapMemory(ctx->device->logical, buffer->memory);
    }

    U0 destroy(Context::It* ctx, Buffer::It* buffer) {
        vkDestroyBuffer(ctx->device->logical, buffer->handle, NULL);
        vkFreeMemory(ctx->device->logical, buffer->memory, NULL);
    }
}
