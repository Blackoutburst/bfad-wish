#include "debug/validationLayers.hh"

#define LAYERS_COUNT 3

namespace ValidationLayer { 
    const static I8* layers[LAYERS_COUNT] = {
        //"VK_LAYER_LUNARG_api_dump",
        //"VK_LAYER_LUNARG_crash_diagnostic",
        //"VK_LAYER_LUNARG_gfxreconstruct",
        //"VK_LAYER_LUNARG_monitor",
        //"VK_LAYER_LUNARG_screenshot",
        //"VK_LAYER_KHRONOS_profiles",
        //"VK_LAYER_KHRONOS_timeline_semaphore",
        "VK_LAYER_KHRONOS_validation",
        "VK_LAYER_KHRONOS_shader_object",
        "VK_LAYER_KHRONOS_synchronization2",
        //"VK_LAYER_MESA_screenshot",
        //"VK_LAYER_MESA_vram_report_limit",
        //"VK_LAYER_MESA_overlay"
    };

    I32 count(void) {
        return LAYERS_COUNT;
    }

    const I8** get(void) {
        return layers;
    }
}
