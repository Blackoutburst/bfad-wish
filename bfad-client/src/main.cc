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

#include "object/cube.hh"

I32 main(I32 argc, I8** argv) {
    Args::parse(argc, argv);
    
    GLFWwindow* window = Window::create();

    Engine::It* engine = Engine::create(window);

    Cube::It* cube = Cube::create(engine);
    
    while (Window::isOpen(window)) {
        if (!Engine::begin(engine)) continue;

        Cube::render(engine, cube);

        Engine::end(engine);
    }

    Cube::destroy(engine, cube);

    Engine::destroy(engine);

    Window::destroy(window);


    return 0;
}
