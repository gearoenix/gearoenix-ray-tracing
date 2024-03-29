CMAKE_MINIMUM_REQUIRED(VERSION 3.18.1 FATAL_ERROR)

PROJECT(Gearoenix)

SET(PROJECT_DESCRIPTION "A cross-platform C++ game engine.")

SET(GX_VERSION_MAJOR 0)
SET(GX_VERSION_MINOR 3) # RTX enabled version
SET(GX_VERSION_PATCH 0)
SET(GX_VERSION "${GX_VERSION_MAJOR}.${GX_VERSION_MINOR}.${GX_VERSION_PATCH}")

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
OPTION(GX_TEST_ENABLED "Enables test units" ON)
# Test coverage
OPTION(GX_COVERAGE_ENABLED "Enables the measurement of test coverages" OFF)

IF (CMAKE_SYSTEM_NAME MATCHES "Android")
    SET(GX_IN_ANDROID TRUE)
    ADD_LIBRARY(native_app_glue
            STATIC
            ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
    INCLUDE_DIRECTORIES(${ANDROID_NDK}/sources/android/native_app_glue)
    SET(CMAKE_SHARED_LINKER_FLAGS
            "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
    SET(GX_COMMON_LIBS ${GX_COMMON_LIBS} android native_app_glue EGL log GLESv3)
    ADD_DEFINITIONS(-DIMGUI_IMPL_OPENGL_ES3=1)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    SET(GX_IN_WASM)
    MESSAGE(STATUS "Building for Emscripten.")
    SET(GX_EMS_FLAGS "-s WASM=1 -s NO_EXIT_RUNTIME=1 -s BINARYEN=1 -s USE_SDL=2 --no-heap-copy -Os -s DISABLE_EXCEPTION_CATCHING=0")
    SET(GX_EMS_FLAGS "${GX_EMS_FLAGS} -s INITIAL_MEMORY=268435456 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s LLD_REPORT_UNDEFINED")
    SET(GX_EMS_FLAGS "${GX_EMS_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/assets@/")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GX_EMS_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GX_EMS_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GX_EMS_FLAGS}")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${GX_EMS_FLAGS}")
    SET(CMAKE_EXECUTABLE_SUFFIX .html)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET(GX_IN_LINUX TRUE)
ENDIF()

IF (GX_COVERAGE_ENABLED AND GX_TEST_ENABLED)
    MESSAGE(STATUS "Coverage enabled")
    SET(GX_COVERAGE_COMPILER_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GX_COVERAGE_COMPILER_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GX_COVERAGE_COMPILER_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GX_COVERAGE_COMPILER_FLAGS}")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${GX_COVERAGE_COMPILER_FLAGS}")
ENDIF ()

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)
INCLUDE(gx-compile-lib)
INCLUDE(gx-download-file)
INCLUDE(gx-flag-util)
INCLUDE(gx-imgui)

MESSAGE(STATUS "Gearoenix is going to be compiled for ${CMAKE_SYSTEM_NAME}")

