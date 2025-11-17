cmake_minimum_required(VERSION 3.20)

project(cbox_graphics
    VERSION 1.0.0
    DESCRIPTION "Graphics rendering and visualization"
    LANGUAGES C CXX
)

file(GLOB_RECURSE GRAPHICS_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/backends/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/*.hpp"
)

file(GLOB_RECURSE GRAPHICS_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/*.cpp"
)

add_module(graphics
    SOURCES
        ${GRAPHICS_SOURCES}
    HEADERS
        ${GRAPHICS_HEADERS}
    DEPENDENCIES
        cbox::core
        cbox::math
        glfw
        glad
        stb::stb
        shaderc_combined
        glslang
        SPIRV-Tools
        SPIRV-Tools-opt
        spirv-cross-cpp 
        spirv-cross-glsl 
        spirv-cross-hlsl 
        spirv-cross-msl 
        spirv-cross-reflect
)

target_include_directories(cbox_graphics
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(cbox_graphics
    PRIVATE
        CBOX_GRAPHICS_VERSION="${PROJECT_VERSION}"
)
