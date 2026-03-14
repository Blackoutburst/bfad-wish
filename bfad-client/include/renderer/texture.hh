#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

namespace Texture {
    struct It {
        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;
        VkSampler sampler;
        VkDescriptorSetLayout setLayout;
        VkDescriptorSet descriptorSet;
    };

    Texture::It* create(Context::It* ctx, const I8* path);
    U0 bind(Texture::It* texture, VkCommandBuffer cmdBuffer, VkPipelineLayout pipelineLayout);
    U0 destroy(Context::It* ctx, Texture::It* texture);
}
