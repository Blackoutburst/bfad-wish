#pragma once

#include <vulkan/vulkan.h>

#include "engine/types.hh"
#include "engine/context.hh"

namespace RendererInstance {
    U0 destroy(Context::It* ctx);
    VkInstance create(U0);
}
