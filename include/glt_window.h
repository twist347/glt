#pragma once

#include <stdbool.h>

#include "GLFW/glfw3.h"

typedef struct glt_window_t glt_window_t;

glt_window_t *glt_window_create(int width, int height, const char *title, int major_ver, int minor_ver);

void glt_window_destroy(glt_window_t *window);

GLFWwindow *glt_window_get_handle(const glt_window_t *window);

void glt_window_set_vsync(bool enabled);

bool glt_window_should_close(const glt_window_t *window);

void glt_window_process_input(const glt_window_t *window);

void glt_window_swap_buffers(const glt_window_t *window);

void glt_window_poll_events(void);

void glt_window_set_clear_color(float r, float g, float b, float a);

void glt_window_clear(void);

int glt_window_get_width(const glt_window_t *window);

int glt_window_get_height(const glt_window_t *window);
