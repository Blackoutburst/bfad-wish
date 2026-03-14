#include <stdlib.h>

#include "object/quad.hh"

#include "engine/utils/buffer.hh"
#include "engine/renderer/swapchain.hh"
#include "engine/renderer/vk.hh"

namespace Quad {

    Quad::It* create(Context::It* ctx, RenderSystem::It* renderSystem) {
        Quad::It* quad = (Quad::It*)malloc(sizeof(Quad::It));

        quad->model = Matrix::create();
        quad->vertexBuffer = Buffer::create(ctx, sizeof(Quad::vertex[0]) * Quad::vertexCount * 5, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        quad->indexBuffer  = Buffer::create(ctx, sizeof(Quad::index[0])  * Quad::indexCount,      VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        quad->shaderProgram = ShaderProgram::create(ctx, "./shader/quadVert.spv", "./shader/quadFrag.spv");
        quad->uniformBuffer = UniformBuffer::create(ctx, 0, 128);

        Matrix::translate2d(quad->model, 500.0f, 500.0f);
        Matrix::scale2d(quad->model, 200.0f, 200.0f);

        Buffer::upload(ctx, quad->vertexBuffer, Quad::vertex, sizeof(Quad::vertex[0]) * Quad::vertexCount * 5);
        Buffer::upload(ctx, quad->indexBuffer,  Quad::index,  sizeof(Quad::index[0])  * Quad::indexCount);

        VertexArray::Attribute attrs[2] = {
            { 0, VK_FORMAT_R32G32_SFLOAT, 0    },
            { 1, VK_FORMAT_R32G32B32_SFLOAT, 8 },
        };
        VertexArray::Description desc = { 20, attrs, 2 };
        
        quad->vao = VertexArray::create(ctx, &desc, quad->uniformBuffer, NULL, quad->shaderProgram, renderSystem->renderPass);

        return quad;
    }

    U0 render(Quad::It* quad, VkCommandBuffer cmdBuffer) {
        VertexArray::bind(quad->vao, cmdBuffer);

        F32 uniformData[32];
        Matrix::writeTo(quad->model, uniformData);
        Matrix::writeTo(getProjection2dMatrix(), uniformData + 16);

        UniformBuffer::update(quad->uniformBuffer, uniformData, 128);

        VkBuffer vertexBuffers[] = { quad->vertexBuffer->handle };
        VkDeviceSize offsets[] = { 0 };

        vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(cmdBuffer, quad->indexBuffer->handle, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(cmdBuffer, Quad::indexCount, 1, 0, 0, 0);
    }

    U0 destroy(Context::It* ctx, Quad::It* quad) {
        UniformBuffer::destroy(ctx, quad->uniformBuffer);

        Buffer::destroy(ctx, quad->vertexBuffer);
        Buffer::destroy(ctx, quad->indexBuffer);

        ShaderProgram::destroy(ctx, quad->shaderProgram);
        VertexArray::destroy(ctx, quad->vao);

        Matrix::destroy(quad->model);

        free(quad);
    }
}
