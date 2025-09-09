#pragma once

#include "glad/glad.h"

typedef struct glt_shader_t glt_shader_t;

glt_shader_t *glt_shader_create(const char *vertex_shader_src, const char *fragment_shader_src);

void glt_shader_destroy(glt_shader_t *shader);

void glt_shader_use(const glt_shader_t *shader);

GLuint glt_shader_get_id(const glt_shader_t *shader);

/* ---- int / ivec ---- */

void glt_shader_set_int(glt_shader_t *shader, const char *name, GLint value);
void glt_shader_set_int_loc(glt_shader_t *shader, GLint loc, GLint value);

void glt_shader_set_ivec2(glt_shader_t *shader, const char *name, GLint x, GLint y);
void glt_shader_set_ivec2_loc(glt_shader_t *shader, GLint loc, GLint x, GLint y);

void glt_shader_set_ivec3(glt_shader_t *shader, const char *name, GLint x, GLint y, GLint z);
void glt_shader_set_ivec3_loc(glt_shader_t *shader, GLint loc, GLint x, GLint y, GLint z);

void glt_shader_set_ivec4(glt_shader_t *shader, const char *name, GLint x, GLint y, GLint z, GLint w);
void glt_shader_set_ivec4_loc(glt_shader_t *shader, GLint loc, GLint x, GLint y, GLint z, GLint w);

/* ---- uint / uvec ---- */
void glt_shader_set_uint(const glt_shader_t *shader, const char *name, GLuint value);
void glt_shader_set_uint_loc(const glt_shader_t *shader, GLint loc, GLuint value);

void glt_shader_set_uvec2(const glt_shader_t *shader, const char *name, GLuint x, GLuint y);
void glt_shader_set_uvec2_loc(const glt_shader_t *shader, GLint loc, GLuint x, GLuint y);

void glt_shader_set_uvec3(const glt_shader_t *shader, const char *name, GLuint x, GLuint y, GLuint z);
void glt_shader_set_uvec3_loc(const glt_shader_t *shader, GLint loc, GLuint x, GLuint y, GLuint z);

void glt_shader_set_uvec4(const glt_shader_t *shader, const char *name, GLuint x, GLuint y, GLuint z, GLuint w);
void glt_shader_set_uvec4_loc(const glt_shader_t *shader, GLint loc, GLuint x, GLuint y, GLuint z, GLuint w);

/* ---- float / vec ---- */

void glt_shader_set_float(const glt_shader_t *shader, const char *name, GLfloat v);
void glt_shader_set_float_loc(const glt_shader_t *shader, GLint loc, GLfloat v);

void glt_shader_set_vec2(const glt_shader_t *shader, const char *name, GLfloat x, GLfloat y);
void glt_shader_set_vec2_loc(const glt_shader_t *shader, GLint loc, GLfloat x, GLfloat y);

void glt_shader_set_vec3(const glt_shader_t *shader, const char *name, GLfloat x, GLfloat y, GLfloat z);
void glt_shader_set_vec3_loc(const glt_shader_t *shader, GLint loc, GLfloat x, GLfloat y, GLfloat z);

void glt_shader_set_vec4(const glt_shader_t* shader, const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glt_shader_set_vec4_loc(const glt_shader_t* shader, GLint loc, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

/* ---- matrices (float) ---- */

void glt_shader_set_mat2(const glt_shader_t *shader, const char *name, const GLfloat *m2x2);
void glt_shader_set_mat2_loc(const glt_shader_t *shader, GLint loc, const GLfloat *m2x2);

void glt_shader_set_mat3(const glt_shader_t *shader, const char *name, const GLfloat *m3x3);
void glt_shader_set_mat3_loc(const glt_shader_t *shader, GLint loc, const GLfloat *m3x3);

void glt_shader_set_mat4(const glt_shader_t *shader, const char *name, const GLfloat *m4x4);
void glt_shader_set_mat4_loc(const glt_shader_t *shader, GLint loc, const GLfloat *m4x4);
