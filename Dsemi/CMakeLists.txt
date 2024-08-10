cmake_minimum_required(VERSION 3.22.1)

set(VSHLSL
    "dsemi/graphics/shaders/directx/default_vs.hlsl"
)
set(FSHLSL
    "dsemi/graphics/shaders/directx/default_ps.hlsl"
)
set(HLSL ${VSHLSL} ${FSHLSL})
add_custom_target(
    SHADERS_DSEMI ALL
    DEPENDS ${HLSL}
)
set_source_files_properties(${VSHLSL} PROPERTIES ShaderType "vs")
set_source_files_properties(${FSHLSL} PROPERTIES ShaderType "ps")
set_source_files_properties(${VSHLSL} ${FSHLSL} PROPERTIES ShaderModel "4_0")

set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dsmei/graphics/shaders")
foreach(FILE ${HLSL})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(shadertype ${FILE} ShaderType)
    get_source_file_property(shadermodel ${FILE} ShaderModel)
    add_custom_command(TARGET SHADERS_DSEMI 
        COMMAND "C:/Program Files (x86)/Windows Kits/10/bin/10.0.22621.0/x86/fxc.exe" /nologo /Emain /T${shadertype}_${shadermodel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/shaders/${FILE_WE}.cso /Fd ${CMAKE_BINARY_DIR}/int/${FILE_WE}.pdb ${FILE}
        DEPENDS ${FILE}
        COMMENT "HLSL ${FILE}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM)
endforeach(FILE)

file(GLOB_RECURSE sources "dsemi/*.cpp")
add_library(DSEMI STATIC ${sources})
add_dependencies(DSEMI SHADERS_DSEMI)

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
