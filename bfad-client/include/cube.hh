#pragma once

#include "types.hh"
#include "context.hh"
#include "renderSystem.hh"

#include "math/matrix.hh"
#include "renderer/shaderProgram.hh"
#include "renderer/vertexArray.hh"
#include "utils/uniformBuffer.hh"

namespace Cube {
    static U32 vertexCount = 144;
    static U32 indexCount = 36;

    static F32 vertex[144] = {
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

    static U32 index[36] = {
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

    struct It {
        Matrix::It* model;
        ShaderProgram::It* shaderProgram;
        Buffer::It* vertexBuffer;
        Buffer::It* indexBuffer;
        UniformBuffer::It* uniformBuffer;
        VertexArray::It* vao;
    };

    Cube::It* create(Context::It* ctx, RenderSystem::It* renderSystem);
    U0 render(Cube::It* cube, VkCommandBuffer cmdBuffer);
    U0 destroy(Context::It* ctx, Cube::It* cube);
}
