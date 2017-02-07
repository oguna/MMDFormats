cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH_BACKUP ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

find_package(ICU COMPONENTS uc io REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH_BACKUP})

project(MikuMikuFormats)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${ICU_INCLUDE_DIRS})

add_library(pmx Pmx.cpp)
target_link_libraries(pmx PRIVATE ${ICU_LIBRARIES})

add_executable(demo test.cpp)
target_link_libraries(demo pmx)
