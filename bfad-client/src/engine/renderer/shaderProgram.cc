#include <stdlib.h>

#include "engine/renderer/shaderProgram.hh"
#include "engine/renderer/shader.hh"

namespace ShaderProgram {

    VkPipelineShaderStageCreateInfo createShaderInfo(VkShaderStageFlagBits stage, VkShaderModule shader) {
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

    ShaderProgram::It* create(Context::It* ctx, const I8* vertexSource, const I8* fragmentSource) {
        VkShaderModule vertexShader = Shader::create(ctx, vertexSource);
        VkShaderModule fragmentShader = Shader::create(ctx, fragmentSource);

        VkPipelineShaderStageCreateInfo vertexInfo = createShaderInfo(VK_SHADER_STAGE_VERTEX_BIT, vertexShader);
        VkPipelineShaderStageCreateInfo fragmentInfo = createShaderInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader);

        ShaderProgram::It* shaderProgram = (ShaderProgram::It*)malloc(sizeof(ShaderProgram::It));
        shaderProgram->handle[0] = vertexInfo;
        shaderProgram->handle[1] = fragmentInfo;
        shaderProgram->vertexShader = vertexShader;
        shaderProgram->fragmentShader = fragmentShader;

        return shaderProgram;
    }

    U0 destroy(Context::It* ctx, ShaderProgram::It* shaderProgram) {
        Shader::destroy(ctx, shaderProgram->vertexShader);
        Shader::destroy(ctx, shaderProgram->fragmentShader);
    
        free(shaderProgram);
    }
}
