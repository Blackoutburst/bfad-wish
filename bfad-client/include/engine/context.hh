#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "types.hh"
#include "device/device.hh"

namespace Context {
    struct It {
        GLFWwindow* window;
        VkInstance instance;
        Device::It* device;
        VkSurfaceKHR surface;
        VkDebugUtilsMessengerEXT loggerCallback;
        VkDescriptorPool descriptorPool;
        VkCommandPool cmdPool;
    };

    Context::It* create(GLFWwindow* window);
    U0 destroy(Context::It* context);
}
