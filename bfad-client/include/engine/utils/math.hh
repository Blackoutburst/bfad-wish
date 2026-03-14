#pragma once

#include "engine/types.hh"

U32 clamp(U32 d, U32 min, U32 max) {
  const U32 t = d < min ? min : d;
  return t > max ? max : t;
}
