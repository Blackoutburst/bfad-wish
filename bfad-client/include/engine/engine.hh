#pragma once

#include <GLFW/glfw3.h>

#include "types.hh"

#include "engine/context.hh"
#include "engine/renderer/renderSystem.hh"
#include "engine/utils/fence.hh"
#include "engine/device/commandBuffer.hh"
#include "engine/window/window.hh"
#include "engine/renderer/renderPass.hh"
#include "engine/math/matrix.hh"

namespace Engine {

    struct It {
        Context::It* ctx;
        RenderSystem::It* renderSystem;
        VkCommandBuffer cmdBuffer;
        VkFence drawFence;

        Matrix::It* view;
        Matrix::It* projection;
        Matrix::It* projection2d;
    };

    U8 begin(Engine::It* engine);
    U0 end(Engine::It* engine);
    Engine::It* create(GLFWwindow* window);
    U0 destroy(Engine::It* engine);
}
