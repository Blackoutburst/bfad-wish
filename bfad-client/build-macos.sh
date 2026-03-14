#!/bin/bash
readonly NAME="vulkan"
readonly ENGINE_SRC="src/engine/*.cc src/engine/renderer/*.cc src/engine/window/*.cc src/engine/utils/*.cc src/engine/debug/*.cc src/engine/device/*.cc src/engine/math/*.cc src/engine/texture/*.cc"
readonly SRC="src/*.cc src/object/*.cc"
readonly INCLUDE="-Iinclude"

readonly I_VK="/opt/homebrew/include"
readonly L_VK="/opt/homebrew/lib"
readonly VK="-isystem $I_VK -L$L_VK -lvulkan -Wl,-rpath,$L_VK"

readonly I_GLFW="/opt/homebrew/include"
readonly L_GLFW="/opt/homebrew/lib"
readonly GLFW="-I$I_GLFW -L$L_GLFW -lglfw3"

readonly F_ERROR="-Wall -Wextra -Wpedantic"
readonly F_DEBUG="-g3 -fno-omit-frame-pointer -fsanitize=address -fsanitize-address-use-after-return=always"
readonly F_DISABLED=""
readonly FRAMEWORKS="-framework Cocoa -framework IOKit"

### CLANG ###

printf "\e[94mCLANG\e[0m: "
clang++ -o $NAME $ENGINE_SRC $SRC $INCLUDE $VK $GLFW $F_ERROR $F_DEBUG $F_DISABLED $FRAMEWORKS
CLANG_EXIT=$?

if [ $CLANG_EXIT -ne 0 ]; then
    printf "\e[91mFAILED\e[0m\n"
else
    printf "\e[92mOK\e[0m\n"
fi

slangc shader/cube.slang -entry vertMain -stage vertex -target spirv -o shader/cubeVert.spv
slangc shader/cube.slang -entry fragMain -stage fragment -target spirv -o shader/cubeFrag.spv
slangc shader/quad.slang -entry vertMain -stage vertex -target spirv -o shader/quadVert.spv
slangc shader/quad.slang -entry fragMain -stage fragment -target spirv -o shader/quadFrag.spv

