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
#include "devices/commandBuffer.hh"
#include "utils/semaphore.hh"
#include "utils/fence.hh"
#include "utils/buffer.hh"
#include "utils/uniformBuffer.hh"
#include "renderer/vertexArray.hh"
#include "math/matrix.hh"
#include "math/math.hh"

static Context::It* ctx;
static RenderSystem::It* renderSystem;
static ShaderProgram::It* shaderProgram;
static VertexArray::It* vao;
static VkCommandBuffer cmdBuffer;
static VkFence drawFence;
static Buffer::It* vertexBuffer;
static Buffer::It* indexBuffer;
static UniformBuffer::It* uniformBuffer;

static Matrix::It* model;
static Matrix::It* view;
static Matrix::It* projection;

U0 vkDrawTriangle(U0) {
    Fence::wait(ctx, drawFence);

    VkResult swapchainStatus = vkAcquireNextImageKHR(ctx->device->logical, renderSystem->swapchain, UINT64_MAX, renderSystem->imageView->pSemaphore[renderSystem->imageView->currentFrame], VK_NULL_HANDLE, &renderSystem->imageView->imageIndex);
    if (swapchainStatus == VK_ERROR_OUT_OF_DATE_KHR || swapchainStatus == VK_SUBOPTIMAL_KHR) {
        RenderSystem::update(ctx, renderSystem);
        VkExtent2D ext = Swapchain::extend(ctx);
        Matrix::projection(projection, ext.width, ext.height, 90, 0.1, 1000);
        return;
    }

    Fence::reset(ctx, drawFence);

    CommandBuffer::begin(cmdBuffer);
    
    RenderPass::begin(ctx, renderSystem->renderPass, renderSystem->framebuffers, cmdBuffer, renderSystem->imageView->imageIndex);
    
    VertexArray::bind(vao, cmdBuffer);

    Swapchain::setViewport(ctx, cmdBuffer);

    VkBuffer vertexBuffers[] = { vertexBuffer->handle };
    VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(cmdBuffer, indexBuffer->handle, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(cmdBuffer, 36, 1, 0, 0, 0);

    RenderPass::end(cmdBuffer);
    CommandBuffer::end(cmdBuffer);

    F32 uniformData[48] = {
        // Model
        model->m00, model->m01, model->m02, model->m03,
        model->m10, model->m11, model->m12, model->m13,
        model->m20, model->m21, model->m22, model->m23,
        model->m30, model->m31, model->m32, model->m33,

        // View
        view->m00, view->m01, view->m02, view->m03,
        view->m10, view->m11, view->m12, view->m13,
        view->m20, view->m21, view->m22, view->m23,
        view->m30, view->m31, view->m32, view->m33,

        // Projection
        projection->m00, projection->m01, projection->m02, projection->m03,
        projection->m10, projection->m11, projection->m12, projection->m13,
        projection->m20, projection->m21, projection->m22, projection->m23,
        projection->m30, projection->m31, projection->m32, projection->m33,
    };

    UniformBuffer::update(uniformBuffer, uniformData, 192);

    RenderSystem::submit(ctx, renderSystem, cmdBuffer, drawFence);
    RenderSystem::present(ctx, renderSystem);
}

U0 vkInit(GLFWwindow* window) {
    ctx = Context::create(window);
    renderSystem = RenderSystem::create(ctx);

    shaderProgram = ShaderProgram::create(ctx, "./shader/triangleVert.spv", "./shader/triangleFrag.spv");

    uniformBuffer = UniformBuffer::create(ctx, 0, 192);

    VertexArray::Attribute attrs[2] = {
        { 0, VK_FORMAT_R32G32B32_SFLOAT, 0  },
        { 1, VK_FORMAT_R32G32B32_SFLOAT, 12 },
    };
    VertexArray::Description desc = { 24, attrs, 2 };
    vao = VertexArray::create(ctx, &desc, uniformBuffer, shaderProgram, renderSystem->renderPass);
    
    cmdBuffer = CommandBuffer::create(ctx);

    drawFence = Fence::create(ctx);

    F32 vertexData[144] = {
        // Front
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

        // Back
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

        // Left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

        // Right
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,

        // Top
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,

        // Bottom
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
    };

    U32 indexData[36] = {
        // Front
        0,  1,  2,   2,  3,  0,
        // Back
        5,  4,  7,   7,  6,  5,
        // Left
        9,  8,  11,  11, 10, 9,
        // Right
        12, 13, 14,  14, 15, 12,
        // Top
        16, 17, 18,  18, 19, 16,
        // Bottom
        22, 23, 20,  20, 21, 22,
    };

    vertexBuffer = Buffer::create(ctx, sizeof(vertexData[0]) * 144, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    Buffer::upload(ctx, vertexBuffer, vertexData, sizeof(vertexData[0]) * 144);

    indexBuffer = Buffer::create(ctx, sizeof(indexData[0]) * 36, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    Buffer::upload(ctx, indexBuffer, indexData, sizeof(indexData[0]) * 36);


    model = Matrix::create();
    view = Matrix::create();
    projection = Matrix::create();
    Matrix::projection(projection, 1280, 720, 90, 0.1, 1000);
    Matrix::translate3d(view, 0.0f, 0.0f, -2.0f);
    Matrix::rotate(model, rad(45), 1.0f, 0.0f, 0.0f);
    Matrix::rotate(model, rad(45), 0.0f, 1.0f, 0.0f);
    Matrix::rotate(model, rad(45), 0.0f, 0.0f, 1.0f);
}

U0 vkClean(U0) {
    vkDeviceWaitIdle(ctx->device->logical);

    UniformBuffer::destroy(ctx, uniformBuffer);

    Matrix::destroy(model);
    Matrix::destroy(view);
    Matrix::destroy(projection);

    Buffer::destroy(ctx, vertexBuffer);
    Buffer::destroy(ctx, indexBuffer);

    Fence::destroy(ctx, drawFence);

    CommandBuffer::destroy(ctx, cmdBuffer);
    
    ShaderProgram::destroy(ctx, shaderProgram);
    VertexArray::destroy(ctx, vao);
    
    RenderSystem::destroy(ctx, renderSystem);
    Context::destroy(ctx);
}
