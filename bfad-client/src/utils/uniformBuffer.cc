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
        uniformBuffer->binding = binding;
        uniformBuffer->size = size;
        
        vkMapMemory(ctx->device->logical, buffer->memory, 0, size, 0, &uniformBuffer->mappedData);

        VkDescriptorSetAllocateInfo allocInfo;
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.pNext = NULL;
        allocInfo.descriptorPool = ctx->descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &uniformBuffer->setLayout;

        VkDescriptorSet descriptorSet;
        vkAllocateDescriptorSets(ctx->device->logical, &allocInfo, &descriptorSet);

        VkDescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = uniformBuffer->buffer->handle;
        bufferInfo.offset = 0;
        bufferInfo.range = size;

        VkWriteDescriptorSet write;
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.pNext = NULL;
        write.dstSet = descriptorSet;
        write.dstBinding = binding;
        write.dstArrayElement = 0;
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.pBufferInfo = &bufferInfo;
        write.pImageInfo = NULL;
        write.pTexelBufferView = NULL;

        vkUpdateDescriptorSets(ctx->device->logical, 1, &write, 0, NULL);

        uniformBuffer->descriptorSet = descriptorSet;

        return uniformBuffer;
    }

    U0 bind(UniformBuffer::It* uniformBuffer, VkCommandBuffer cmdBuffer, VkPipelineLayout pipelineLayout) {
        vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &uniformBuffer->descriptorSet, 0, NULL);
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
