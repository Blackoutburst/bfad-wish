#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

namespace Buffer {
    struct It {
        VkBuffer handle;
        VkDeviceMemory memory;
    };

    U32 findMemoryType(Context::It* ctx, U32 typeFilter, VkMemoryPropertyFlags properties);
    Buffer::It* create(Context::It* ctx, U32 size, VkBufferUsageFlagBits usage);
    U0 upload(Context::It* ctx, Buffer::It* buffer, U0* data, U32 size);
    U0 destroy(Context::It* ctx, Buffer::It* buffer);
}
