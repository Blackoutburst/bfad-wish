#pragma once

#include <math.h>

#incluce "types.hh"

namespace VectorI {
    struct It {
        I32 x;
        I32 y;
        I32 z;
        I32 w;
    };

    U8 equals(VectorI* a, VectorI* b);
    U0 set(VectorI* vec, I32 x, I32 y, I32 z, I32 w);
    U0 normalize(VectorI* vec);
    I32 length(VectorI* vec);
    U0 zero(VectorI* vec);
    U0 destroy(VectorI* vec);
    VectorI::It* create(U0);
}
