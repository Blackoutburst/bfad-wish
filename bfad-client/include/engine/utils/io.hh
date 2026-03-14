#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "engine/types.hh"

namespace File {
    I64 size(FILE* file);
    FILE* open(const I8* filePath);
    I8* read(FILE* file);
}
