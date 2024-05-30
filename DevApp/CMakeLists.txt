cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE sources "src/*.cpp")
add_executable(DEVAPP ${sources})

target_include_directories(DEVAPP PUBLIC ${DSEMI_PATH})
target_link_libraries(DEVAPP PRIVATE ${DSEMI_LIB})
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_compile_definitions(_DEBUG UNICODE _UNICODE _WIN32 _WIN64)
