#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "types.hh"
#include "context.hh"

#include "renderer/shaderProgram.hh"

namespace Pipeline {
    VkPipelineDynamicStateCreateInfo createDynamicState(U0);
    VkPipelineVertexInputStateCreateInfo createVertexInput(U0);
    VkPipelineInputAssemblyStateCreateInfo createInputAssembly(U0);
    VkViewport createViewport(Context::It* ctx);
    VkRect2D createScissor(Context::It* ctx);
    VkPipelineViewportStateCreateInfo createViewportState(VkViewport* viewport, VkRect2D* scissor);
    VkPipelineRasterizationStateCreateInfo createRasterizerState(U0);
    VkPipelineMultisampleStateCreateInfo createMultisamplingState(U0);
    VkPipelineColorBlendAttachmentState createColorBlendAttachmentState(U0);
    VkPipelineColorBlendStateCreateInfo createColorBlendState(VkPipelineColorBlendAttachmentState* colorBlendAttachment);
    U0 destroy(Context::It* ctx, VkPipeline graphicsPipeline);
    VkPipeline create(Context::It* ctx, VkPipelineLayout layout, ShaderProgram::It* shaderProgram, VkRenderPass renderPass);
    VkPipelineDepthStencilStateCreateInfo createDepthStencil(U0);

    namespace Layout {
        U0 remove(Context::It* ctx, VkPipelineLayout layout);
        U0 destroy(Context::It* ctx, VkPipelineLayout pipelineLayout);
        VkPipelineLayout create(Context::It* ctx, U32 count, VkDescriptorSetLayout setLayout);
    }
}
