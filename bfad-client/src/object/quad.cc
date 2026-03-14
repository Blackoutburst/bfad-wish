#include <stdlib.h>

#include "object/quad.hh"

#include "engine/utils/buffer.hh"
#include "engine/renderer/swapchain.hh"

namespace Quad {

    Quad::It* create(Engine::It* engine) {
        Quad::It* quad = (Quad::It*)malloc(sizeof(Quad::It));

        quad->model = Matrix::create();
        quad->vertexBuffer = Buffer::create(engine->ctx, sizeof(Quad::vertex[0]) * Quad::vertexCount * 5, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        quad->indexBuffer  = Buffer::create(engine->ctx, sizeof(Quad::index[0])  * Quad::indexCount,      VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        quad->shaderProgram = ShaderProgram::create(engine->ctx, "./shader/quadVert.spv", "./shader/quadFrag.spv");
        quad->uniformBuffer = UniformBuffer::create(engine->ctx, 0, 128);

        Matrix::translate2d(quad->model, 500.0f, 500.0f);
        Matrix::scale2d(quad->model, 200.0f, 200.0f);

        Buffer::upload(engine->ctx, quad->vertexBuffer, Quad::vertex, sizeof(Quad::vertex[0]) * Quad::vertexCount * 5);
        Buffer::upload(engine->ctx, quad->indexBuffer,  Quad::index,  sizeof(Quad::index[0])  * Quad::indexCount);

        VertexArray::Attribute attrs[2] = {
            { 0, VK_FORMAT_R32G32_SFLOAT, 0    },
            { 1, VK_FORMAT_R32G32B32_SFLOAT, 8 },
        };
        VertexArray::Description desc = { 20, attrs, 2 };
        
        quad->vao = VertexArray::create(engine->ctx, &desc, quad->uniformBuffer, NULL, quad->shaderProgram, engine->renderSystem->renderPass);

        return quad;
    }

    U0 render(Engine::It* engine, Quad::It* quad) {
        VertexArray::bind(quad->vao, engine->cmdBuffer);

        F32 uniformData[32];
        Matrix::writeTo(quad->model, uniformData);
        Matrix::writeTo(engine->projection2d, uniformData + 16);

        UniformBuffer::update(quad->uniformBuffer, uniformData, 128);

        VkBuffer vertexBuffers[] = { quad->vertexBuffer->handle };
        VkDeviceSize offsets[] = { 0 };

        vkCmdBindVertexBuffers(engine->cmdBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(engine->cmdBuffer, quad->indexBuffer->handle, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(engine->cmdBuffer, Quad::indexCount, 1, 0, 0, 0);
    }

    U0 destroy(Engine::It* engine, Quad::It* quad) {
        vkDeviceWaitIdle(engine->ctx->device->logical);
    
        UniformBuffer::destroy(engine->ctx, quad->uniformBuffer);

        Buffer::destroy(engine->ctx, quad->vertexBuffer);
        Buffer::destroy(engine->ctx, quad->indexBuffer);

        ShaderProgram::destroy(engine->ctx, quad->shaderProgram);
        VertexArray::destroy(engine->ctx, quad->vao);

        Matrix::destroy(quad->model);

        free(quad);
    }
}
