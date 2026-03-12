#include <stdlib.h>

#include "renderSystem.hh"

#include "renderer/framebuffer.hh"
#include "renderer/renderPass.hh"

namespace RenderSystem {
    RenderSystem::It* create(Context::It* ctx) {
        VkSwapchainKHR swapchain = Swapchain::create(ctx);
        ImageView::It* imageView = ImageView::create(ctx, swapchain);
        VkRenderPass renderPass = RenderPass::create(ctx);
        VkFramebuffer* framebuffers = Framebuffer::create(ctx, swapchain, renderPass, imageView);
    
        RenderSystem::It* renderSystem = (RenderSystem::It*)malloc(sizeof(RenderSystem::It));

        renderSystem->swapchain = swapchain;
        renderSystem->imageView = imageView;
        renderSystem->framebuffers = framebuffers;
        renderSystem->renderPass = renderPass;

        return renderSystem;
    }
    
    U0 destroy(Context::It* ctx, RenderSystem::It* renderSystem) {
        Framebuffer::destroy(ctx, renderSystem->swapchain, renderSystem->framebuffers);
        RenderPass::destroy(ctx, renderSystem->renderPass);
        ImageView::destroy(ctx, renderSystem->imageView);
        Swapchain::destroy(ctx, renderSystem->swapchain);
    
        free(renderSystem);
    }

    U0 submit(Context::It* ctx, RenderSystem::It* renderSystem, VkCommandBuffer cmdBuffer, VkFence fence) {
        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submitInfo;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = NULL;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &renderSystem->imageView->pSemaphore[renderSystem->imageView->currentFrame];
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmdBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &renderSystem->imageView->rSemaphore[renderSystem->imageView->currentFrame];

        vkQueueSubmit(ctx->device->graphicQueue, 1, &submitInfo, fence);
    }

    U0 present(Context::It* ctx, RenderSystem::It* renderSystem) {
        VkPresentInfoKHR presentInfo;
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = NULL;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderSystem->imageView->rSemaphore[renderSystem->imageView->currentFrame];
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &renderSystem->swapchain;
        presentInfo.pImageIndices = &renderSystem->imageView->imageIndex;
        presentInfo.pResults = NULL;

        vkQueuePresentKHR(ctx->device->presentQueue, &presentInfo);

        renderSystem->imageView->currentFrame = (renderSystem->imageView->currentFrame + 1) % renderSystem->imageView->swapChainImagesCount;
    }

    U0 update(Context::It* ctx, RenderSystem::It* renderSystem) {
        vkDeviceWaitIdle(ctx->device->logical);

        Framebuffer::destroy(ctx, renderSystem->swapchain, renderSystem->framebuffers);
        ImageView::destroy(ctx, renderSystem->imageView);
        Swapchain::destroy(ctx, renderSystem->swapchain);

        VkSwapchainKHR swapchain = Swapchain::create(ctx);
        ImageView::It* imageView = ImageView::create(ctx, swapchain);
        VkFramebuffer* framebuffers = Framebuffer::create(ctx, swapchain, renderSystem->renderPass, imageView);

        renderSystem->swapchain = swapchain;
        renderSystem->imageView = imageView;
        renderSystem->framebuffers = framebuffers;
    }
}
