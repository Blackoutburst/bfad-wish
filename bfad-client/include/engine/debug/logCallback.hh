#pragma once

#include <vulkan/vulkan.h>

#include "engine/types.hh"

namespace LogCallback {
    U0 destroy(VkInstance instance, VkDebugUtilsMessengerEXT logger);
    VkDebugUtilsMessengerCreateInfoEXT* getCreateInfo(U0);
    VkDebugUtilsMessengerEXT create(VkInstance instance);
    VKAPI_ATTR VkBool32 VKAPI_CALL logCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, U0* pUserData);
}
