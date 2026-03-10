#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>

#include "utils/args.hh"
#include "debug/validationLayers.hh"
#include "debug/logCallback.hh"
#include "renderer/rendererInstance.hh"
#include "utils/logger.hh"
#include "utils/vkerror.hh"

namespace RendererInstance {
    U0 destroy(VkInstance instance) {
        vkDestroyInstance(instance, NULL);
    }

    VkInstance create(U0) {
        VkInstance instance;
    
        VkApplicationInfo appInfo;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = NULL;
        appInfo.pApplicationName = "VKMC";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        U32 glfwExtensionCount = 0;
        const I8** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        const I8** requiredExtensions = (const I8**)malloc((glfwExtensionCount + 2) * sizeof(char*));
        memcpy(requiredExtensions, glfwExtensions, glfwExtensionCount * sizeof(char*));
        requiredExtensions[glfwExtensionCount++] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
        requiredExtensions[glfwExtensionCount++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

        VkInstanceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        createInfo.pApplicationInfo = &appInfo;

        if (Args::getValidationLayers()) {
            VkDebugUtilsMessengerCreateInfoEXT* logCallback = LogCallback::getCreateInfo();
            createInfo.pNext = logCallback;
            createInfo.enabledLayerCount = ValidationLayer::count();
            createInfo.ppEnabledLayerNames = ValidationLayer::get();
        } else {
            createInfo.pNext = NULL;
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = NULL;
        }

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = requiredExtensions;

        VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
        if (result != VK_SUCCESS) {
            exit(1);
        }

        if (Args::getValidationLayers()) {
            logI("Enabled %i validation layers", ValidationLayer::count());
        }

        return instance;
    }
}
