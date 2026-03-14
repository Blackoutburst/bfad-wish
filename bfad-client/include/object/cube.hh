#pragma once

#include "types.hh"
#include "engine/context.hh"

#include "engine/engine.hh"
#include "engine/math/matrix.hh"
#include "engine/renderer/shaderProgram.hh"
#include "engine/renderer/vertexArray.hh"
#include "engine/utils/uniformBuffer.hh"
#include "engine/texture/texture.hh"

namespace Cube {
    static U32 vertexCount = 192;
    static U32 indexCount = 36;

    // layout per vertex: x,y,z, r,g,b, u,v  (8 floats, stride=32)
    static F32 vertex[192] = {
        // Front
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

        // Back
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,

        // Left
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

        // Right
         1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,

        // Top
        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,

        // Bottom
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
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
        Texture::It* texture;
        VertexArray::It* vao;
    };

    Cube::It* create(Engine::It* engine);
    U0 render(Engine::It* engine, Cube::It* cube);
    U0 destroy(Engine::It* engine, Cube::It* cube);
}
