#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "renderer/texture.hh"
#include "renderer/imageView.hh"
#include "renderer/imageLayout.hh"
#include "renderer/sampler.hh"
#include "utils/buffer.hh"
#include "devices/commandBuffer.hh"

static U0 submitOnce(Context::It* ctx, VkCommandBuffer cmdBuffer) {
    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = NULL;
    submitInfo.pWaitDstStageMask = NULL;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = NULL;

    vkQueueSubmit(ctx->device->graphicQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(ctx->device->graphicQueue);
}

namespace Texture {
    Texture::It* create(Context::It* ctx, const I8* path) {
        I32 width, height, channels;
        U8* pixels = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
        U32 imageSize = (U32)(width * height * 4);

        Buffer::It* staging = Buffer::create(ctx, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        Buffer::upload(ctx, staging, pixels, imageSize);
        stbi_image_free(pixels);

        VkImage image;
        VkDeviceMemory memory;

        VkImageCreateInfo imageInfo;
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.pNext = NULL;
        imageInfo.flags = 0;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        imageInfo.extent = { (U32)width, (U32)height, 1 };
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.queueFamilyIndexCount = 0;
        imageInfo.pQueueFamilyIndices = NULL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        vkCreateImage(ctx->device->logical, &imageInfo, NULL, &image);

        VkMemoryRequirements memReqs;
        vkGetImageMemoryRequirements(ctx->device->logical, image, &memReqs);

        VkMemoryAllocateInfo allocInfo;
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = NULL;
        allocInfo.allocationSize = memReqs.size;
        allocInfo.memoryTypeIndex = Buffer::findMemoryType(ctx, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkAllocateMemory(ctx->device->logical, &allocInfo, NULL, &memory);
        vkBindImageMemory(ctx->device->logical, image, memory, 0);

        VkCommandBuffer cmd = CommandBuffer::create(ctx);
        CommandBuffer::begin(cmd);

        ImageLayout::transition(cmd, image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        VkBufferImageCopy region;
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { (U32)width, (U32)height, 1 };
        vkCmdCopyBufferToImage(cmd, staging->handle, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        ImageLayout::transition(cmd, image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        CommandBuffer::end(cmd);
        submitOnce(ctx, cmd);
        CommandBuffer::destroy(ctx, cmd);

        Buffer::destroy(ctx, staging);

        VkImageView view = ImageView::makeView(ctx, image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
        VkSampler sampler = Sampler::create(ctx);

        VkDescriptorSetLayoutBinding samplerBinding;
        samplerBinding.binding = 0;
        samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerBinding.descriptorCount = 1;
        samplerBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        samplerBinding.pImmutableSamplers = NULL;

        VkDescriptorSetLayoutCreateInfo layoutInfo;
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.pNext = NULL;
        layoutInfo.flags = 0;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &samplerBinding;

        VkDescriptorSetLayout setLayout;
        vkCreateDescriptorSetLayout(ctx->device->logical, &layoutInfo, NULL, &setLayout);

        VkDescriptorSetAllocateInfo descAllocInfo;
        descAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descAllocInfo.pNext = NULL;
        descAllocInfo.descriptorPool = ctx->descriptorPool;
        descAllocInfo.descriptorSetCount = 1;
        descAllocInfo.pSetLayouts = &setLayout;

        VkDescriptorSet descriptorSet;
        vkAllocateDescriptorSets(ctx->device->logical, &descAllocInfo, &descriptorSet);

        VkDescriptorImageInfo imageDescInfo;
        imageDescInfo.sampler = sampler;
        imageDescInfo.imageView = view;
        imageDescInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkWriteDescriptorSet write;
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.pNext = NULL;
        write.dstSet = descriptorSet;
        write.dstBinding = 0;
        write.dstArrayElement = 0;
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.pImageInfo = &imageDescInfo;
        write.pBufferInfo = NULL;
        write.pTexelBufferView = NULL;
        vkUpdateDescriptorSets(ctx->device->logical, 1, &write, 0, NULL);

        Texture::It* texture = (Texture::It*)malloc(sizeof(Texture::It));
        texture->image = image;
        texture->memory = memory;
        texture->view = view;
        texture->sampler = sampler;
        texture->setLayout = setLayout;
        texture->descriptorSet = descriptorSet;

        return texture;
    }

    U0 bind(Texture::It* texture, VkCommandBuffer cmdBuffer, VkPipelineLayout pipelineLayout) {
        vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 1, 1, &texture->descriptorSet, 0, NULL);
    }

    U0 destroy(Context::It* ctx, Texture::It* texture) {
        vkDestroyDescriptorSetLayout(ctx->device->logical, texture->setLayout, NULL);
        Sampler::destroy(ctx, texture->sampler);
        vkDestroyImageView(ctx->device->logical, texture->view, NULL);
        vkDestroyImage(ctx->device->logical, texture->image, NULL);
        vkFreeMemory(ctx->device->logical, texture->memory, NULL);
        free(texture);
    }
}
