@echo off
set NAME=vulkan
set SRC=src\*.cc src\engine\*.cc src\engine\renderer\*.cc src\engine\window\*.cc src\engine\utils\*.cc src\engine\debug\*.cc src\engine\device\*.cc src\engine\math\*.cc src\engine\texture\*.cc src\object\*.cc
set INCLUDE=-Iinclude

set I_VK=C:\VulkanSDK\1.4.341.1\Include
set L_VK=C:\VulkanSDK\1.4.341.1\Lib
set VK=-I"%I_VK%" -L"%L_VK%" -lvulkan-1

set I_GLFW=C:\GLFW\include
set L_GLFW=C:\GLFW\lib-mingw-w64
set GLFW=-I"%I_GLFW%" -L"%L_GLFW%" -lglfw3dll

set F_ERROR=-Wall -Wextra -Wpedantic
set F_DEBUG=-g3 -fno-omit-frame-pointer -fsanitize=address -fsanitize-address-use-after-return=always
set F_DISABLED=

rem CLANG
echo CLANG:
clang++ -o %NAME% %SRC% %INCLUDE% %VK% %GLFW% %F_ERROR% %F_DEBUG% %F_DISABLED%
set CLANG_EXIT=%ERRORLEVEL%

if %CLANG_EXIT% neq 0 (
    echo FAILED
) else (
    echo OK
)

