#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "types.hh"

namespace WindowSurface {
    U0 destroy(VkInstance instance, VkSurfaceKHR surface);
    VkSurfaceKHR create(VkInstance instance, GLFWwindow* window);
}
