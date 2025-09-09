#include "glt_vertex_array.h"

#include <stdio.h>
#include <stdlib.h>

struct glt_vertex_array_t {
    GLuint id;
};

static void bind_vao_and_vbo(const glt_vertex_array_t *vao, const glt_vertex_buffer_t *vbo);

glt_vertex_array_t *glt_vertex_array_create(void) {
    glt_vertex_array_t *array = malloc(sizeof(glt_vertex_array_t));
    if (!array) {
        fprintf(stderr, "[VERTEX_ARRAY::ERROR] failed to allocate memory\n");
        return NULL;
    }
    array->id = 0;

    glGenVertexArrays(1, &array->id);
    if (!array->id) {
        fprintf(stderr, "[VERTEX_ARRAY::ERROR] failed to create vertex array\n");
        free(array);
        return NULL;
    }
    return array;
}

void glt_vertex_array_destroy(glt_vertex_array_t *array) {
    if (array) {
        if (array->id) {
            glDeleteVertexArrays(1, &array->id);
            array->id = 0;
        }
        free(array);
    }
}

void glt_vertex_array_bind(const glt_vertex_array_t *array) {
    if (array && array->id) {
        glBindVertexArray(array->id);
    }
}

void glt_vertex_array_unbind(void) {
    glBindVertexArray(0);
}

GLuint glt_vertex_array_get_id(const glt_vertex_array_t *array) {
    return array ? array->id : 0;
}

void glt_vertex_array_enable_attrib(GLuint index) {
    glEnableVertexAttribArray(index);
}

void glt_vertex_array_disable_attrib(GLuint index) {
    glDisableVertexAttribArray(index);
}

void glt_vertex_array_set_divisor(GLuint index, GLuint divisor) {
    glVertexAttribDivisor(index, divisor);
}

void glt_vertex_array_attrib_pointerf(
    const glt_vertex_array_t *vao,
    const glt_vertex_buffer_t *vbo,
    GLuint index, GLint size, GLenum type, GLboolean normalized,
    GLsizei stride, const void *pointer
) {
    if (!vao || !vbo) {
        fprintf(stderr, "[VERTEX_ARRAY::ERROR] attrib_pointerf: null vao or vbo\n");
        return;
    }
    bind_vao_and_vbo(vao, vbo);
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
}

void glt_vertex_array_attrib_pointeri(
    const glt_vertex_array_t *vao,
    const glt_vertex_buffer_t *vbo,
    GLuint index, GLint size, GLenum type,
    GLsizei stride, const void *pointer
) {
    if (!vao || !vbo) {
        fprintf(stderr, "[VERTEX_ARRAY::ERROR] attrib_pointeri: null vao or vbo\n");
        return;
    }
    bind_vao_and_vbo(vao, vbo);
    glVertexAttribIPointer(index, size, type, stride, pointer);
    glEnableVertexAttribArray(index);
}

void glt_vertex_array_attrib_pointerl(
    const glt_vertex_array_t *vao,
    const glt_vertex_buffer_t *vbo,
    GLuint index, GLint size, GLenum type,
    GLsizei stride, const void *pointer
) {
    if (!vao || !vbo) {
        fprintf(stderr, "[VERTEX_ARRAY::ERROR] attrib_pointerl: null vao or vbo\n");
        return;
    }
    bind_vao_and_vbo(vao, vbo);
    glVertexAttribLPointer(index, size, type, stride, pointer);
    glEnableVertexAttribArray(index);
}

static void bind_vao_and_vbo(const glt_vertex_array_t *vao, const glt_vertex_buffer_t *vbo) {
    glBindVertexArray(vao ? vao->id : 0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo ? glt_vertex_buffer_get_id(vbo) : 0);
}
