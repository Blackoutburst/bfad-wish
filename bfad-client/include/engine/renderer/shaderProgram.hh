#pragma once

#include <vulkan/vulkan.h>

#include "engine/types.hh"
#include "engine/context.hh"

namespace ShaderProgram {
    struct It {
        VkPipelineShaderStageCreateInfo handle[2];
        VkShaderModule vertexShader;
        VkShaderModule fragmentShader;
    };

    VkPipelineShaderStageCreateInfo createShaderInfo(VkShaderStageFlagBits stage, VkShaderModule shader);
    ShaderProgram::It* create(Context::It* ctx, const I8* vertexSource, const I8* fragmentSource);
    U0 destroy(Context::It* ctx, ShaderProgram::It* shaderProgram);
}
