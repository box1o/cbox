include(FetchContent)

function(find_or_fetch_package package_name)
    set(options REQUIRED)
    set(one_value_args GIT_REPOSITORY GIT_TAG)
    set(multi_value_args FIND_PACKAGE_ARGS)
    
    cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
    
    find_package(${package_name} ${ARG_FIND_PACKAGE_ARGS} QUIET)
    
    if(${package_name}_FOUND)
        message(STATUS "Found ${package_name} (system)")
    else()
        if(ARG_GIT_REPOSITORY)
            message(STATUS "Fetching ${package_name} from ${ARG_GIT_REPOSITORY}")
            
            FetchContent_Declare(
                ${package_name}
                GIT_REPOSITORY ${ARG_GIT_REPOSITORY}
                GIT_TAG ${ARG_GIT_TAG}
                GIT_SHALLOW TRUE
            )
            
            FetchContent_MakeAvailable(${package_name})
            
        elseif(ARG_REQUIRED)
            message(FATAL_ERROR "${package_name} not found and no GIT_REPOSITORY provided")
        else()
            message(WARNING "${package_name} not found")
        endif()
    endif()
endfunction()
