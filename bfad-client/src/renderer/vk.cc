#include <stdlib.h>

#include "renderer/vk.hh"
#include "devices/devices.hh"
#include "debug/logCallback.hh"
#include "window/window.hh"
#include "renderer/rendererInstance.hh"
#include "renderer/windowSurface.hh"
#include "utils/args.hh"
#include "utils/logger.hh"
#include "renderer/swapChain.hh"
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

static VkInstance instance;
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
static Device::It* device;
static VkSurfaceKHR windowSurface;
static VkDebugUtilsMessengerEXT loggerCallback;
static VkSemaphore* presentCompleteSemaphores;
static VkSemaphore* renderFinishedSemaphores;
static VkFence drawFence;
static U32 swapChainImageCount;
static U32 currentFrame;

U0 vkDrawTriangle(GLFWwindow* window) {
    Fence::wait(device, drawFence);
    Fence::reset(device, drawFence);

    U32 imageIndex;
    vkAcquireNextImageKHR(device->logical, swapchain, UINT64_MAX, presentCompleteSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(cmdBuffer, 0);
    CommandBuffer::begin(cmdBuffer);
    RenderPass::begin(window, device, renderPass, frambuffers, cmdBuffer, imageIndex, windowSurface);
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    VkExtent2D extends = SwapChain::extend(window, device, windowSurface);
    
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

    vkQueueSubmit(device->graphicQueue, 1, &submitInfo, drawFence);

    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphores[currentFrame];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(device->presentQueue, &presentInfo);

    currentFrame = (currentFrame + 1) % swapChainImageCount;
}

U0 vkInit(GLFWwindow* window) {
    instance = RendererInstance::create();

    if (Args::getValidationLayers()) {
        loggerCallback = LogCallback::create(instance);
    }
    
    windowSurface = WindowSurface::create(instance, window);

    VkPhysicalDevice physical = PhysicalDevice::create(instance, windowSurface);
    VkDevice logical = LogicalDevice::create(physical, windowSurface);
    device = Device::create(physical, logical, windowSurface);
    
    swapchain = SwapChain::create(window, device, windowSurface);
    imageView = ImageView::create(device, swapchain, windowSurface);

    swapChainImageCount = SwapChain::getImagesCount(device, swapchain);

    vertexShader = Shader::create(device, "./shader/triangleVert.spv");
    fragmentShader = Shader::create(device, "./shader/triangleFrag.spv");
    pipelineLayout = Pipeline::Layout::create(device);
    renderPass = RenderPass::create(device, windowSurface);
    pipeline = Pipeline::create(window, device, pipelineLayout, vertexShader, fragmentShader, renderPass, windowSurface);
    
    frambuffers = Framebuffer::create(window, device, swapchain, renderPass, imageView->imageView, windowSurface);
    cmdPool = CommandPool::create(device);
    cmdBuffer = CommandBuffer::create(cmdPool, device);

    presentCompleteSemaphores = (VkSemaphore*)malloc(swapChainImageCount * sizeof(VkSemaphore));
    for (U32 i = 0; i < swapChainImageCount; i++) {
        presentCompleteSemaphores[i] = Semaphore::create(device);
    }

    renderFinishedSemaphores = (VkSemaphore*)malloc(swapChainImageCount * sizeof(VkSemaphore));
    for (U32 i = 0; i < swapChainImageCount; i++) {
        renderFinishedSemaphores[i] = Semaphore::create(device);
    }
    
    drawFence = Fence::create(device);
}

U0 vkClean(U0) {
    vkDeviceWaitIdle(device->logical);

    for (U32 i = 0; i < swapChainImageCount; i++) {
        Semaphore::destroy(device, presentCompleteSemaphores[i]);
    }
    free(presentCompleteSemaphores);

    for (U32 i = 0; i < swapChainImageCount; i++) {
        Semaphore::destroy(device, renderFinishedSemaphores[i]);
    }
    free(renderFinishedSemaphores);
    
    Fence::destroy(device, drawFence);

    CommandBuffer::destroy(cmdBuffer, cmdPool, device);
    CommandPool::destroy(cmdPool, device);
    Framebuffer::destroy(device, swapchain, frambuffers);
    
    Shader::destroy(device, vertexShader);
    Shader::destroy(device, fragmentShader);
    Pipeline::Layout::destroy(device, pipelineLayout);
    RenderPass::destroy(device, renderPass);
    Pipeline::destroy(device, pipeline);
    ImageView::destroy(device, imageView, swapchain);
    SwapChain::destroy(device, swapchain);
    Device::destroy(device);
    WindowSurface::destroy(instance, windowSurface);
    
    if (Args::getValidationLayers()) {
        LogCallback::destroy(instance, loggerCallback);
    }
    
    RendererInstance::destroy(instance);
}
