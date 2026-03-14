#pragma once

#include <vulkan/vulkan.h>

#include "engine/types.hh"
#include "engine/context.hh"
#include "engine/renderer/shaderProgram.hh"
#include "engine/texture/texture.hh"
#include "engine/utils/uniformBuffer.hh"

namespace VertexArray {
    struct Attribute {
        U32 location;
        VkFormat format;
        U32 offset;
    };

    struct Description {
        U32 stride;
        Attribute* attributes;
        U32 attributeCount;
    };

    struct It {
        VkPipelineLayout layout;
        VkPipeline pipeline;
        UniformBuffer::It* uniformBuffer;
    };

    VertexArray::It* create(Context::It* ctx, Description* desc, UniformBuffer::It* uniformBuffer, Texture::It* texture, ShaderProgram::It* shaderProgram, VkRenderPass renderPass);
    U0 bind(VertexArray::It* vao, VkCommandBuffer cmdBuffer);
    U0 destroy(Context::It* ctx, VertexArray::It* vao);
}
