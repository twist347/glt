#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "glt.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SCREEN_TITLE "triangle example"

const char *vertex_src =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "}\n";

const char *fragment_src =
        "#version 330 core\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "    frag_color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

int main() {
    int exit_code = EXIT_SUCCESS;
    glt_window_t *window = NULL;
    glt_shader_t *shader = NULL;
    glt_vertex_buffer_t *vbo = NULL;
    glt_vertex_array_t *vao = NULL;

    window = glt_window_create(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, 4, 6);
    if (!window) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    if (!glt_init_glad()) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    glt_info_print();

    const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f, // right
        0.0f, 0.5f, 0.0f // top
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

    glt_vertex_array_attrib_pointerf(vao, vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);

    shader = glt_shader_create(vertex_src, fragment_src);
    if (!shader) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    glt_window_set_clear_color(1.f, 1.f, 1.f, 1.f);

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
