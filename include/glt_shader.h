#pragma once

#include "glad/glad.h"

typedef struct glt_shader_t glt_shader_t;

glt_shader_t *glt_shader_create(const char *vertex_shader_src, const char *fragment_shader_src);

void glt_shader_destroy(glt_shader_t *shader);

void glt_shader_use(const glt_shader_t *shader);

GLuint glt_shader_get_id(const glt_shader_t *shader);

void glt_shader_set_int(glt_shader_t *shader, const char *name, GLint value);

void glt_shader_set_int_loc(glt_shader_t *shader, GLint loc, GLint value);

void glt_shader_set_vec4(const glt_shader_t* shader, const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glt_shader_set_vec4_loс(const glt_shader_t* shader, GLint loc, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
