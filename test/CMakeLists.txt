add_executable(test test.c)
target_link_libraries(test PRIVATE glfw OpenGL::GL glad::glad)

target_compile_options(test PRIVATE -Wall -Wextra -Wpedantic)