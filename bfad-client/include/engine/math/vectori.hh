#pragma once

#include <math.h>

#include "engine/types.hh"

namespace VectorI {
    struct It {
        I32 x;
        I32 y;
        I32 z;
        I32 w;
    };

    U8 equals(VectorI::It* a, VectorI::It* b);
    U0 set(VectorI::It* vec, I32 x, I32 y, I32 z, I32 w);
    U0 normalize(VectorI::It* vec);
    I32 length(VectorI::It* vec);
    U0 zero(VectorI::It* vec);
    U0 destroy(VectorI::It* vec);
    VectorI::It* create(U0);
    U0 writeTo2(VectorI::It* vec, I32* dest);
    U0 writeTo3(VectorI::It* vec, I32* dest);
    U0 writeTo4(VectorI::It* vec, I32* dest);
}
