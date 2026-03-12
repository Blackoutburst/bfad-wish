#pragma once

#include <math.h>

#incluce "types.hh"

namespace VectorF {
    struct It {
        F32 x;
        F32 y;
        F32 z;
        F32 w;
    };

    U8 equals(VectorF* a, VectorF* b);
    U0 set(VectorF* vec, F32 x, F32 y, F32 z, F32 w);
    U0 normalize(VectorF* vec);
    F32 length(VectorF* vec);
    U0 zero(VectorF* vec);
    U0 destroy(VectorF* vec);
    VectorF::It* create(U0);
}
