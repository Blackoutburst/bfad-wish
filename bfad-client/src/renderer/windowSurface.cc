#include "renderer/windowSurface.hh"

namespace WindowSurface {
    U0 destroy(VkInstance instance, VkSurfaceKHR surface) {
        vkDestroySurfaceKHR(instance, surface, NULL);
    }

    VkSurfaceKHR create(VkInstance instance, GLFWwindow* window) {
        VkSurfaceKHR surface = NULL;
        glfwCreateWindowSurface(instance, window, NULL, &surface);

        return surface;
    }
}
