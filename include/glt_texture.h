#pragma once

#include "glad/glad.h"

typedef struct glt_texture_t glt_texture_t;

glt_texture_t *glt_texture_load(const char *path);

void glt_texture_destroy(glt_texture_t *texture);

GLuint glt_texture_get_id(const glt_texture_t *texture);

GLsizei glt_texture_get_width(const glt_texture_t *texture);

GLsizei glt_texture_get_height(const glt_texture_t *texture);
