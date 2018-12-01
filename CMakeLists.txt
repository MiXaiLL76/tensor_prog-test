cmake_minimum_required(VERSION 2.8)

SET(exec_name analyser)
SET(CMAKE_BUILD_TYPE Release)

project(${exec_name})

add_definitions(-std=c++11 -O3 -Wall)
add_definitions(-Wno-reorder)

message(STATUS "Build analyser app")

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost 1.63.0 COMPONENTS filesystem regex system program_options)

if(Boost_FOUND)
     include_directories(${Boost_INCLUDE_DIRS}) 
     link_directories(${Boost_LIBRARY_DIR})
endif()

add_executable(${exec_name} ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_library(file_tree ${PROJECT_SOURCE_DIR}/src/FileTree.cpp)

target_link_libraries(${exec_name} file_tree ${Boost_LIBRARIES})