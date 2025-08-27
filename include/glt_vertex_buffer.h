#pragma once

#include "glad/glad.h"

typedef struct glt_vertex_buffer_t glt_vertex_buffer_t;

glt_vertex_buffer_t *glt_vertex_buffer_create(const void *data, GLsizeiptr size, GLenum usage);

void glt_vertex_buffer_destroy(glt_vertex_buffer_t *buffer);

void glt_vertex_buffer_bind(const glt_vertex_buffer_t *buffer);

void glt_vertex_buffer_unbind(void);

void glt_vertex_buffer_set_data(glt_vertex_buffer_t *buffer, const void *data, GLsizeiptr size);

GLuint glt_vertex_buffer_get_id(const glt_vertex_buffer_t *buffer);

GLsizeiptr glt_vertex_buffer_get_size(const glt_vertex_buffer_t *buffer);
