#define GLFW_INCLUDE_VULKAN
#define LOGGER_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdio.h>

#include "engine/logger.h"

#include "engine/types.hh"
#include "engine/utils/args.hh"
#include "engine/engine.hh"
#include "engine/window/window.hh"
#include "engine/math/matrix.hh"
#include "engine/math/math.hh"

#include "object/cube.hh"

#define CAMERA_SPEED 0.001
#define CAMERA_SENSITIVITY 0.1

I32 main(I32 argc, I8** argv) {
    Args::parse(argc, argv);
    
    GLFWwindow* window = Window::create();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Engine::It* engine = Engine::create(window);

    Cube::It* cubes[64];

    VectorF::It* position = VectorF::create();
    F32 yaw = 0;
    F32 pitch = 0;
    F64 prevMouseX = 0;
    F64 prevMouseY = 0;

    for (U32 i = 0; i < 64; i++) {
        F32 x = (i % 4) * 2.0f - 3.0f;
        F32 y = ((i / 4) % 4) * 2.0f - 3.0f;
        F32 z = (i / 16) * -2.0f;
        cubes[i] = Cube::create(engine, "./texture2.png", { x, y, z, 0 }, { 0.5, 0.5, 0.5, 0.5 });
    }
    
    while (Window::isOpen(window)) {
        if (!Engine::begin(engine)) continue;

        for (U32 i = 0; i < 64; i++) {
            Cube::render(engine, cubes[i]);
        }

        Engine::end(engine);

        // Camera move    
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position->x -= sin(rad(-yaw)) * CAMERA_SPEED;
            position->z -= cos(rad(-yaw)) * CAMERA_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position->x += sin(rad(-yaw)) * CAMERA_SPEED;
            position->z += cos(rad(-yaw)) * CAMERA_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            position->x += sin(rad(-yaw - 90)) * CAMERA_SPEED;
            position->z += cos(rad(-yaw - 90)) * CAMERA_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            position->x += sin(rad(-yaw + 90)) * CAMERA_SPEED;
            position->z += cos(rad(-yaw + 90)) * CAMERA_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            position->y -= 1 * CAMERA_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            position->y += 1 * CAMERA_SPEED;
        }

        // Camera rotate
        F64 mouseX = 0;
        F64 mouseY = 0;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        F64 yawOffset = mouseX - prevMouseX;
        F64 pitchOffset = mouseY - prevMouseY;

        pitchOffset *= CAMERA_SENSITIVITY;
        yawOffset *= CAMERA_SENSITIVITY;

        prevMouseX = mouseX;
        prevMouseY = mouseY;

        yaw += yawOffset;
        pitch -= pitchOffset;
        if (pitch > 89) pitch = 89;
        if (pitch < -89) pitch = -89;

        

        Matrix::setIdentity(engine->view);
        Matrix::rotate(engine->view, rad(pitch), 1, 0, 0);
        Matrix::rotate(engine->view, rad(yaw), 0, 1, 0);
        Matrix::translate3d(engine->view, -position->x, position->y, -position->z);
    }

    for (U32 i = 0; i < 64; i++) {
        Cube::destroy(engine, cubes[i]);
    }

    Engine::destroy(engine);

    Window::destroy(window);


    return 0;
}
