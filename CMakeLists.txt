cmake_minimum_required(VERSION 3.22.1)

project(dsemi)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "root path")

add_subdirectory(dsemi)
add_subdirectory(devapp)
