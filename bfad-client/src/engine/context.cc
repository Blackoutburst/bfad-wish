#include <stdlib.h>

#include "engine/context.hh"

#include "engine/renderer/rendererInstance.hh"
#include "engine/renderer/windowSurface.hh"
#include "engine/device/logicalDevice.hh"
#include "engine/device/physicalDevice.hh"
#include "engine/device/device.hh"
#include "engine/debug/logCallback.hh"
#include "engine/renderer/descriptorPool.hh"
#include "engine/device/commandPool.hh"
#include "engine/utils/args.hh"

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

        VkDescriptorPool descriptorPool = DescriptorPool::create(ctx);
        ctx->descriptorPool = descriptorPool;

        ctx->cmdPool = CommandPool::create(ctx);

        return ctx;
    }

    U0 destroy(Context::It* ctx) {
        CommandPool::destroy(ctx, ctx->cmdPool);
        DescriptorPool::destroy(ctx, ctx->descriptorPool);
        Device::destroy(ctx->device);
        WindowSurface::destroy(ctx);
        
        if (Args::getValidationLayers()) {
            LogCallback::destroy(ctx->instance, ctx->loggerCallback);
        }
        
        RendererInstance::destroy(ctx);
    
        free(ctx);
    }
}   
