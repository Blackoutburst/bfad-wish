#include <stdlib.h>
#include <string.h>

#include "math/vectori.hh"

namespace VectorI {
    U8 equals(VectorI::It* a, VectorI::It* b) {
        if (a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w) return 1;
        return 0;
    }

    U0 set(VectorI::It* vec, I32 x, I32 y, I32 z, I32 w) {
        vec->x = x;
        vec->y = y;
        vec->z = z;
        vec->w = w;
    }

    U0 normalize(VectorI::It* vec) {
        F32 mag = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z + vec->w * vec->w);
        if (mag == 0.0) return;

        vec->x /= mag;
        vec->y /= mag;
        vec->z /= mag;
        vec->w /= mag;
    }

    I32 length(VectorI::It* vec) {
        return sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z + vec->w * vec->w);
    }

    U0 zero(VectorI::It* vec) {
        set(vec, 0, 0, 0, 0);
    }

    U0 destroy(VectorI::It* vec) {
        free(vec);
    }

    VectorI::It* create(U0) {
        VectorI::It* vec = (VectorI::It*)malloc(sizeof(VectorI::It));
        zero(vec);

        return vec;
    }

    U0 writeTo2(VectorI::It* vec, I32* dest) {
        memcpy(dest, vec, sizeof(I32) * 2);
    }

    U0 writeTo3(VectorI::It* vec, I32* dest) {
        memcpy(dest, vec, sizeof(I32) * 3);
    }

    U0 writeTo4(VectorI::It* vec, I32* dest) {
        memcpy(dest, vec, sizeof(I32) * 4);
    }
}
