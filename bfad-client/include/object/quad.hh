#pragma once

#include "types.hh"
#include "engine/context.hh"

#include "engine/engine.hh"
#include "engine/renderer/shaderProgram.hh"
#include "engine/renderer/vertexArray.hh"
#include "engine/utils/uniformBuffer.hh"
#include "engine/math/matrix.hh"

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

    Quad::It* create(Engine::It* engine);
    U0 render(Engine::It* engine, Quad::It* quad);
    U0 destroy(Engine::It* engine, Quad::It* quad);
}
