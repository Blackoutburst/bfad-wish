#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "types.hh"
#include "devices/devices.hh"

namespace Context {
    struct It {
        GLFWwindow* window;
        VkInstance instance;
        Device::It* device;
        VkSurfaceKHR surface;
    };

    Context::It* create(GLFWwindow* window, VkInstance instance, Device::It* device, VkSurfaceKHR surface);
    U0 destroy(Context::It* context);
}
