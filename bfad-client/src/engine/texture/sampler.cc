#include "engine/texture/sampler.hh"

namespace Sampler {
    VkSampler create(Context::It* ctx) {
        VkSampler sampler;

        VkSamplerCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.magFilter = VK_FILTER_NEAREST;
        createInfo.minFilter = VK_FILTER_NEAREST;
        createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        createInfo.mipLodBias = 0.0f;
        createInfo.anisotropyEnable = VK_TRUE;
        createInfo.maxAnisotropy = ctx->device->properties.limits.maxSamplerAnisotropy;
        createInfo.compareEnable = VK_FALSE;
        createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        createInfo.minLod = 0.0f;
        createInfo.maxLod = 0.0f;
        createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        createInfo.unnormalizedCoordinates = VK_FALSE;

        vkCreateSampler(ctx->device->logical, &createInfo, NULL, &sampler);

        return sampler;
    }

    U0 destroy(Context::It* ctx, VkSampler sampler) {
        vkDestroySampler(ctx->device->logical, sampler, NULL);
    }
}
