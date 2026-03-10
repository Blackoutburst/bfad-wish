#include "renderer/pipeline.hh"
#include "renderer/swapChain.hh"

namespace Pipeline {
    VkPipelineShaderStageCreateInfo createShader(VkShaderStageFlagBits stage, VkShaderModule shader) {
        VkPipelineShaderStageCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.stage = stage;
        createInfo.module = shader;
        createInfo.pName = "main";
        createInfo.pSpecializationInfo = NULL;

        return createInfo;
    }

    VkPipelineDynamicStateCreateInfo createDynamicState(U0) {
        VkDynamicState dynamicStates[2] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        }; 

        VkPipelineDynamicStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.dynamicStateCount = 2;
        createInfo.pDynamicStates = dynamicStates;

        return createInfo;
    }

    VkPipelineVertexInputStateCreateInfo createVertexInput(U0) {
        VkPipelineVertexInputStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.vertexBindingDescriptionCount = 0;
        createInfo.pVertexBindingDescriptions = NULL;
        createInfo.vertexAttributeDescriptionCount = 0;
        createInfo.pVertexAttributeDescriptions = NULL;

        return createInfo;
    }

    VkPipelineInputAssemblyStateCreateInfo createInputAssembly(U0) {
        VkPipelineInputAssemblyStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        createInfo.primitiveRestartEnable = VK_FALSE;

        return createInfo;
    }

    VkViewport createViewport(GLFWwindow* window, Device::It* device, VkSurfaceKHR windowSurface) {
        VkExtent2D extend = SwapChain::extend(window, device, windowSurface);

        VkViewport viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (F32) extend.width;
        viewport.height = (F32) extend.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        return viewport;
    }

    VkRect2D createScissor(GLFWwindow* window, Device::It* device, VkSurfaceKHR windowSurface) {
        VkExtent2D extend = SwapChain::extend(window, device, windowSurface);

        VkRect2D scissor;
        scissor.offset = {0, 0};
        scissor.extent = extend;

        return scissor;
    }

    VkPipelineViewportStateCreateInfo createViewportState(VkViewport viewport, VkRect2D scissor) {
        VkPipelineViewportStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.viewportCount = 1;
        createInfo.pViewports = &viewport;
        createInfo.scissorCount = 1;
        createInfo.pScissors = &scissor;

        return createInfo;
    }

    VkPipelineRasterizationStateCreateInfo createRasterizerState(U0) {
        VkPipelineRasterizationStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.depthClampEnable = VK_FALSE;
        createInfo.rasterizerDiscardEnable = VK_FALSE;
        createInfo.polygonMode = VK_POLYGON_MODE_FILL;
        createInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        createInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        createInfo.depthBiasEnable = VK_FALSE;
        createInfo.depthBiasConstantFactor = 0.0f;
        createInfo.depthBiasClamp = 0.0f;
        createInfo.depthBiasSlopeFactor = 0.0f;
        createInfo.lineWidth = 1.0f;

        return createInfo;
    }

    VkPipelineMultisampleStateCreateInfo createMultisamplingState(U0) {
        VkPipelineMultisampleStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.sampleShadingEnable = VK_FALSE;
        createInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        createInfo.minSampleShading = 1.0f;
        createInfo.pSampleMask = NULL;
        createInfo.alphaToCoverageEnable = VK_FALSE;
        createInfo.alphaToOneEnable = VK_FALSE;

        return createInfo;
    }

    VkPipelineColorBlendAttachmentState createColorBlendAttachmentState(U0) {
        VkPipelineColorBlendAttachmentState createInfo;
        createInfo.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        createInfo.blendEnable = VK_FALSE;
        createInfo.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        createInfo.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        createInfo.colorBlendOp = VK_BLEND_OP_ADD;
        createInfo.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        createInfo.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        createInfo.alphaBlendOp = VK_BLEND_OP_ADD;

        return createInfo;
    }

    VkPipelineColorBlendStateCreateInfo createColorBlendState(VkPipelineColorBlendAttachmentState colorBlendAttachment) {
        VkPipelineColorBlendStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.logicOpEnable = VK_FALSE;
        createInfo.logicOp = VK_LOGIC_OP_COPY;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = &colorBlendAttachment;
        createInfo.blendConstants[0] = 0.0f;
        createInfo.blendConstants[1] = 0.0f;
        createInfo.blendConstants[2] = 0.0f;
        createInfo.blendConstants[3] = 0.0f;

        return createInfo;
    }

    U0 destroy(Device::It* device, VkPipeline graphicsPipeline) {
        vkDestroyPipeline(device->logical, graphicsPipeline, NULL);
    }

    VkPipeline create(GLFWwindow* window, Device::It* device, VkPipelineLayout layout, VkShaderModule vertexShader, VkShaderModule fragmentShader, VkRenderPass renderPass, VkSurfaceKHR windowSurface) {
        UNUSED_VAR(layout);
        
        VkPipelineShaderStageCreateInfo vertexInfo = createShader(VK_SHADER_STAGE_VERTEX_BIT, vertexShader);
        VkPipelineShaderStageCreateInfo fragmentInfo = createShader(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader);
        VkPipelineShaderStageCreateInfo shaderInfo[2] = {vertexInfo, fragmentInfo};

        VkPipelineVertexInputStateCreateInfo vertexInput = createVertexInput();
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = createInputAssembly();
        VkPipelineViewportStateCreateInfo viewport = createViewportState(createViewport(window, device, windowSurface), createScissor(window, device, windowSurface));
        VkPipelineRasterizationStateCreateInfo rasterizer = createRasterizerState();
        VkPipelineMultisampleStateCreateInfo multiSampling = createMultisamplingState();
        VkPipelineColorBlendStateCreateInfo colorBlend =  createColorBlendState(createColorBlendAttachmentState());
        VkPipelineDynamicStateCreateInfo dynamicState = createDynamicState();

        VkGraphicsPipelineCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.pNext = NULL;
        createInfo.stageCount = 2;
        createInfo.pStages = shaderInfo;
        createInfo.pVertexInputState = &vertexInput;
        createInfo.pInputAssemblyState = &inputAssembly;
        createInfo.pTessellationState = NULL;
        createInfo.pViewportState = &viewport;
        createInfo.pRasterizationState = &rasterizer;
        createInfo.pMultisampleState = &multiSampling;
        createInfo.pDepthStencilState = NULL;
        createInfo.pColorBlendState = &colorBlend;
        createInfo.pDynamicState = &dynamicState;
        createInfo.layout = layout;
        createInfo.renderPass = renderPass;
        createInfo.subpass = 0;
        createInfo.basePipelineHandle = VK_NULL_HANDLE;
        createInfo.basePipelineIndex = -1;

        VkPipeline graphicsPipeline;
        vkCreateGraphicsPipelines(device->logical, VK_NULL_HANDLE, 1, &createInfo, NULL, &graphicsPipeline);

        return graphicsPipeline;
    }

    namespace Layout {
        U0 remove(Device::It* device, VkPipelineLayout layout) {
            vkDestroyPipelineLayout(device->logical, layout, NULL);
        }

        VkPipelineLayout create(Device::It* device) {
            VkPipelineLayout pipelineLayout;
            
            VkPipelineLayoutCreateInfo createInfo;
            createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            createInfo.pNext = NULL;
            createInfo.flags = 0;
            createInfo.setLayoutCount = 0;
            createInfo.pSetLayouts = NULL;
            createInfo.pushConstantRangeCount = 0;
            createInfo.pPushConstantRanges = NULL;
            
            vkCreatePipelineLayout(device->logical, &createInfo, NULL, &pipelineLayout);

            return pipelineLayout;
        }

        U0 destroy(Device::It* device, VkPipelineLayout pipelineLayout) {
            vkDestroyPipelineLayout(device->logical, pipelineLayout, NULL);
        }
    }
}
