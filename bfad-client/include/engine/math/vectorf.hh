#pragma once

#include <math.h>

#include "engine/types.hh"

namespace VectorF {
    struct It {
        F32 x;
        F32 y;
        F32 z;
        F32 w;
    };

    U8 equals(VectorF::It* a, VectorF::It* b);
    U0 set(VectorF::It* vec, F32 x, F32 y, F32 z, F32 w);
    U0 normalize(VectorF::It* vec);
    F32 length(VectorF::It* vec);
    U0 zero(VectorF::It* vec);
    U0 destroy(VectorF::It* vec);
    VectorF::It* create(U0);
    U0 writeTo2(VectorF::It* vec, F32* dest);
    U0 writeTo3(VectorF::It* vec, F32* dest);
    U0 writeTo4(VectorF::It* vec, F32* dest);
}
