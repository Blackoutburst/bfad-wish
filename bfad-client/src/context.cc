#include <stdlib.h>

#include "context.hh"

#include "renderer/rendererInstance.hh"
#include "renderer/windowSurface.hh"
#include "devices/logicalDevice.hh"
#include "devices/physicalDevice.hh"
#include "devices/devices.hh"
#include "debug/logCallback.hh"
#include "utils/args.hh"

namespace Context {

    Context::It* create(GLFWwindow* window) {
        VkInstance instance = RendererInstance::create();
        VkDebugUtilsMessengerEXT loggerCallback = NULL;

        if (Args::getValidationLayers()) {
            loggerCallback = LogCallback::create(instance);
        }
        
        VkSurfaceKHR windowSurface = WindowSurface::create(instance, window);

        VkPhysicalDevice physical = PhysicalDevice::create(instance, windowSurface);
        VkDevice logical = LogicalDevice::create(physical, windowSurface);
        Device::It* device = Device::create(physical, logical, windowSurface);
        
        Context::It* ctx = (Context::It*)malloc(sizeof(Context::It));
        ctx->window = window;
        ctx->instance = instance;
        ctx->device = device;
        ctx->surface = windowSurface;
        ctx->loggerCallback = loggerCallback;

        return ctx;
    }

    U0 destroy(Context::It* ctx) {
        Device::destroy(ctx->device);
        WindowSurface::destroy(ctx);
        
        if (Args::getValidationLayers()) {
            LogCallback::destroy(ctx->instance, ctx->loggerCallback);
        }
        
        RendererInstance::destroy(ctx);
    
        free(ctx);
    }
}   
