#include <stdlib.h>

#include "cube.hh"

#include "utils/buffer.hh"
#include "renderer/vk.hh"
#include "math/math.hh"

namespace Cube {

    Cube::It* create(Context::It* ctx, RenderSystem::It* renderSystem) {
        Cube::It* cube = (Cube::It*)malloc(sizeof(Cube::It));

        cube->model = Matrix::create();
        cube->vertexBuffer = Buffer::create(ctx, sizeof(Cube::vertex[0]) * Cube::vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        cube->indexBuffer = Buffer::create(ctx, sizeof(Cube::index[0]) * Cube::indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        cube->shaderProgram = ShaderProgram::create(ctx, "./shader/triangleVert.spv", "./shader/triangleFrag.spv");
        cube->uniformBuffer = UniformBuffer::create(ctx, 0, 192);
        
        Buffer::upload(ctx, cube->vertexBuffer, Cube::vertex, sizeof(Cube::vertex[0]) * Cube::vertexCount);
        Buffer::upload(ctx, cube->indexBuffer, Cube::index, sizeof(Cube::index[0]) * Cube::indexCount);

        VertexArray::Attribute attrs[2] = {
            { 0, VK_FORMAT_R32G32B32_SFLOAT, 0  },
            { 1, VK_FORMAT_R32G32B32_SFLOAT, 12 },
        };
        VertexArray::Description desc = { 24, attrs, 2 };
        
        cube->vao = VertexArray::create(ctx, &desc, cube->uniformBuffer, cube->shaderProgram, renderSystem->renderPass);

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
        
        Buffer::destroy(ctx, cube->vertexBuffer);
        Buffer::destroy(ctx, cube->indexBuffer);

        ShaderProgram::destroy(ctx, cube->shaderProgram);
        VertexArray::destroy(ctx, cube->vao);

        Matrix::destroy(cube->model);

        free(cube);
    }

}
