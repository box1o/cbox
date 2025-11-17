cmake_minimum_required(VERSION 3.5)

find_or_fetch_package(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)

find_or_fetch_package(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)

find_or_fetch_package(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
)

add_subdirectory(glad)
add_subdirectory(stb)

find_or_fetch_package(spirv-cross
    GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git
    GIT_TAG main 
)

set(SHADERC_SKIP_TESTS ON)
set(SHADERC_GITHUB_REPO "https://github.com/google/shaderc.git")
set(SHADERC_SUBMODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaderc")

if(NOT EXISTS "${SHADERC_SUBMODULE_DIR}")
    message(STATUS "Cloning Shaderc from ${SHADERC_GITHUB_REPO}")
    execute_process(
        COMMAND git clone --recurse-submodules ${SHADERC_GITHUB_REPO} ${SHADERC_SUBMODULE_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_CLONE_RESULT
    )
    if(NOT GIT_CLONE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to clone Shaderc repository. Git returned ${GIT_CLONE_RESULT}.")
    endif()
else()
    message(STATUS "Shaderc directory already exists: ${SHADERC_SUBMODULE_DIR}")
endif()

if(EXISTS ${SHADERC_SUBMODULE_DIR} AND NOT EXISTS "${SHADERC_SUBMODULE_DIR}/third_party/spirv-tools/")
    message(STATUS "Running git-sync-deps at ${SHADERC_SUBMODULE_DIR}/utils/git-sync-deps")
    execute_process(
        COMMAND ${SHADERC_SUBMODULE_DIR}/utils/git-sync-deps
    )
endif()
