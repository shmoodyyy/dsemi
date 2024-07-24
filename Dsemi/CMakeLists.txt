cmake_minimum_required(VERSION 3.22.1)

file(GLOB_RECURSE sources "dsemi/*.cpp")
add_library(DSEMI STATIC ${sources})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(DSEMI_LIB DSEMI CACHE PATH "dsemi lib cmake name")
set(DSEMI_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "dsemi include path")
set(DSEMI_PATH ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "dsemi include path")
set(DSEMI_WIN_LIBS 
    d2d1
    d3d11
    d3d11.lib
    d3dcompiler.lib
    dwrite
    dxgi.lib
    dxguid.lib
    CACHE PATH "dsemi windows libs")

target_include_directories(DSEMI PRIVATE ${DSEMI_PATH})

target_link_libraries(DSEMI PRIVATE ${DSEMI_WIN_LIBS})

add_compile_definitions(_DEBUG _WIN32 _WIN64)
