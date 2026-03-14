#include "engine/renderer/windowSurface.hh"

namespace WindowSurface {
    U0 destroy(Context::It* ctx) {
        vkDestroySurfaceKHR(ctx->instance, ctx->surface, NULL);
    }

    VkSurfaceKHR create(VkInstance instance, GLFWwindow* window) {
        VkSurfaceKHR surface = NULL;
        glfwCreateWindowSurface(instance, window, NULL, &surface);

        return surface;
    }
}
