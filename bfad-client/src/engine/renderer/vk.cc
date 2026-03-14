#include <stdlib.h>

#include "object/cube.hh"
#include "object/quad.hh"

#include "engine/context.hh"
#include "engine/renderer/renderSystem.hh"
#include "engine/math/matrix.hh"
#include "engine/utils/fence.hh"
#include "engine/device/commandBuffer.hh"
#include "engine/window/window.hh"
#include "engine/renderer/renderPass.hh"


static Context::It* ctx;
static RenderSystem::It* renderSystem;
static VkCommandBuffer cmdBuffer;
static VkFence drawFence;

static Cube::It* cube;
static Quad::It* quad;

static Matrix::It* view;
static Matrix::It* projection;
static Matrix::It* projection2d;

Matrix::It* getViewMatrix(U0) {
    return view;
}

Matrix::It* getProjectionMatrix(U0) {
    return projection;
}

Matrix::It* getProjection2dMatrix(U0) {
    return projection2d;
}


U0 vkDrawTriangle(U0) {
    if (Window::isMinimized()) return;

    Fence::wait(ctx, drawFence);

    VkResult acquireResult = vkAcquireNextImageKHR(ctx->device->logical, renderSystem->swapchain, UINT64_MAX, renderSystem->imageView->pSemaphore[renderSystem->imageView->currentFrame], VK_NULL_HANDLE, &renderSystem->imageView->imageIndex);

    if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR) {
        RenderSystem::update(ctx, renderSystem);
        VkExtent2D ext = Swapchain::extend(ctx);
        Matrix::projection(projection, ext.width, ext.height, 90, 0.1, 1000);
        Matrix::ortho2D(projection2d, 0.0f, ext.width, 0.0f, ext.height, -1.0f, 1.0f);

        return;
    }

    Fence::reset(ctx, drawFence);

    CommandBuffer::begin(cmdBuffer);
    RenderPass::begin(renderSystem->renderPass, renderSystem->framebuffers, cmdBuffer, renderSystem->imageView->imageIndex);

    Swapchain::setViewport(ctx, cmdBuffer);

    Cube::render(cube, cmdBuffer);
    //Quad::render(quad, cmdBuffer);

    RenderPass::end(cmdBuffer);
    CommandBuffer::end(cmdBuffer);

    RenderSystem::submit(ctx, renderSystem, cmdBuffer, drawFence);
    VkResult presentResult = RenderSystem::present(ctx, renderSystem);

    if (acquireResult == VK_SUBOPTIMAL_KHR || presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR) {
        RenderSystem::update(ctx, renderSystem);
        VkExtent2D ext = Swapchain::extend(ctx);
        Matrix::projection(projection, ext.width, ext.height, 90, 0.1, 1000);
        Matrix::ortho2D(projection2d, 0.0f, ext.width, 0.0f, ext.height, -1.0f, 1.0f);
    }
}

U0 vkInit(GLFWwindow* window) {
    ctx = Context::create(window);
    renderSystem = RenderSystem::create(ctx);

    cmdBuffer = CommandBuffer::create(ctx);

    drawFence = Fence::create(ctx);

    view = Matrix::create();
    projection = Matrix::create();
    projection2d = Matrix::create();
    VkExtent2D ext = Swapchain::extend(ctx);
    Matrix::projection(projection, ext.width, ext.height, 90, 0.1, 1000);
    Matrix::ortho2D(projection2d, 0.0f, ext.width, 0.0f, ext.height, -1.0f, 1.0f);
    Matrix::translate3d(view, 0.0f, 0.0f, -4.0f);

    cube = Cube::create(ctx, renderSystem);
    quad = Quad::create(ctx, renderSystem);
}

U0 vkClean(U0) {
    vkDeviceWaitIdle(ctx->device->logical);

    Cube::destroy(ctx, cube);
    Quad::destroy(ctx, quad);
    
    Matrix::destroy(view);
    Matrix::destroy(projection);
    Matrix::destroy(projection2d);

    Fence::destroy(ctx, drawFence);

    CommandBuffer::destroy(ctx, cmdBuffer);
    
    RenderSystem::destroy(ctx, renderSystem);
    Context::destroy(ctx);
}
