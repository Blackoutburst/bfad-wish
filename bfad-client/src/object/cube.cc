#include <stdlib.h>

#include "object/cube.hh"

#include "engine/engine.hh"
#include "engine/utils/buffer.hh"
#include "engine/math/math.hh"

namespace Cube {

    Cube::It* create(Engine::It* engine) {
        Cube::It* cube = (Cube::It*)malloc(sizeof(Cube::It));

        cube->model = Matrix::create();
        cube->vertexBuffer = Buffer::create(engine->ctx, sizeof(Cube::vertex[0]) * Cube::vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        cube->indexBuffer = Buffer::create(engine->ctx, sizeof(Cube::index[0]) * Cube::indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        cube->shaderProgram = ShaderProgram::create(engine->ctx, "./shader/cubeVert.spv", "./shader/cubeFrag.spv");
        cube->uniformBuffer = UniformBuffer::create(engine->ctx, 0, 192);
        cube->texture = Texture::create(engine->ctx, "./texture.png");

        Buffer::upload(engine->ctx, cube->vertexBuffer, Cube::vertex, sizeof(Cube::vertex[0]) * Cube::vertexCount);
        Buffer::upload(engine->ctx, cube->indexBuffer, Cube::index, sizeof(Cube::index[0]) * Cube::indexCount);

        VertexArray::Attribute attrs[3] = {
            { 0, VK_FORMAT_R32G32B32_SFLOAT, 0  },
            { 1, VK_FORMAT_R32G32B32_SFLOAT, 12 },
            { 2, VK_FORMAT_R32G32_SFLOAT,    24 },
        };
        VertexArray::Description desc = { 32, attrs, 3 };

        cube->vao = VertexArray::create(engine->ctx, &desc, cube->uniformBuffer, cube->texture, cube->shaderProgram, engine->renderSystem->renderPass);

        Matrix::rotate(cube->model, rad(45), 1.0, 0.0, 0.0);
        Matrix::rotate(cube->model, rad(45), 0.0, 1.0, 0.0);
        Matrix::rotate(cube->model, rad(45), 0.0, 0.0, 1.0);

        return cube;
    }
    
    U0 render(Engine::It* engine, Cube::It* cube) {
        VertexArray::bind(cube->vao, engine->cmdBuffer);

        F32 uniformData[48];
        Matrix::writeTo(cube->model, uniformData);
        Matrix::writeTo(engine->view, uniformData + 16);
        Matrix::writeTo(engine->projection, uniformData + 32);

        UniformBuffer::update(cube->uniformBuffer, uniformData, 192);

        VkBuffer vertexBuffers[] = { cube->vertexBuffer->handle };
        VkDeviceSize offsets[] = { 0 };

        vkCmdBindVertexBuffers(engine->cmdBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(engine->cmdBuffer, cube->indexBuffer->handle, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(engine->cmdBuffer, Cube::indexCount, 1, 0, 0, 0);
    }
    
    U0 destroy(Engine::It* engine, Cube::It* cube) {
        vkDeviceWaitIdle(engine->ctx->device->logical);
    
        UniformBuffer::destroy(engine->ctx, cube->uniformBuffer);
        Texture::destroy(engine->ctx, cube->texture);

        Buffer::destroy(engine->ctx, cube->vertexBuffer);
        Buffer::destroy(engine->ctx, cube->indexBuffer);

        ShaderProgram::destroy(engine->ctx, cube->shaderProgram);
        VertexArray::destroy(engine->ctx, cube->vao);

        Matrix::destroy(cube->model);

        free(cube);
    }

}
