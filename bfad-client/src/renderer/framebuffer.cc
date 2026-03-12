#include <stdlib.h>
#include "renderer/framebuffer.hh"
#include "renderer/swapchain.hh"
#include "devices/devices.hh"

namespace Framebuffer {
    U0 destroy(Context::It* ctx, VkSwapchainKHR swapchain, VkFramebuffer* framebuffers) {
        U32 bufferCount = Swapchain::getImagesCount(ctx, swapchain);

        for (U32 i = 0; i < bufferCount; i++) {
            vkDestroyFramebuffer(ctx->device->logical, framebuffers[i], NULL);
        }
    }

    VkFramebuffer* create(Context::It* ctx, VkSwapchainKHR swapchain, VkRenderPass renderPass, ImageView::It* imageView) {
        U32 imageCount = Swapchain::getImagesCount(ctx, swapchain);
        VkFramebuffer* framebuffers = (VkFramebuffer*)malloc(sizeof(VkFramebuffer) * imageCount);
        VkExtent2D extends = Swapchain::extend(ctx);

        for (U32 i = 0; i < imageCount; i++) {
            VkImageView attachments[2] = { imageView->handle[i], imageView->depthImageView };

            VkFramebufferCreateInfo createInfo;
            createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.pNext = NULL;
            createInfo.flags = 0;
            createInfo.renderPass = renderPass;
            createInfo.attachmentCount = 2;
            createInfo.pAttachments = attachments;
            createInfo.width = extends.width;
            createInfo.height = extends.height;
            createInfo.layers = 1;

            vkCreateFramebuffer(ctx->device->logical, &createInfo, NULL, &framebuffers[i]);
        }

        return framebuffers;
    }
}
