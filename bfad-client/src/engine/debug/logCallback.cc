#include <stdlib.h>
#include <stdio.h>

#include "engine/logger.h"

#include "engine/debug/logCallback.hh"

// Create Info is never free can't be fuck doing that 
// it will only lead to minimal memory loss if validation layers are enabled

namespace LogCallback {
    U0 destroy(VkInstance instance, VkDebugUtilsMessengerEXT logger) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

        if (func && logger != NULL) {
            func(instance, logger, NULL);
        }
    }

    VkDebugUtilsMessengerCreateInfoEXT* getCreateInfo(U0) {
        VkDebugUtilsMessengerCreateInfoEXT* createInfo;
    
        createInfo = (VkDebugUtilsMessengerCreateInfoEXT*)malloc(sizeof(VkDebugUtilsMessengerCreateInfoEXT));
        createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo->pNext = NULL;
        createInfo->flags = 0;
        createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo->pfnUserCallback = logCallback;
        createInfo->pUserData = NULL;
        
        return createInfo;
    }

    VkDebugUtilsMessengerEXT create(VkInstance instance) {
        VkDebugUtilsMessengerEXT logger = NULL;
        PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

        if (func) {
            func(instance, getCreateInfo(), NULL, &logger);
        }

        return logger;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL logCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        UNUSED_VAR(messageType);
        UNUSED_VAR(pUserData);

        switch (messageSeverity) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: logT("%s", pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: logI("%s", pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: logW("%s", pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: logE("%s", pCallbackData->pMessage); break;
            default: logMsg("%s", pCallbackData->pMessage);
        }

        return VK_FALSE;
    }
}
