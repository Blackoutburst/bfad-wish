#pragma once

#include "context.hh"
#include "types.hh"

#include "utils/buffer.hh"

namespace UniformBuffer {
    struct It {
        Buffer::It* buffer;
        VkDescriptorSetLayout setLayout;
        U0* mappedData;
    };

    UniformBuffer::It* create(Context::It* ctx, U32 binding, U32 size);
    U0 update(UniformBuffer::It* uniformBuffer, U0* data, U32 size);
    U0 destroy(Context::It* ctx, UniformBuffer::It* uniformBuffer);
}
