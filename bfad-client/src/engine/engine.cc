#include <stdlib.h>

#include "engine/math/math.hh"
#include "engine/engine.hh"

namespace Engine {

    U8 begin(Engine::It* engine) {
        Window::update();
        if (Window::isMinimized()) return 0;

        Fence::wait(engine->ctx, engine->drawFence);

        VkResult acquireResult = vkAcquireNextImageKHR(engine->ctx->device->logical, engine->renderSystem->swapchain, UINT64_MAX, engine->renderSystem->imageView->pSemaphore[engine->renderSystem->imageView->currentFrame], VK_NULL_HANDLE, &engine->renderSystem->imageView->imageIndex);

        if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR) {
            RenderSystem::update(engine->ctx, engine->renderSystem);
            VkExtent2D ext = Swapchain::extend(engine->ctx);
            Matrix::projection(engine->projection, ext.width, ext.height, 90, 0.1, 1000);
            Matrix::ortho2D(engine->projection2d, 0.0f, ext.width, 0.0f, ext.height, -1.0f, 1.0f);

            return 0;
        }

        Fence::reset(engine->ctx, engine->drawFence);

        CommandBuffer::begin(engine->cmdBuffer);
        RenderPass::begin(engine->renderSystem->renderPass, engine->renderSystem->framebuffers, engine->cmdBuffer, engine->renderSystem->imageView->imageIndex);

        Swapchain::setViewport(engine->ctx, engine->cmdBuffer);
    
        return 1;
    }

    U0 end(Engine::It* engine) {
        RenderPass::end(engine->cmdBuffer);
        CommandBuffer::end(engine->cmdBuffer);

        RenderSystem::submit(engine->ctx, engine->renderSystem, engine->cmdBuffer, engine->drawFence);
        VkResult presentResult = RenderSystem::present(engine->ctx, engine->renderSystem);

        if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR) {
            RenderSystem::update(engine->ctx, engine->renderSystem);
            VkExtent2D ext = Swapchain::extend(engine->ctx);
            Matrix::projection(engine->projection, ext.width, ext.height, 90, 0.1, 1000);
            Matrix::ortho2D(engine->projection2d, 0.0f, ext.width, 0.0f, ext.height, -1.0f, 1.0f);
        }
    }

    Engine::It* create(GLFWwindow* window) {
        Engine::It* engine = (Engine::It*)malloc(sizeof(Engine::It));
    
        engine->ctx = Context::create(window);
        engine->renderSystem = RenderSystem::create(engine->ctx);
        engine->cmdBuffer = CommandBuffer::create(engine->ctx);
        engine->drawFence = Fence::create(engine->ctx);
        engine->view = Matrix::create();
        engine->projection = Matrix::create();
        engine->projection2d = Matrix::create();
        
        VkExtent2D ext = Swapchain::extend(engine->ctx);
        Matrix::projection(engine->projection, ext.width, ext.height, 90, 0.1, 1000);
        Matrix::ortho2D(engine->projection2d, 0.0f, ext.width, 0.0f, ext.height, -1.0f, 1.0f);
        Matrix::translate3d(engine->view, 0.0f, 0.0f, 0.0f);

        return engine;
    }

    U0 destroy(Engine::It* engine) {
        vkDeviceWaitIdle(engine->ctx->device->logical);

        Matrix::destroy(engine->view);
        Matrix::destroy(engine->projection);
        Matrix::destroy(engine->projection2d);

        Fence::destroy(engine->ctx, engine->drawFence);

        CommandBuffer::destroy(engine->ctx, engine->cmdBuffer);
        
        RenderSystem::destroy(engine->ctx, engine->renderSystem);
        Context::destroy(engine->ctx);

        free(engine);
    }
}

