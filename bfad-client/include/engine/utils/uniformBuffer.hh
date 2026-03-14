#pragma once

#include "types.hh"
#include "engine/context.hh"

#include "engine/utils/buffer.hh"

namespace UniformBuffer {
    struct It {
        Buffer::It* buffer;
        VkDescriptorSetLayout setLayout;
        U0* mappedData;
        U32 binding;
        U32 size;
        VkDescriptorSet descriptorSet;
    };

    UniformBuffer::It* create(Context::It* ctx, U32 binding, U32 size);
    U0 update(UniformBuffer::It* uniformBuffer, U0* data, U32 size);
    U0 bind(UniformBuffer::It* uniformBuffer, VkCommandBuffer cmdBuffer, VkPipelineLayout pipelineLayout);
    U0 destroy(Context::It* ctx, UniformBuffer::It* uniformBuffer);
}
