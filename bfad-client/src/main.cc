#define GLFW_INCLUDE_VULKAN
#define LOGGER_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdio.h>

#include "logger.hh"
#include "types.hh"

#include "engine/utils/args.hh"
#include "engine/renderer/vk.hh"
#include "engine/window/window.hh"

I32 main(I32 argc, I8** argv) {
    Args::parse(argc, argv);
    
    GLFWwindow* window = Window::create();

    vkInit(window);
    
    while(Window::isOpen(window)) {
        Window::update(window);

        vkDrawTriangle();
    }

    vkClean();

    Window::destroy(window);


    return 0;
}
