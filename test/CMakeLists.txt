cmake_minimum_required (VERSION 3.14)

project(test)

file(GLOB_RECURSE test_cpp src/*.cpp)
file(GLOB_RECURSE test_hpp include/*.hpp)

add_executable(${PROJECT_NAME} ${test_cpp})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CMAKE_CXX_STANDARD 17
    CMAKE_CXX_STANDARD_REQUIRED ON
    CMAKE_CXX_EXTENSIONS OFF
)

target_link_libraries(${PROJECT_NAME} parser)

### Filter Settings for Visual Studio
# Build the visual studio filter same as source directories form.
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)
assign_source_group(${test_cpp})
assign_source_group(${test_hpp})
