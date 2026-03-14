#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "engine/utils/args.hh"

static U8 validationLayerEnabled = 0;

static const I8* args[4] = {
    "-h", "--help",
    "-d", "--debug",
};

namespace Args {
    U0 parse(I32 argc, I8** argv) {
        for (I32 i = 1; i < argc; i++) {
            if (!strcmp(argv[i], args[0]) || !strcmp(argv[i], args[1])) {
                help();
            }
            if (!strcmp(argv[i], args[2]) || !strcmp(argv[i], args[3])) {
                validationLayers();
            }
        }
    }

    U8 getValidationLayers(U0) {
        return validationLayerEnabled;
    }

    U0 validationLayers(U0) {
        validationLayerEnabled = 1;
    }

    U0 help(U0) {
        const I8* help = "==============================================\n"
                        "[Minecraft Wish client]\n\n"
                        "-h, --help:\n"
                        "\tShow this message\n\n"
                        "-d, --debug:"
                        "\tEnable Vulkan validation layers\n\n"
                        "==============================================\n";

        printf("%s", help);
        exit(0);
    }
}
