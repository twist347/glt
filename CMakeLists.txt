cmake_minimum_required(VERSION 3.20)

project(glt)

find_package(OpenGL REQUIRED)

set(CMAKE_C_STANDARD 11)

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)

set(GLFW_DIR ${EXTERNAL_DIR}/glfw-3.4)
set(GLAD_DIR ${EXTERNAL_DIR}/glad)
set(CGLM_DIR ${EXTERNAL_DIR}/cglm-0.9.6)

add_subdirectory(${GLFW_DIR} ${CMAKE_BINARY_DIR}/_deps/glfw-build EXCLUDE_FROM_ALL)
add_subdirectory(${CGLM_DIR} ${CMAKE_BINARY_DIR}/_deps/cglm-build EXCLUDE_FROM_ALL)

add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(
        glad PUBLIC
        ${GLAD_DIR}/include
        ${EXTERNAL_DIR}
)
if (NOT TARGET glad::glad)
    add_library(glad::glad ALIAS glad)
endif ()

add_library(glt STATIC
        src/glt_window.c
        src/glt_shader.c
        src/glt_vertex_buffer.c
        src/glt_vertex_array.c
        src/glt_texture.c
        src/glt_info.c
        src/glt_log.c
        src/glt_math.c
)

target_include_directories(glt PUBLIC
        include
)

target_link_libraries(glt PUBLIC glad glfw OpenGL::GL)

target_compile_options(glt PRIVATE -Wall -Wextra -Wpedantic)

add_subdirectory(${EXAMPLES_DIR})
