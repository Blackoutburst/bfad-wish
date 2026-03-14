#include "engine/window/window.hh"

static U8 minimized = 0;

static U0 framebufferResizeCallback(GLFWwindow* window, I32 width, I32 height) {
    UNUSED_VAR(window);
    minimized = (width == 0 || height == 0) ? 1 : 0;
}

namespace Window {
    U0 update() {
        glfwPollEvents();
    }

    U0 destroy(GLFWwindow* window) {
        glfwDestroyWindow(window);

        glfwTerminate();
    }

    U8 isOpen(GLFWwindow* window) {
        return glfwWindowShouldClose(window) ? 0 : 1;
    }

    U8 isMinimized(U0) {
        return minimized;
    }

    GLFWwindow* create(U0) {
        GLFWwindow* window;
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(1280, 720, "Vulkan", NULL, NULL);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

        return window;
    }
}

