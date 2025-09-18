set(T 03_texture_triangle_example)

add_executable(${T} main.c)
target_link_libraries(${T} glt m)
add_custom_command(
        TARGET ${T}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/img1.png
        $<TARGET_FILE_DIR:${T}>/img1.png
)
