#!/bin/bash
readonly NAME="vulkan"
readonly SRC="src/*.cc src/renderer/*.cc src/window/*.cc src/utils/*.cc src/debug/*.cc src/devices/*.cc"
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
clang++ -o $NAME $SRC $INCLUDE $VK $GLFW $F_ERROR $F_DEBUG $F_DISABLED $FRAMEWORKS
CLANG_EXIT=$?

if [ $CLANG_EXIT -ne 0 ]; then
    printf "\e[91mFAILED\e[0m\n"
else
    printf "\e[92mOK\e[0m\n"
fi

