# cbox::math Module
cmake_minimum_required(VERSION 3.30)

project(cbox_math
    VERSION 1.0.0
    DESCRIPTION "Mathematics and linear algebra module for cbox"
    LANGUAGES CXX
)


file(GLOB_RECURSE MATH_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

file(GLOB_RECURSE MATH_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)


add_module(math
    SOURCES
        ${MATH_SOURCES}
    HEADERS
        ${MATH_HEADERS}
    DEPENDENCIES
    
)


target_compile_definitions(cbox_math
    PRIVATE
        CBOX_MATH_VERSION="${PROJECT_VERSION}"
)


