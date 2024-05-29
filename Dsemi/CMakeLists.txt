cmake_minimum_required(VERSION 3.22.1)

file(GLOB sources "dsemi/*.cpp")
add_library(DSEMI STATIC ${sources})

set(DSEMI_LIB DSEMI CACHE PATH "dsemi lib cmake name")
set(DSEMI_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "dsemi include path")
set(DSEMI_PATH ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "dsemi include path")
target_include_directories(DSEMI PUBLIC ${DSEMI_INCLUDE})
