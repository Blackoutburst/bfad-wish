#include <stdlib.h>

#include "renderSystem.hh"

#include "renderer/vk.hh"
#include "devices/devices.hh"
#include "debug/logCallback.hh"
#include "window/window.hh"
#include "renderer/rendererInstance.hh"
#include "renderer/windowSurface.hh"
#include "utils/args.hh"
#include "utils/logger.hh"
#include "renderer/swapchain.hh"
#include "renderer/imageView.hh"
#include "renderer/shader.hh"
#include "renderer/pipeline.hh"
#include "renderer/framebuffer.hh"
#include "renderer/renderPass.hh"
#include "devices/physicalDevice.hh"
#include "devices/logicalDevice.hh"
#include "devices/commandPool.hh"
#include "devices/commandBuffer.hh"
#include "utils/semaphore.hh"
#include "utils/fence.hh"
#include "utils/buffer.hh"

static Context::It* ctx;
static RenderSystem::It* renderSystem;
static ShaderProgram::It* shaderProgram;
static VkPipelineLayout pipelineLayout;
static VkPipeline pipeline;
static VkCommandPool cmdPool;
static VkCommandBuffer cmdBuffer;
static VkFence drawFence;
static Buffer::It* vertexBuffer;
static Buffer::It* indexBuffer;

U0 vkDrawTriangle(U0) {
    Fence::wait(ctx, drawFence);

    VkResult swapchainStatus = vkAcquireNextImageKHR(ctx->device->logical, renderSystem->swapchain, UINT64_MAX, renderSystem->imageView->pSemaphore[renderSystem->imageView->currentFrame], VK_NULL_HANDLE, &renderSystem->imageView->imageIndex);
    if (swapchainStatus == VK_ERROR_OUT_OF_DATE_KHR || swapchainStatus == VK_SUBOPTIMAL_KHR) {
        RenderSystem::update(ctx, renderSystem);
        return;
    }

    Fence::reset(ctx, drawFence);

    vkResetCommandBuffer(cmdBuffer, 0);
    CommandBuffer::begin(cmdBuffer);
    RenderPass::begin(ctx, renderSystem->renderPass, renderSystem->framebuffers, cmdBuffer, renderSystem->imageView->imageIndex);
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    VkExtent2D extends = Swapchain::extend(ctx);
    
    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (F32)extends.width;
    viewport.height = (F32)extends.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

    VkRect2D scissor;
    scissor.offset = {0, 0};
    scissor.extent = extends;
    vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

    VkBuffer vertexBuffers[] = { vertexBuffer->handle };
    VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(cmdBuffer, indexBuffer->handle, 0, VK_INDEX_TYPE_UINT32);

    //vkCmdDraw(cmdBuffer, 6, 1, 0, 0);
    vkCmdDrawIndexed(cmdBuffer, 6, 1, 0, 0, 0);

    RenderPass::end(cmdBuffer);
    CommandBuffer::end(cmdBuffer);

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo;
    submitInfo.pNext = NULL;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &renderSystem->imageView->pSemaphore[renderSystem->imageView->currentFrame];
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderSystem->imageView->rSemaphore[renderSystem->imageView->currentFrame];

    vkQueueSubmit(ctx->device->graphicQueue, 1, &submitInfo, drawFence);

    VkPresentInfoKHR presentInfo;
    presentInfo.pNext = NULL;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderSystem->imageView->rSemaphore[renderSystem->imageView->currentFrame];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &renderSystem->swapchain;
    presentInfo.pImageIndices = &renderSystem->imageView->imageIndex;

    vkQueuePresentKHR(ctx->device->presentQueue, &presentInfo);

    renderSystem->imageView->currentFrame = (renderSystem->imageView->currentFrame + 1) % renderSystem->imageView->swapChainImagesCount;
}

U0 vkInit(GLFWwindow* window) {
    ctx = Context::create(window);
    renderSystem = RenderSystem::create(ctx);

    shaderProgram = ShaderProgram::create(ctx, "./shader/triangleVert.spv", "./shader/triangleFrag.spv");
    
    pipelineLayout = Pipeline::Layout::create(ctx, 0, NULL);
    pipeline = Pipeline::create(ctx, pipelineLayout, shaderProgram, renderSystem->renderPass);
    
    cmdPool = CommandPool::create(ctx);
    cmdBuffer = CommandBuffer::create(ctx, cmdPool);

    drawFence = Fence::create(ctx);

    F32 vertexData[30] = {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
    };

    U32 indexData[6] = {
        0, 1, 2, 
        2, 3, 0,
    };

    vertexBuffer = Buffer::create(ctx, sizeof(vertexData[0]) * 30, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    Buffer::upload(ctx, vertexBuffer, vertexData, sizeof(vertexData[0]) * 30);

    indexBuffer = Buffer::create(ctx, sizeof(indexData[0]) * 6, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    Buffer::upload(ctx, indexBuffer, indexData, sizeof(indexData[0]) * 6);
}

U0 vkClean(U0) {
    vkDeviceWaitIdle(ctx->device->logical);

    Buffer::destroy(ctx, vertexBuffer);
    Buffer::destroy(ctx, indexBuffer);

    Fence::destroy(ctx, drawFence);

    CommandBuffer::destroy(ctx, cmdBuffer, cmdPool);
    CommandPool::destroy(ctx, cmdPool);
    
    ShaderProgram::destroy(ctx, shaderProgram);
    
    Pipeline::Layout::destroy(ctx, pipelineLayout);
    Pipeline::destroy(ctx, pipeline);
    
    RenderSystem::destroy(ctx, renderSystem);
    Context::destroy(ctx);
}
