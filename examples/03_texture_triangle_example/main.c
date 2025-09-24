#include <stdlib.h>
#include <stddef.h>

#include "glt.h"

#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 6

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SCREEN_TITLE "03_texture_triangle example"

#define BG_COLOR GLT_WHITE

#define TEXTURE_PATH "textures/img2.png"

#define VERTEX_SHADER_PATH "shaders/shader.vert"
#define FRAGMENT_SHADER_PATH "shaders/shader.frag"

typedef struct {
    GLfloat pos[3];
    GLfloat color[3];
    GLfloat tex_coord[2];
} vertex_t;

int main() {
    int exit_code = EXIT_SUCCESS;
    glt_window_t *window = NULL;
    glt_shader_t *shader = NULL;
    glt_vertex_buffer_t *vbo = NULL;
    glt_vertex_array_t *vao = NULL;
    glt_texture_t *texture = NULL;

    window = glt_window_create(
        SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE,
        OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION
    );

    if (!window) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    glt_info_print();

    const vertex_t vertices[] = {
        {.pos = {-0.5f, -0.5f, 0.f}, .color = {1.f, 0.f, 0.f}, .tex_coord = {0.f, 0.f}},
        {.pos = {0.5f, -0.5f, 0.f}, .color = {0.f, 1.f, 0.f}, .tex_coord = {1.f, 0.f}},
        {.pos = {0.0f, 0.5f, 0.f}, .color = {0.f, 0.f, 1.f}, .tex_coord = {0.5f, 1.f}},
    };

    vbo = glt_vertex_buffer_create(vertices, sizeof(vertices), GL_STATIC_DRAW);
    if (!vbo) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    vao = glt_vertex_array_create();
    if (!vao) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    glt_vertex_array_attrib_pointerf(
        vao, vbo,
        0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void *) offsetof(vertex_t, pos)
    );
    glt_vertex_array_attrib_pointerf(
        vao, vbo,
        1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void *) offsetof(vertex_t, color)
    );
    glt_vertex_array_attrib_pointerf(
        vao, vbo,
        2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void *) offsetof(vertex_t, tex_coord)
    );

    shader = glt_shader_prog_create_path(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    if (!shader) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    glt_shader_set_int(shader, "tex_sampler", 0);

    texture = glt_texture_load(TEXTURE_PATH);
    if (!texture) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    glt_window_set_clear_color(GLT_UNPACK_COLOR(BG_COLOR), 1.f);

    while (!glt_window_should_close(window)) {
        glt_window_process_input(window);
        glt_window_clear();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, glt_texture_get_id(texture));

        glt_shader_use(shader);
        glt_vertex_array_bind(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glt_vertex_array_unbind();

        glt_window_swap_buffers(window);
        glt_window_poll_events();
    }

cleanup:
    if (texture) {
        glt_texture_destroy(texture);
    }

    if (vao) {
        glt_vertex_array_destroy(vao);
    }

    if (vbo) {
        glt_vertex_buffer_destroy(vbo);
    }

    if (shader) {
        glt_shader_destroy(shader);
    }

    if (window) {
        glt_window_destroy(window);
    }

    return exit_code;
}
