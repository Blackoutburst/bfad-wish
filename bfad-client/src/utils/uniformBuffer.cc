#include <stdlib.h>
#include <string.h>

#include "utils/uniformBuffer.hh"

namespace UniformBuffer {

    UniformBuffer::It* create(Context::It* ctx, U32 binding, U32 size) {
        VkDescriptorSetLayoutBinding layout;
        layout.binding = binding;
        layout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layout.descriptorCount = 1;
        layout.stageFlags = VK_SHADER_STAGE_ALL;
        layout.pImmutableSamplers = NULL;

        VkDescriptorSetLayoutCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.bindingCount = 1;
        createInfo.pBindings = &layout;

        VkDescriptorSetLayout setLayout;
        vkCreateDescriptorSetLayout(ctx->device->logical, &createInfo, NULL, &setLayout);

        Buffer::It* buffer = Buffer::create(ctx, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

        UniformBuffer::It* uniformBuffer = (UniformBuffer::It*)malloc(sizeof(UniformBuffer::It));
        uniformBuffer->buffer = buffer;
        uniformBuffer->setLayout = setLayout;
        
        vkMapMemory(ctx->device->logical, buffer->memory, 0, size, 0, &uniformBuffer->mappedData);

        return uniformBuffer;
    }

    U0 update(UniformBuffer::It* uniformBuffer, U0* data, U32 size) {
        memcpy(uniformBuffer->mappedData, data, size);
    }
    
    U0 destroy(Context::It* ctx, UniformBuffer::It* uniformBuffer) {
        vkUnmapMemory(ctx->device->logical, uniformBuffer->buffer->memory);
        vkDestroyDescriptorSetLayout(ctx->device->logical, uniformBuffer->setLayout, NULL);
        Buffer::destroy(ctx, uniformBuffer->buffer);
        free(uniformBuffer);
    }
}
