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

static VkInstance instance;
static VkShaderModule vertexShader;
static VkShaderModule fragmentShader;
static VkPipelineLayout pipelineLayout;
static VkRenderPass renderPass;
static VkPipeline pipeline;
static VkSwapchainKHR swapchain;
static VkImageView* imageView;
static VkFramebuffer* frambuffers;
static VkCommandPool cmdPool;
static VkCommandBuffer cmdBuffer;
static Device::It* device;
static VkSurfaceKHR windowSurface;
static VkDebugUtilsMessengerEXT loggerCallback;

U0 vkDrawTriangle(GLFWwindow* window) {
    CommandBuffer::begin(cmdBuffer);
    RenderPass::begin(window, device, renderPass, frambuffers, cmdBuffer, 0, windowSurface);

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
}
U0 vkInit(GLFWwindow* window) {
    RendererInstance::create();
    
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

    vertexShader = Shader::create(device, "./shader/triangleVert.spv");
    fragmentShader = Shader::create(device, "./shader/triangleFrag.spv");
    pipelineLayout = Pipeline::Layout::create(device);
    renderPass = RenderPass::create(device, windowSurface);
    pipeline = Pipeline::create(window, device, pipelineLayout, vertexShader, fragmentShader, renderPass, windowSurface);
    
    frambuffers = Framebuffer::create(window, device, swapchain, renderPass, imageView, windowSurface);
    cmdPool = CommandPool::create(device);
    cmdBuffer = CommandBuffer::create(cmdPool, device);
}

U0 vkClean(U0) {
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
