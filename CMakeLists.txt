cmake_minimum_required(VERSION 3.11)

project(my_emdw_app)

message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Build  directory: ${CMAKE_BINARY_DIR}")
message(STATUS "Configuring ${PROJECT_TITLE} ...")

#define CMAKE_MODULE_PATH to use some of our macros and functions...
#and include some of our functions
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
include(MacroOutOfSourceBuild)
include(FetchContentHelpers)
include(ShowCmakeVariables)

macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build.")

# Required boost support
find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

# Required package gLinear and add its include directory
find_package(gLinear REQUIRED)
include_directories(${GLINEAR_INCLUDE_DIRS})

# Required gLinear support
find_package(prlite REQUIRED)
include_directories(${PRLITE_INCLUDE_DIRS})

# Required package emdw and add its include directory
find_package(emdw REQUIRED)
include_directories(${EMDW_INCLUDE_DIRS})

#****************************************************************
# Our main project targets
#****************************************************************
add_subdirectory(src)
