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
