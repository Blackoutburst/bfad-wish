#include "renderer/shader.hh"
#include "utils/io.hh"

namespace Shader {
    U0 destroy(Context::It* ctx, VkShaderModule shader) {
        vkDestroyShaderModule(ctx->device->logical, shader, NULL);
    }

    VkShaderModule create(Context::It* ctx, const I8* filePath) {
        FILE* file = File::open(filePath);
        U32 codeSize = File::size(file);
        U32* code = (U32*)File::read(file);

        VkShaderModuleCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.codeSize = codeSize;
        createInfo.pCode = code;

        VkShaderModule shaderModule;
        vkCreateShaderModule(ctx->device->logical, &createInfo, NULL, &shaderModule);

        return shaderModule;
    }
}
