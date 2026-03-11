#include <stdlib.h>

#include "context.hh"

namespace Context {

    Context::It* create(GLFWwindow* window, VkInstance instance, Device::It* device, VkSurfaceKHR surface) {
        Context::It* context = (Context::It*)malloc(sizeof(Context::It));
        context->window = window;
        context->instance = instance;
        context->device = device;
        context->surface = surface;

        return context;
    }

    U0 destroy(Context::It* context) {
        free(context);
    }
}   
