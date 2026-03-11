#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "types.hh"
#include "devices/devices.hh"

namespace Pipeline {
    VkPipelineShaderStageCreateInfo createShader(VkShaderStageFlagBits stage, VkShaderModule shader);
    VkPipelineDynamicStateCreateInfo createDynamicState(U0);
    VkPipelineVertexInputStateCreateInfo createVertexInput(U0);
    VkPipelineInputAssemblyStateCreateInfo createInputAssembly(U0);
    VkViewport createViewport(GLFWwindow* window, Device::It* device, VkSurfaceKHR windowSurface);
    VkRect2D createScissor(GLFWwindow* window, Device::It* device, VkSurfaceKHR windowSurface);
    VkPipelineViewportStateCreateInfo createViewportState(VkViewport* viewport, VkRect2D* scissor);
    VkPipelineRasterizationStateCreateInfo createRasterizerState(U0);
    VkPipelineMultisampleStateCreateInfo createMultisamplingState(U0);
    VkPipelineColorBlendAttachmentState createColorBlendAttachmentState(U0);
    VkPipelineColorBlendStateCreateInfo createColorBlendState(VkPipelineColorBlendAttachmentState* colorBlendAttachment);
    U0 destroy(Device::It* device, VkPipeline graphicsPipeline);
    VkPipeline create(GLFWwindow* window, Device::It* device, VkPipelineLayout layout, VkShaderModule vertexShader, VkShaderModule fragmentShader, VkRenderPass renderPass, VkSurfaceKHR windowSurface);

    namespace Layout {
        U0 remove(Device::It* device, VkPipelineLayout layout);
        U0 destroy(Device::It* device, VkPipelineLayout pipelineLayout);
        VkPipelineLayout create(Device::It* device);
    }
}
