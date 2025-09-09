#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "glt.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SCREEN_TITLE "01_window_example"

#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 6

int main() {
    int exit_code = EXIT_SUCCESS;
    glt_window_t *window = NULL;

    window = glt_window_create(
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SCREEN_TITLE,
        OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION
    );
    if (!window) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    if (!glt_init_glad()) {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }

    glt_info_print();

    glt_window_set_clear_color(1.f, 1.f, 1.f, 1.f);

    while (!glt_window_should_close(window)) {
        glt_window_process_input(window);

        glt_window_clear();

        glt_window_swap_buffers(window);
        glt_window_poll_events();
    }

cleanup:
    if (window) {
        glt_window_destroy(window);
    }

    return exit_code;
}
