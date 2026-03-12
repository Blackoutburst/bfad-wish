#pragma once

#include "types.hh"
#include "context.hh"
#include "renderSystem.hh"

#include "math/matrix.hh"
#include "renderer/shaderProgram.hh"
#include "renderer/vertexArray.hh"
#include "utils/uniformBuffer.hh"

namespace Quad {
    static U32 vertexCount = 4;
    static U32 indexCount = 6;

    static F32 vertex[20] = {
        -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
    };

    static U32 index[6] = {
        0, 1, 2,  2, 3, 0,
    };

    struct It {
        Matrix::It* model;
        ShaderProgram::It* shaderProgram;
        Buffer::It* vertexBuffer;
        Buffer::It* indexBuffer;
        UniformBuffer::It* uniformBuffer;
        VertexArray::It* vao;
    };

    Quad::It* create(Context::It* ctx, RenderSystem::It* renderSystem);
    U0 render(Quad::It* quad, VkCommandBuffer cmdBuffer);
    U0 destroy(Context::It* ctx, Quad::It* quad);
}
