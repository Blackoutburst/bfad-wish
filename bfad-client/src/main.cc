#define GLFW_INCLUDE_VULKAN
#define LOGGER_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdio.h>

#include "utils/logger.hh"
#include "types.hh"
#include "utils/args.hh"
#include "renderer/vk.hh"
#include "window/window.hh"

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
