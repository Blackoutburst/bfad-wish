#pragma once

#include <vulkan/vulkan.h>

#include "engine/types.hh"
#include "engine/context.hh"

namespace Sampler {
    VkSampler create(Context::It* ctx);
    U0 destroy(Context::It* ctx, VkSampler sampler);
}
