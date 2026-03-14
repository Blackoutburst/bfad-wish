#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "types.hh"
#include "engine/context.hh"

namespace WindowSurface {
    U0 destroy(Context::It* ctx);
    VkSurfaceKHR create(VkInstance instance, GLFWwindow* window);
}
