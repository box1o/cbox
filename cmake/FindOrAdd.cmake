if(NOT DEFINED ROOT_DIR)
    set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

include(FetchContent)

set(CBOX_DEPS_ROOT        "${ROOT_DIR}/3rdparty/.deps" CACHE PATH "")
set(FETCHCONTENT_BASE_DIR "${CBOX_DEPS_ROOT}"          CACHE PATH "")

message(STATUS "Deps directory:              ${CBOX_DEPS_ROOT}")
message(STATUS "FetchContent base directory: ${FETCHCONTENT_BASE_DIR}")

function(find_or_fetch_package package_name)
    set(options REQUIRED RECURSE_SUBMODULES NO_SHALLOW DISABLE_INSTALL)
    set(one_value_args GIT_REPOSITORY GIT_TAG POST_FETCH_SCRIPT)
    set(multi_value_args FIND_PACKAGE_ARGS POST_FETCH_COMMANDS CMAKE_CACHE_VARS)

    cmake_parse_arguments(ARG "${options}" "${one_value_args}"
                          "${multi_value_args}" ${ARGN})

    find_package(${package_name} ${ARG_FIND_PACKAGE_ARGS} QUIET)

    if(${package_name}_FOUND)
        message(STATUS "Found ${package_name} (system)")
        return()
    endif()

    if(NOT ARG_GIT_REPOSITORY)
        if(ARG_REQUIRED)
            message(FATAL_ERROR
                "${package_name} not found and no GIT_REPOSITORY provided")
        endif()
        message(WARNING "${package_name} not found")
        return()
    endif()

    message(STATUS "Fetching ${package_name} from ${ARG_GIT_REPOSITORY}")

    set(PKG_SOURCE_DIR "${CBOX_DEPS_ROOT}/${package_name}-src")

    set(FETCH_ARGS
        GIT_REPOSITORY ${ARG_GIT_REPOSITORY}
        SOURCE_DIR     ${PKG_SOURCE_DIR}
    )

    if(ARG_GIT_TAG)
        list(APPEND FETCH_ARGS GIT_TAG ${ARG_GIT_TAG})
    endif()

    if(ARG_RECURSE_SUBMODULES)
        list(APPEND FETCH_ARGS GIT_SUBMODULES_RECURSE TRUE)
    elseif(NOT ARG_NO_SHALLOW)
        list(APPEND FETCH_ARGS GIT_SHALLOW TRUE)
    endif()

    if(ARG_CMAKE_CACHE_VARS)
        foreach(cache_var ${ARG_CMAKE_CACHE_VARS})
            if(cache_var MATCHES "^([^=]+)=(.*)$")
                set(${CMAKE_MATCH_1} ${CMAKE_MATCH_2} CACHE INTERNAL "")
            endif()
        endforeach()
    endif()

    FetchContent_Declare(${package_name} ${FETCH_ARGS})

    FetchContent_GetProperties(${package_name})
    string(TOLOWER ${package_name} package_name_lower)
    
    if(NOT ${package_name_lower}_POPULATED)
        FetchContent_Populate(${package_name})
        
        if(ARG_POST_FETCH_SCRIPT)
            set(SCRIPT_PATH "${${package_name_lower}_SOURCE_DIR}/${ARG_POST_FETCH_SCRIPT}")
            if(EXISTS "${SCRIPT_PATH}")
                message(STATUS "Running post-fetch script: ${ARG_POST_FETCH_SCRIPT}")
                execute_process(
                    COMMAND ${SCRIPT_PATH}
                    WORKING_DIRECTORY ${${package_name_lower}_SOURCE_DIR}
                    RESULT_VARIABLE SCRIPT_RESULT
                )
                if(NOT SCRIPT_RESULT EQUAL 0)
                    message(FATAL_ERROR "Post-fetch script failed: ${ARG_POST_FETCH_SCRIPT}")
                endif()
            else()
                message(WARNING "Post-fetch script not found: ${SCRIPT_PATH}")
            endif()
        endif()

        if(ARG_POST_FETCH_COMMANDS)
            foreach(cmd ${ARG_POST_FETCH_COMMANDS})
                message(STATUS "Running post-fetch command: ${cmd}")
                separate_arguments(cmd_list UNIX_COMMAND "${cmd}")
                execute_process(
                    COMMAND ${cmd_list}
                    WORKING_DIRECTORY ${${package_name_lower}_SOURCE_DIR}
                    RESULT_VARIABLE CMD_RESULT
                )
                if(NOT CMD_RESULT EQUAL 0)
                    message(FATAL_ERROR "Post-fetch command failed: ${cmd}")
                endif()
            endforeach()
        endif()

        if(ARG_DISABLE_INSTALL)
            set(CMAKE_SKIP_INSTALL_RULES TRUE)
            set(SKIP_INSTALL_ALL TRUE)
        endif()

        add_subdirectory(${${package_name_lower}_SOURCE_DIR} ${${package_name_lower}_BINARY_DIR} EXCLUDE_FROM_ALL)
        
        if(ARG_DISABLE_INSTALL)
            set(CMAKE_SKIP_INSTALL_RULES FALSE)
            set(SKIP_INSTALL_ALL FALSE)
        endif()
    endif()
endfunction()
