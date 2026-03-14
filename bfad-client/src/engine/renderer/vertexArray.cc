#include <stdlib.h>

#include "engine/renderer/vertexArray.hh"
#include "engine/renderer/pipeline.hh"

namespace VertexArray {

    VertexArray::It* create(Context::It* ctx, Description* desc, UniformBuffer::It* uniformBuffer, Texture::It* texture, ShaderProgram::It* shaderProgram, VkRenderPass renderPass) {
        VkVertexInputBindingDescription bindingDesc;
        bindingDesc.binding = 0;
        bindingDesc.stride = desc->stride;
        bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        VkVertexInputAttributeDescription* attrDescs = (VkVertexInputAttributeDescription*)malloc(sizeof(VkVertexInputAttributeDescription) * desc->attributeCount);
        for (U32 i = 0; i < desc->attributeCount; i++) {
            attrDescs[i].location = desc->attributes[i].location;
            attrDescs[i].binding = 0;
            attrDescs[i].format = desc->attributes[i].format;
            attrDescs[i].offset = desc->attributes[i].offset;
        }

        VkPipelineVertexInputStateCreateInfo vertexInput;
        vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInput.pNext = NULL;
        vertexInput.flags = 0;
        vertexInput.vertexBindingDescriptionCount = 1;
        vertexInput.pVertexBindingDescriptions = &bindingDesc;
        vertexInput.vertexAttributeDescriptionCount = desc->attributeCount;
        vertexInput.pVertexAttributeDescriptions = attrDescs;

        VkPipelineLayout layout;
        if (texture != NULL) {
            VkDescriptorSetLayout setLayouts[2] = { uniformBuffer->setLayout, texture->setLayout };
            layout = Pipeline::Layout::create(ctx, 2, setLayouts);
        } else {
            VkDescriptorSetLayout setLayouts[1] = { uniformBuffer->setLayout };
            layout = Pipeline::Layout::create(ctx, 1, setLayouts);
        }

        VkPipeline pipeline = Pipeline::create(ctx, layout, shaderProgram, renderPass, &vertexInput);

        free(attrDescs);

        VertexArray::It* vao = (VertexArray::It*)malloc(sizeof(VertexArray::It));
        vao->layout = layout;
        vao->pipeline = pipeline;
        vao->uniformBuffer = uniformBuffer;
        vao->texture = texture;

        return vao;
    }

    U0 bind(VertexArray::It* vao, VkCommandBuffer cmdBuffer) {
        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vao->pipeline);
        UniformBuffer::bind(vao->uniformBuffer, cmdBuffer, vao->layout);
        if (vao->texture != NULL) {
            Texture::bind(vao->texture, cmdBuffer, vao->layout);
        }
    }

    U0 destroy(Context::It* ctx, VertexArray::It* vao) {
        Pipeline::Layout::destroy(ctx, vao->layout);
        Pipeline::destroy(ctx, vao->pipeline);
        free(vao);
    }
}
