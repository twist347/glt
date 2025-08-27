cmake_minimum_required(VERSION 3.20)

project(glt)

find_package(OpenGL REQUIRED)

set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.4)
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glad)

add_subdirectory(${GLFW_DIR} ${CMAKE_BINARY_DIR}/_deps/glfw-build EXCLUDE_FROM_ALL)

add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)
if (NOT TARGET glad::glad)
    add_library(glad::glad ALIAS glad)
endif ()

add_library(glt STATIC
        src/glt_window.c
        src/glt_glad_wrapper.c
        src/glt_shader.c
        src/glt_vertex_buffer.c
        src/glt_vertex_array.c
        src/glt_info.c
)

target_include_directories(glt PUBLIC
        include
)

target_link_libraries(glt PUBLIC glad glfw OpenGL::GL)

target_compile_options(glt PRIVATE -Wall -Wextra -Wpedantic)

add_subdirectory(examples/triangle)
