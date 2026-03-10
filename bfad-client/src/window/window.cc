#include "window/window.hh"

namespace Window {
    U0 update(GLFWwindow* window) {
        UNUSED_VAR(window);
        
        glfwPollEvents();
    }

    U0 destroy(GLFWwindow* window) {
        glfwDestroyWindow(window);

        glfwTerminate();
    }

    U8 isOpen(GLFWwindow* window) {
        return glfwWindowShouldClose(window) ? 0 : 1;
    }

    GLFWwindow* create(U0) {
        GLFWwindow* window;
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(800, 600, "Vulkan", NULL, NULL);

        return window;
    }
}

