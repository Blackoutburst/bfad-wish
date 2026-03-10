#include <stdlib.h>
#include "renderer/framebuffer.hh"
#include "renderer/swapChain.hh"
#include "devices/devices.hh"

namespace Framebuffer {
    U0 destroy(Device::It* device, VkSwapchainKHR swapchain, VkFramebuffer* framebuffers) {
        U32 bufferCount = SwapChain::getImagesCount(device, swapchain);

        for (U32 i = 0; i < bufferCount; i++) {
            vkDestroyFramebuffer(device->logical, framebuffers[i], NULL);
        }
    }

    VkFramebuffer* create(GLFWwindow* window, Device::It* device, VkSwapchainKHR swapchain, VkRenderPass renderPass, VkImageView* imageView, VkSurfaceKHR windowSurface) {
        U32 imageCount = SwapChain::getImagesCount(device, swapchain);
        VkFramebuffer* framebuffers = (VkFramebuffer*)malloc(sizeof(VkFramebuffer) * imageCount);
        VkExtent2D extends = SwapChain::extend(window, device, windowSurface);

        for (U32 i = 0; i < imageCount; i++) {
            VkImageView attachments[1] = { imageView[i] };

            VkFramebufferCreateInfo createInfo;
            createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.pNext = NULL;
            createInfo.flags = 0;
            createInfo.renderPass = renderPass;
            createInfo.attachmentCount = 1;
            createInfo.pAttachments = attachments;
            createInfo.width = extends.width;
            createInfo.height = extends.height;
            createInfo.layers = 1;

            vkCreateFramebuffer(device->logical, &createInfo, NULL, &framebuffers[i]);
        }

        return framebuffers;
    }
}
