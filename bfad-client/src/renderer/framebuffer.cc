#include <stdlib.h>

#include "renderer/framebuffer.hh"
#include "renderer/swapchain.hh"
#include "devices/devices.hh"

namespace Framebuffer {
    U0 destroy(Context::It* ctx, Framebuffer::It* framebuffers) {
        for (U32 i = 0; i < framebuffers->bufferCount; i++) {
            vkDestroyFramebuffer(ctx->device->logical, framebuffers->handles[i], NULL);
        }
    }

    Framebuffer::It* create(Context::It* ctx, VkSwapchainKHR swapchain, VkRenderPass renderPass, ImageView::It* imageView) {
        Framebuffer::It* parent = (Framebuffer::It*)malloc(sizeof(Framebuffer::It));
        
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

        parent->handles = framebuffers;
        parent->extends = extends;
        parent->bufferCount = imageCount;

        return parent;
    }
}
