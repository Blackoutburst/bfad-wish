#pragma once

#include <vulkan/vulkan.h>

#include "types.hh"
#include "context.hh"

namespace Sampler {
    VkSampler create(Context::It* ctx);
    U0 destroy(Context::It* ctx, VkSampler sampler);
}
