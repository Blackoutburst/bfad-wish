#pragma once

#include "types.hh"

#define PI 3.141592653589793

F32 rad(F32 angle);
I8 sign(I32 value);
I8 fsign(F32 value);
F32 fsignf(F32 value);
I32 clamp(I32 v, I32 min, I32 max);
I32 fclamp(F32 v, F32 min, F32 max);
F32 fclampf(F32 v, F32 min, F32 max);
