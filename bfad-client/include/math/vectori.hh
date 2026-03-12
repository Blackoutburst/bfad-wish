#pragma once

#include <math.h>

#include "types.hh"

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
}
