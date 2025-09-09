#include "glt_window.h"

#include <stdio.h>
#include <stdlib.h>

struct glt_window_t {
    GLFWwindow *handle;
};

static void framebuffer_size_callback(GLFWwindow *handle, int width, int height) {
    (void) handle;
    glViewport(0, 0, width, height);
}

glt_window_t *glt_window_create(int width, int height, const char *title, int major_ver, int minor_ver) {
    if (!glfwInit()) {
        fprintf(stderr, "[WINDOW::ERROR] failed to initialize GLFW\n");
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_ver);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_ver);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *handle = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!handle) {
        fprintf(stderr, "[WINDOW::ERROR] failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(handle);

    glfwSwapInterval(1); // VSync

    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(handle, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);
    glfwSetFramebufferSizeCallback(handle, framebuffer_size_callback);

    glt_window_t *window = malloc(sizeof(glt_window_t));
    if (!window) {
        fprintf(stderr, "[WINDOW::ERROR] failed to allocate memory for window\n");
        glfwDestroyWindow(handle);
        glfwTerminate();
        return NULL;
    }

    window->handle = handle;

    return window;
}

void glt_window_destroy(glt_window_t *window) {
    if (!window) {
        return;
    }

    if (window->handle) {
        glfwDestroyWindow(window->handle);
        window->handle = NULL;
    }
    free(window);

    glfwTerminate();
}

GLFWwindow *glt_window_get_handle(const glt_window_t *window) {
    return window ? window->handle : NULL;
}

void glt_window_set_vsync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
}

bool glt_window_should_close(const glt_window_t *window) {
    if (!window || !window->handle) {
        return true;
    }
    return glfwWindowShouldClose(window->handle);
}

void glt_window_process_input(const glt_window_t *window) {
    if (!window || !window->handle) {
        return;
    }
    if (glfwGetKey(glt_window_get_handle(window), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(glt_window_get_handle(window), GLFW_TRUE);
    }
}

void glt_window_swap_buffers(const glt_window_t *window) {
    if (window && window->handle) {
        glfwSwapBuffers(window->handle);
    }
}

void glt_window_poll_events(void) {
    glfwPollEvents();
}

void glt_window_set_clear_color(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void glt_window_clear(void) {
    glClear(GL_COLOR_BUFFER_BIT);
}

int glt_window_get_width(const glt_window_t *window) {
    if (!window || !window->handle) {
        return 0;
    }

    int width, height;
    glfwGetWindowSize(window->handle, &width, &height);
    return width;
}

int glt_window_get_height(const glt_window_t *window) {
    if (!window || !window->handle) {
        return 0;
    }

    int width, height;
    glfwGetWindowSize(window->handle, &width, &height);
    return height;
}
