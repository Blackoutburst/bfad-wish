#include <stdlib.h>

#include "object/cube.hh"

#include "engine/utils/buffer.hh"
#include "engine/renderer/vk.hh"
#include "engine/math/math.hh"

namespace Cube {

    Cube::It* create(Context::It* ctx, RenderSystem::It* renderSystem) {
        Cube::It* cube = (Cube::It*)malloc(sizeof(Cube::It));

        cube->model = Matrix::create();
        cube->vertexBuffer = Buffer::create(ctx, sizeof(Cube::vertex[0]) * Cube::vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        cube->indexBuffer = Buffer::create(ctx, sizeof(Cube::index[0]) * Cube::indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        cube->shaderProgram = ShaderProgram::create(ctx, "./shader/cubeVert.spv", "./shader/cubeFrag.spv");
        cube->uniformBuffer = UniformBuffer::create(ctx, 0, 192);
        cube->texture = Texture::create(ctx, "./texture.png");

        Buffer::upload(ctx, cube->vertexBuffer, Cube::vertex, sizeof(Cube::vertex[0]) * Cube::vertexCount);
        Buffer::upload(ctx, cube->indexBuffer, Cube::index, sizeof(Cube::index[0]) * Cube::indexCount);

        VertexArray::Attribute attrs[3] = {
            { 0, VK_FORMAT_R32G32B32_SFLOAT, 0  },
            { 1, VK_FORMAT_R32G32B32_SFLOAT, 12 },
            { 2, VK_FORMAT_R32G32_SFLOAT,    24 },
        };
        VertexArray::Description desc = { 32, attrs, 3 };

        cube->vao = VertexArray::create(ctx, &desc, cube->uniformBuffer, cube->texture, cube->shaderProgram, renderSystem->renderPass);

        Matrix::rotate(cube->model, rad(45), 1.0, 0.0, 0.0);
        Matrix::rotate(cube->model, rad(45), 0.0, 1.0, 0.0);
        Matrix::rotate(cube->model, rad(45), 0.0, 0.0, 1.0);

        return cube;
    }
    
    U0 render(Cube::It* cube, VkCommandBuffer cmdBuffer) {
        VertexArray::bind(cube->vao, cmdBuffer);

        F32 uniformData[48];
        Matrix::writeTo(cube->model, uniformData);
        Matrix::writeTo(getViewMatrix(), uniformData + 16);
        Matrix::writeTo(getProjectionMatrix(), uniformData + 32);

        UniformBuffer::update(cube->uniformBuffer, uniformData, 192);

        VkBuffer vertexBuffers[] = { cube->vertexBuffer->handle };
        VkDeviceSize offsets[] = { 0 };

        vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(cmdBuffer, cube->indexBuffer->handle, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(cmdBuffer, Cube::indexCount, 1, 0, 0, 0);
    }
    
    U0 destroy(Context::It* ctx, Cube::It* cube) {
        UniformBuffer::destroy(ctx, cube->uniformBuffer);
        Texture::destroy(ctx, cube->texture);

        Buffer::destroy(ctx, cube->vertexBuffer);
        Buffer::destroy(ctx, cube->indexBuffer);

        ShaderProgram::destroy(ctx, cube->shaderProgram);
        VertexArray::destroy(ctx, cube->vao);

        Matrix::destroy(cube->model);

        free(cube);
    }

}
