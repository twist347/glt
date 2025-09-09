#include "glt_glad_wrapper.h"

#include <stdio.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

int glt_init_glad(void) {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "[GLAD::ERROR]: failed to initialize GLAD\n");
        return 0;
    }
    return 1;
}
