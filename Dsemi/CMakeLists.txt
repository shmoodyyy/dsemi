cmake_minimum_required(VERSION 3.22.1)

file(GLOB sources "src/*.cpp")
add_library(DSEMI STATIC ${sources})

set(DSEMI_LIB DSEMI CACHE PATH "dsemi lib cmake name")
set(DSEMI_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src CACHE PATH "dsemi include path")
target_include_directories(DSEMI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
