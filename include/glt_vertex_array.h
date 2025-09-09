#pragma once

#include "glt_vertex_buffer.h"

typedef struct glt_vertex_array_t glt_vertex_array_t;

glt_vertex_array_t *glt_vertex_array_create(void);

void glt_vertex_array_destroy(glt_vertex_array_t *array);

void glt_vertex_array_bind(const glt_vertex_array_t *array);

void glt_vertex_array_unbind(void);

GLuint glt_vertex_array_get_id(const glt_vertex_array_t *array);

void glt_vertex_array_enable_attrib(GLuint index);

void glt_vertex_array_disable_attrib(GLuint index);

void glt_vertex_array_set_divisor(GLuint index, GLuint divisor);

void glt_vertex_array_attrib_pointerf(
    const glt_vertex_array_t *vao,
    const glt_vertex_buffer_t *vbo,
    GLuint index, GLint size, GLenum type, GLboolean normalized,
    GLsizei stride, const void *pointer
);

void glt_vertex_array_attrib_pointeri(
    const glt_vertex_array_t *vao,
    const glt_vertex_buffer_t *vbo,
    GLuint index, GLint size, GLenum type,
    GLsizei stride, const void *pointer
);

void glt_vertex_array_attrib_pointerl(
    const glt_vertex_array_t *vao,
    const glt_vertex_buffer_t *vbo,
    GLuint index, GLint size, GLenum type,
    GLsizei stride, const void *pointer
);
