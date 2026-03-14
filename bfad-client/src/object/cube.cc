#include <stdlib.h>

#include "object/cube.hh"

#include "engine/engine.hh"
#include "engine/utils/buffer.hh"
#include "engine/math/math.hh"

static U8 isInit = 0;
static ShaderProgram::It* shaderProgram;
static Buffer::It* vertexBuffer;
static Buffer::It* indexBuffer;

namespace Cube {
    static U0 __init(Engine::It* engine) {
        vertexBuffer = Buffer::create(engine->ctx, sizeof(Cube::vertex[0]) * Cube::vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        indexBuffer = Buffer::create(engine->ctx, sizeof(Cube::index[0]) * Cube::indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        shaderProgram = ShaderProgram::create(engine->ctx, "./shader/cubeVert.spv", "./shader/cubeFrag.spv");

        Buffer::upload(engine->ctx, vertexBuffer, Cube::vertex, sizeof(Cube::vertex[0]) * Cube::vertexCount);
        Buffer::upload(engine->ctx, indexBuffer, Cube::index, sizeof(Cube::index[0]) * Cube::indexCount);

        isInit = 1;
    }

    Cube::It* create(Engine::It* engine, const I8* texturePath, VectorF::It position, VectorF::It size) {
        if (!isInit) __init(engine);
    
        Cube::It* cube = (Cube::It*)malloc(sizeof(Cube::It));

        cube->position = VectorF::create();
        cube->size = VectorF::create();
        cube->model = Matrix::create();
        cube->texture = Texture::create(engine->ctx, texturePath);
        cube->uniformBuffer = UniformBuffer::create(engine->ctx, 0, 192);

        VectorF::set(cube->position, position.x, position.y, position.z, position.w);
        VectorF::set(cube->size,  size.x, size.y, size.z, size.w);

        VertexArray::Attribute attrs[3] = {
            { 0, VK_FORMAT_R32G32B32_SFLOAT, 0  },
            { 1, VK_FORMAT_R32G32B32_SFLOAT, 12 },
            { 2, VK_FORMAT_R32G32_SFLOAT,    24 },
        };
        VertexArray::Description desc = { 32, attrs, 3 };

        cube->vao = VertexArray::create(engine->ctx, &desc, cube->uniformBuffer, cube->texture, shaderProgram, engine->renderSystem->renderPass);

        return cube;
    }
    
    U0 render(Engine::It* engine, Cube::It* cube) {
        Matrix::setIdentity(cube->model);
        Matrix::translate3dP(cube->model, cube->position);
        Matrix::scale3dP(cube->model, cube->size);
    
        VertexArray::bind(cube->vao, engine->cmdBuffer);
        Texture::bind(cube->texture, engine->cmdBuffer, cube->vao->layout);

        F32 uniformData[48];
        Matrix::writeTo(cube->model, uniformData);
        Matrix::writeTo(engine->view, uniformData + 16);
        Matrix::writeTo(engine->projection, uniformData + 32);

        UniformBuffer::update(cube->uniformBuffer, uniformData, 192);

        VkBuffer vertexBuffers[] = { vertexBuffer->handle };
        VkDeviceSize offsets[] = { 0 };

        vkCmdBindVertexBuffers(engine->cmdBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(engine->cmdBuffer, indexBuffer->handle, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(engine->cmdBuffer, Cube::indexCount, 1, 0, 0, 0);
    }
    
    U0 destroy(Engine::It* engine, Cube::It* cube) {
        vkDeviceWaitIdle(engine->ctx->device->logical);
    
        Texture::destroy(engine->ctx, cube->texture);

        Matrix::destroy(cube->model);
        VectorF::destroy(cube->position);
        VectorF::destroy(cube->size);

        UniformBuffer::destroy(engine->ctx, cube->uniformBuffer);

        VertexArray::destroy(engine->ctx, cube->vao);

        free(cube);
    }

}
