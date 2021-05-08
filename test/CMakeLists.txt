cmake_minimum_required (VERSION 3.13)

project(test)

file(GLOB_RECURSE test_cpp src/*.cpp)
file(GLOB_RECURSE test_hpp include/*.hpp)

add_executable(${PROJECT_NAME} ${test_cpp})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

#target_compile_features(${PROJECT_NAME} PUBLIC
#    cxx_std_17
#)

target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    find_package(parser REQUIRED)
endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
target_link_libraries(${PROJECT_NAME} PMX2TXT::parser)

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
