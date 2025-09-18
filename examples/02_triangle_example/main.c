#include <stdlib.h>
#include <stddef.h>

#include "glt.h"

#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 6

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SCREEN_TITLE "02_triangle_example"

#define BG_COLOR GLT_WHITE

const char *vertex_src =
        "#version 460 core\n"

        "layout (location = 0) in vec3 a_pos;\n"
        "layout (location = 1) in vec3 a_color;\n"

        "out vec3 color;\n"

        "void main() {\n"
        "    gl_Position = vec4(a_pos, 1.0);\n"
        "    color = a_color;\n"
        "}\n";

const char *fragment_src =
        "#version 460 core\n"

        "out vec4 frag_color;\n"

        "in vec3 color;\n"

        "void main() {\n"
        "    frag_color = vec4(color, 1.0);\n"
        "}\n";

typedef struct {
    GLfloat pos[3];
    GLfloat color[3];
} vertex_t;

int main() {
    int exit_code = EXIT_SUCCESS;
    glt_window_t *window = NULL;
    glt_shader_t *shader = NULL;
    glt_vertex_buffer_t *vbo = NULL;
    glt_vertex_array_t *vao = NULL;

    window = glt_window_create(
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SCREEN_TITLE,
        OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION
    );

    if (!window) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    glt_info_print();

    const vertex_t vertices[] = {
        {{-0.5f, -0.5f, 0.f}, {1.f, 0.f, 0.f}}, // left
        {{0.5f, -0.5f, 0.f}, {0.f, 1.f, 0.f}}, // right
        {{0.0f, 0.5f, 0.f}, {0.f, 0.f, 1.f}} // top
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
        0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
        (void *) offsetof(vertex_t, pos)
    );

    glt_vertex_array_attrib_pointerf(
        vao, vbo,
        1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
        (void *) offsetof(vertex_t, color)
    );

    shader = glt_shader_create(vertex_src, fragment_src);
    if (!shader) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    glt_window_set_clear_color(GLT_UNPACK_COLOR(BG_COLOR), 1.f);

    while (!glt_window_should_close(window)) {
        glt_window_process_input(window);

        glt_window_clear();

        glt_shader_use(shader);
        glt_vertex_array_bind(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glt_vertex_array_unbind();

        glt_window_swap_buffers(window);
        glt_window_poll_events();
    }

cleanup:
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
