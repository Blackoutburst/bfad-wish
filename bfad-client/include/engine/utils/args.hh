#pragma once

#include "engine/types.hh"

namespace Args {
    U0 parse(I32 argc, I8** argv);
    U8 getValidationLayers(U0);
    U0 validationLayers(U0);
    U0 help(U0);
}
