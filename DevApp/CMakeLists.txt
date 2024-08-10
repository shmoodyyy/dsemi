cmake_minimum_required(VERSION 3.22.1)


add_custom_target(DEVAPP_COPYSHADERS
    DEPENDS DSEMI_SHADERS)

set(SHADER_FILES
    "default_vs.cso"
    "default_ps.cso"
)
set(SHADER_PATHS)
foreach(FILE ${SHADER_FILES})
    set(SHADER_PATHS ${SHADER_PATHS} "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/shaders/${FILE}")
    add_custom_command(TARGET DEVAPP_COPYSHADERS POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/shaders/${FILE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/shaders/${FILE}"
        COMMENT "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/shaders/${FILE}"
        DEPENDS DSEMI_SHADERS ALWAYS_REBUILD "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/shaders/${FILE}")
endforeach(FILE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE sources "src/*.cpp")
add_executable(DEVAPP ${sources})
add_dependencies(DEVAPP DEVAPP_COPYSHADERS)

target_include_directories(DEVAPP PUBLIC ${DSEMI_PATH})
target_link_libraries(DEVAPP PRIVATE ${DSEMI_LIB})
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_compile_definitions(_DEBUG _WIN32 _WIN64)
