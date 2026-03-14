#pragma once

#include "engine/types.hh"
#include "engine/engine.hh"
#include "engine/math/vectorf.hh"
#include "engine/math/matrix.hh"
#include "engine/renderer/shaderProgram.hh"
#include "engine/renderer/vertexArray.hh"
#include "engine/utils/uniformBuffer.hh"
#include "engine/texture/texture.hh"

namespace Cube {
    static U32 vertexCount = 192;
    static U32 indexCount = 36;

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
        8,  9,  10,  10, 11, 8,
        // Right
        13, 12, 15,  15, 14, 13,
        // Top
        16, 17, 18,  18, 19, 16,
        // Bottom
        20, 23, 22,  22, 21, 20,
    };

    struct It {
        VertexArray::It* vao;
        UniformBuffer::It* uniformBuffer;
        VectorF::It* position;
        VectorF::It* size;
        Matrix::It* model;
        Texture::It* texture;
    };

    Cube::It* create(Engine::It* engine, const I8* texturePath, VectorF::It position, VectorF::It size);
    U0 render(Engine::It* engine, Cube::It* cube);
    U0 destroy(Engine::It* engine, Cube::It* cube);
}