IF (MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    SET(CMAKE_CXX_FLAGS "/std:c++latest /MP /arch:AVX2 /fp:fast /Oi ${CMAKE_CXX_FLAGS}")
    ADD_COMPILE_OPTIONS("$<$<CONFIG:RELEASE>:/Ot>")
    ADD_COMPILE_OPTIONS("$<$<CONFIG:RELEASE>:/Ox>")
    # ADD_COMPILE_OPTIONS("$<$<CONFIG:RELEASE>:/GL>")
    GX_REPLACE_FLAG(CMAKE_CXX_FLAGS "/W3" "/W4")
    SET(GEAROENIX_EXE_TARGET "WIN32")
ELSE ()
    SET(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_CXX_FLAGS}")
ENDIF ()

SET(GX_IN_IOS OFF)

IF (APPLE)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")
    IF (CMAKE_SYSTEM_NAME MATCHES "iOS")
        SET(GX_IN_IOS ON)
    ENDIF (CMAKE_SYSTEM_NAME MATCHES "iOS")
    MESSAGE(STATUS "Compilation for iOS activation status: ${GX_IN_IOS}")
ENDIF (APPLE)

IF (GX_IN_LINUX)
    SET(GX_LIBS ${GX_LIBS} dl)
ENDIF (GX_IN_LINUX)

SET(GX_SDK_PATH "${CMAKE_SOURCE_DIR}/sdk")
SET(GX_SDK_LIB_PATH "${GX_SDK_PATH}/libs/")

SET(GX_UNCOMPRESS_TOP_FOLDER "ON")

SET(GX_SDL2_PATH "${GX_SDK_PATH}/sdl2")
GX_DOWNLOAD_FILE("https://www.libsdl.org/release/SDL2-2.0.22.zip" "${GX_SDL2_PATH}")
GX_COMPILE_LIB(${GX_SDL2_PATH})
SET(GX_SDL2_INCLUDE_DIRS "${GX_SDL2_PATH}/include")
INCLUDE_DIRECTORIES(${GX_SDL2_INCLUDE_DIRS})
SET(GX_SDL2_BUILD_DIRS "${GX_SDL2_PATH}/gx-build")

IF(LINUX)
    SET(GX_COMMON_LIBS
            ${GX_COMMON_LIBS}
            ${GX_SDL2_BUILD_DIRS}/libSDL2.a
            ${GX_SDL2_BUILD_DIRS}/libSDL2main.a
            m dl asound pthread pulse-simple pulse X11 Xext Xcursor Xinerama Xi Xrandr Xss Xxf86vm drm gbm
            wayland-egl wayland-client wayland-cursor xkbcommon rt)
ELSEIF(MSVC)
    SET(GX_COMMON_LIBS 
            ${GX_COMMON_LIBS}
            winmm version oleaut32 imm32 setupapi d3d12 dxgi dxguid)
    SET(GX_DEBUG_LIBS
            ${GX_DEBUG_LIBS}
            ${GX_SDL2_BUILD_DIRS}/Debug/SDL2-staticd.lib
            ${GX_SDL2_BUILD_DIRS}/Debug/SDL2maind.lib)
    SET(GX_RELEASE_LIBS
            ${GX_RELEASE_LIBS}
            ${GX_SDL2_BUILD_DIRS}/Release/SDL2-static.lib
            ${GX_SDL2_BUILD_DIRS}/Release/SDL2main.lib)
ELSEIF(APPLE)
    SET(GX_COMMON_LIBS ${GX_COMMON_LIBS} iconv)
ENDIF()

SET(GX_DEBUG_LIBS ${GX_DEBUG_LIBS} ${GX_COMMON_LIBS})
SET(GX_RELEASE_LIBS ${GX_RELEASE_LIBS} ${GX_COMMON_LIBS})

SET(GX_STB_PATH "${GX_SDK_PATH}/stb")
GX_DOWNLOAD_FILE("https://github.com/nothings/stb/archive/master.zip" "${GX_STB_PATH}")
SET(GX_STB_INCLUDE_DIRS "${GX_STB_PATH}")
INCLUDE_DIRECTORIES(${GX_STB_INCLUDE_DIRS})

SET(GX_D3D_PATH "${GX_SDK_PATH}/d3d")
GX_DOWNLOAD_FILE("https://github.com/microsoft/DirectX-Headers/archive/refs/heads/main.zip" "${GX_D3D_PATH}")
SET(GX_D3D_INCLUDE_DIRS "${GX_D3D_PATH}/include/directx")
INCLUDE_DIRECTORIES(${GX_D3D_INCLUDE_DIRS})

SET(GX_VULKAN_HEADERS_PATH "${GX_SDK_PATH}/vulkan-headers")
GX_DOWNLOAD_FILE("https://github.com/KhronosGroup/Vulkan-Headers/archive/master.zip" "${GX_VULKAN_HEADERS_PATH}")
SET(GX_VULKAN_HEADERS_INCLUDE_DIRS "${GX_VULKAN_HEADERS_PATH}/include")
INCLUDE_DIRECTORIES(${GX_VULKAN_HEADERS_INCLUDE_DIRS})

#SET(GX_BULLET3_PATH "${GX_SDK_PATH}/bullet3")
#GX_DOWNLOAD_FILE("https://github.com/bulletphysics/bullet3/archive/master.zip" "${GX_BULLET3_PATH}")
#GX_COMPILE_LIB(${GX_BULLET3_PATH})

SET(GX_IMGUI_PATH "${GX_SDK_PATH}/imgui")
GX_DOWNLOAD_FILE("https://github.com/ocornut/imgui/archive/refs/tags/v1.87.zip" "${GX_IMGUI_PATH}")
GX_PREPARE_IMGUI(${GX_IMGUI_PATH})
INCLUDE_DIRECTORIES(${GX_IMGUI_PATH})
INCLUDE_DIRECTORIES(${GX_IMGUI_PATH}/backends)
INCLUDE_DIRECTORIES(${GX_IMGUI_PATH}/misc/cpp)

SET(GX_IMGUI_FB_PATH "${GX_SDK_PATH}/imgui-fb")
GX_DOWNLOAD_FILE("https://github.com/aiekick/ImGuiFileDialog/archive/Lib_Only.zip" "${GX_IMGUI_FB_PATH}")
GX_PREPARE_IMGUI_FB(${GX_IMGUI_FB_PATH})
INCLUDE_DIRECTORIES(${GX_IMGUI_FB_PATH})

SET(GX_BOOST_PATH "${GX_SDK_PATH}/boost")
GX_DOWNLOAD_FILE("https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.zip" "${GX_BOOST_PATH}")
FILE(GLOB GX_BOOST_UNWANTED_FILES ${GX_BOOST_PATH}/*)
LIST(REMOVE_ITEM GX_BOOST_UNWANTED_FILES "${GX_BOOST_PATH}/boost")
FOREACH(GX_BOOST_UNWANTED ${GX_BOOST_UNWANTED_FILES})
    FILE(REMOVE_RECURSE ${GX_BOOST_UNWANTED})
ENDFOREACH()
INCLUDE_DIRECTORIES(${GX_BOOST_PATH})

SET(GX_TINYGLTF_PATH "${GX_SDK_PATH}/tinygltf")
GX_DOWNLOAD_FILE("https://github.com/syoyo/tinygltf/archive/master.zip" "${GX_TINYGLTF_PATH}")
ADD_COMPILE_DEFINITIONS("TINYGLTF_USE_CPP14")
FILE(GLOB GX_TINYGLTF_UNWANTED_FILES ${GX_TINYGLTF_PATH}/*)
LIST(REMOVE_ITEM GX_TINYGLTF_UNWANTED_FILES "${GX_TINYGLTF_PATH}/tiny_gltf.h")
LIST(REMOVE_ITEM GX_TINYGLTF_UNWANTED_FILES "${GX_TINYGLTF_PATH}/json.hpp")
FOREACH(GX_TINYGLTF_UNWANTED ${GX_TINYGLTF_UNWANTED_FILES})
    FILE(REMOVE_RECURSE ${GX_TINYGLTF_UNWANTED})
ENDFOREACH()
INCLUDE_DIRECTORIES(${GX_TINYGLTF_PATH})

SET(GX_DXC_PATH "${GX_SDK_PATH}/dxc")
SET(GX_UNCOMPRESS_TOP_FOLDER "OFF")
GX_DOWNLOAD_FILE("https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.6.2106/dxc_2021_07_01.zip" "${GX_DXC_PATH}")
SET(GX_DXC_BINARY_DIR "${GX_DXC_PATH}/bin/x64")
SET(GX_UNCOMPRESS_TOP_FOLDER "ON")

ADD_SUBDIRECTORY(gearoenix)
ADD_SUBDIRECTORY(editor)
ADD_SUBDIRECTORY(examples)

IF(GX_TEST_ENABLED)
    ADD_SUBDIRECTORY(tests)
ENDIF(GX_TEST_ENABLED)