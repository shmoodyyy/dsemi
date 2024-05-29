cmake_minimum_required(VERSION 3.22.1)

file(GLOB sources "src/*.cpp")
add_executable(DEVAPP ${sources})

target_link_libraries(DEVAPP ${DSEMI_LIB})
target_include_directories(DEVAPP PUBLIC ${DSEMI_PATH})
