#include "glt_vertex_buffer.h"

#include <stdio.h>
#include <stdlib.h>

struct glt_vertex_buffer_t {
    GLuint id;
    GLsizeiptr size;
    GLenum usage;
};

static int check_created_size(GLenum target, GLsizeiptr expected);

glt_vertex_buffer_t *glt_vertex_buffer_create(const void *data, GLsizeiptr size, GLenum usage) {
    if (size <= 0) {
        fprintf(stderr, "[VERTEX_BUFFER::ERROR] invalid buffer size: %ld\n", size);
        return NULL;
    }

    glt_vertex_buffer_t *buffer = malloc(sizeof(glt_vertex_buffer_t));
    if (!buffer) {
        fprintf(stderr, "[VERTEX_BUFFER::ERROR] failed to allocate memory\n");
        return NULL;
    }

    buffer->id = 0;
    buffer->size = 0;
    buffer->usage = usage;

    glGenBuffers(1, &buffer->id);
    if (!buffer->id) {
        fprintf(stderr, "[VERTEX_BUFFER::ERROR] failed to glGenBuffers\n");
        free(buffer);
        return NULL;
    }
    glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    const int ok = check_created_size(GL_ARRAY_BUFFER, size);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (!ok) {
        fprintf(stderr, "[VERTEX_BUFFER::ERROR] glBufferData failed to allocate %ld bytes\n", size);
        glDeleteBuffers(1, &buffer->id);
        free(buffer);
        return NULL;
    }

    buffer->size = size;

    return buffer;
}

void glt_vertex_buffer_destroy(glt_vertex_buffer_t *buffer) {
    if (buffer) {
        if (buffer->id) {
            glDeleteBuffers(1, &buffer->id);
        }
        free(buffer);
    }
}

void glt_vertex_buffer_bind(const glt_vertex_buffer_t *buffer) {
    if (buffer && buffer->id) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
    }
}

void glt_vertex_buffer_unbind(void) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void glt_vertex_buffer_set_data(glt_vertex_buffer_t *buffer, const void *data, GLsizeiptr size) {
    if (buffer && buffer->id && data && size > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
        glBufferData(GL_ARRAY_BUFFER, size, data, buffer->usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        buffer->size = size;
    }
}

GLuint glt_vertex_buffer_get_id(const glt_vertex_buffer_t *buffer) {
    return buffer ? buffer->id : 0;
}

GLsizeiptr glt_vertex_buffer_get_size(const glt_vertex_buffer_t *buffer) {
    return buffer ? buffer->size : 0;
}

static int check_created_size(GLenum target, GLsizeiptr expected) {
    GLint actual = 0;
    glGetBufferParameteriv(target, GL_BUFFER_SIZE, &actual);
    return actual == expected;
}
