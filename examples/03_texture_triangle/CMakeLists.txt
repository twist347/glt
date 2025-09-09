add_executable(03_texture_triangle main.c)
target_link_libraries(03_texture_triangle glt)
add_custom_command(
        TARGET 03_texture_triangle
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/img1.png
        $<TARGET_FILE_DIR:03_texture_triangle>/img1.png
)
