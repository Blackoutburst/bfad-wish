#pragma once

#include <GLFW/glfw3.h>

#include "types.hh"
#include "math/matrix.hh"

U0 vkDrawTriangle(U0);
U0 vkInit(GLFWwindow* window);
U0 vkClean(U0);

Matrix::It* getViewMatrix(U0);
Matrix::It* getProjectionMatrix(U0);
Matrix::It* getProjection2dMatrix(U0);
