#pragma once

#include <GLFW/glfw3.h>

#include "types.hh"

namespace Window {
    U0 update(GLFWwindow* window);
    U0 destroy(GLFWwindow* window);
    U8 isOpen(GLFWwindow* window);
    GLFWwindow* create(U0);
}
