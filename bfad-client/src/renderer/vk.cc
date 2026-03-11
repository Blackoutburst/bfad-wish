#include <stdlib.h>

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

static Context::It* ctx;

static VkShaderModule vertexShader;
static VkShaderModule fragmentShader;
static VkPipelineLayout pipelineLayout;
static VkRenderPass renderPass;
static VkPipeline pipeline;
static VkSwapchainKHR swapchain;
static ImageView::It* imageView;
static VkFramebuffer* frambuffers;
static VkCommandPool cmdPool;
static VkCommandBuffer cmdBuffer;
static VkDebugUtilsMessengerEXT loggerCallback;
static VkSemaphore* presentCompleteSemaphores;
static VkSemaphore* renderFinishedSemaphores;
static VkFence drawFence;
static U32 swapChainImageCount;
static U32 currentFrame;

U0 vkDrawTriangle(U0) {
    Fence::wait(ctx, drawFence);
    Fence::reset(ctx, drawFence);

    U32 imageIndex;
    vkAcquireNextImageKHR(ctx->device->logical, swapchain, UINT64_MAX, presentCompleteSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(cmdBuffer, 0);
    CommandBuffer::begin(cmdBuffer);
    RenderPass::begin(ctx, renderPass, frambuffers, cmdBuffer, imageIndex);
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

    vkCmdDraw(cmdBuffer, 3, 1, 0, 0);

    RenderPass::end(cmdBuffer);
    CommandBuffer::end(cmdBuffer);

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &presentCompleteSemaphores[currentFrame];
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderFinishedSemaphores[currentFrame];

    vkQueueSubmit(ctx->device->graphicQueue, 1, &submitInfo, drawFence);

    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphores[currentFrame];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(ctx->device->presentQueue, &presentInfo);

    currentFrame = (currentFrame + 1) % swapChainImageCount;
}

U0 vkInit(GLFWwindow* glfwWindow) {
    VkInstance instance = RendererInstance::create();

    if (Args::getValidationLayers()) {
        loggerCallback = LogCallback::create(instance);
    }
    
    VkSurfaceKHR windowSurface = WindowSurface::create(instance, glfwWindow);

    VkPhysicalDevice physical = PhysicalDevice::create(instance, windowSurface);
    VkDevice logical = LogicalDevice::create(physical, windowSurface);
    Device::It* device = Device::create(physical, logical, windowSurface);

    ctx = Context::create(glfwWindow, instance, device, windowSurface);
    
    swapchain = Swapchain::create(ctx);
    imageView = ImageView::create(ctx, swapchain);

    swapChainImageCount = Swapchain::getImagesCount(ctx, swapchain);

    vertexShader = Shader::create(ctx, "./shader/triangleVert.spv");
    fragmentShader = Shader::create(ctx, "./shader/triangleFrag.spv");
    pipelineLayout = Pipeline::Layout::create(ctx);
    renderPass = RenderPass::create(ctx);
    pipeline = Pipeline::create(ctx, pipelineLayout, vertexShader, fragmentShader, renderPass);
    
    frambuffers = Framebuffer::create(ctx, swapchain, renderPass, imageView->imageView);
    cmdPool = CommandPool::create(ctx);
    cmdBuffer = CommandBuffer::create(ctx, cmdPool);

    presentCompleteSemaphores = (VkSemaphore*)malloc(swapChainImageCount * sizeof(VkSemaphore));
    for (U32 i = 0; i < swapChainImageCount; i++) {
        presentCompleteSemaphores[i] = Semaphore::create(ctx);
    }

    renderFinishedSemaphores = (VkSemaphore*)malloc(swapChainImageCount * sizeof(VkSemaphore));
    for (U32 i = 0; i < swapChainImageCount; i++) {
        renderFinishedSemaphores[i] = Semaphore::create(ctx);
    }
    
    drawFence = Fence::create(ctx);
}

U0 vkClean(U0) {
    vkDeviceWaitIdle(ctx->device->logical);

    for (U32 i = 0; i < swapChainImageCount; i++) {
        Semaphore::destroy(ctx, presentCompleteSemaphores[i]);
    }
    free(presentCompleteSemaphores);

    for (U32 i = 0; i < swapChainImageCount; i++) {
        Semaphore::destroy(ctx, renderFinishedSemaphores[i]);
    }
    free(renderFinishedSemaphores);
    
    Fence::destroy(ctx, drawFence);

    CommandBuffer::destroy(ctx, cmdBuffer, cmdPool);
    CommandPool::destroy(ctx, cmdPool);
    Framebuffer::destroy(ctx, swapchain, frambuffers);
    
    Shader::destroy(ctx, vertexShader);
    Shader::destroy(ctx, fragmentShader);
    Pipeline::Layout::destroy(ctx, pipelineLayout);
    RenderPass::destroy(ctx, renderPass);
    Pipeline::destroy(ctx, pipeline);
    ImageView::destroy(ctx, imageView, swapchain);
    Swapchain::destroy(ctx, swapchain);
    
    Device::destroy(ctx->device);
    WindowSurface::destroy(ctx);
    
    if (Args::getValidationLayers()) {
        LogCallback::destroy(ctx->instance, loggerCallback);
    }
    
    RendererInstance::destroy(ctx);

    Context::destroy(ctx);
}
