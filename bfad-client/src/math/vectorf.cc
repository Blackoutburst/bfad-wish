#include <stdlib.h>
#include <string.h>

#include "math/vectorf.hh"

namespace VectorF {
    U8 equals(VectorF::It* a, VectorF::It* b) {
        if (a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w) return 1;
        return 0;
    }

    U0 set(VectorF::It* vec, F32 x, F32 y, F32 z, F32 w) {
        vec->x = x;
        vec->y = y;
        vec->z = z;
        vec->w = w;
    }

    U0 normalize(VectorF::It* vec) {
        F32 mag = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z + vec->w * vec->w);
        if (mag == 0.0) return;

        vec->x /= mag;
        vec->y /= mag;
        vec->z /= mag;
        vec->w /= mag;
    }

    F32 length(VectorF::It* vec) {
        return sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z + vec->w * vec->w);
    }

    U0 zero(VectorF::It* vec) {
        set(vec, 0, 0, 0, 0);
    }

    U0 destroy(VectorF::It* vec) {
        free(vec);
    }

    VectorF::It* create(U0) {
        VectorF::It* vec = (VectorF::It*)malloc(sizeof(VectorF::It));
        zero(vec);

        return vec;
    }

    U0 writeTo2(VectorF::It* vec, F32* dest) {
        memcpy(dest, vec, sizeof(F32) * 2);
    }

    U0 writeTo3(VectorF::It* vec, F32* dest) {
        memcpy(dest, vec, sizeof(F32) * 3);
    }

    U0 writeTo4(VectorF::It* vec, F32* dest) {
        memcpy(dest, vec, sizeof(F32) * 4);
    }
}
