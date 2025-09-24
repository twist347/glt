#include "glt_shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct glt_shader_t {
    GLuint id;
};

static GLboolean g_has_prog_uniforms = GL_FALSE;
static bool globs_init = false;

// helper funcs decls

static void set_globs(void);
static GLboolean check_compile_errors(GLuint shader, const char *type);
static GLboolean check_link_errors(GLuint program);
static void ensure_bounds(const glt_shader_t *shader);
static char *read_from_text_file(const char *path, size_t *out_size);

// public API

GLuint glt_shader_compile_src(GLenum type, const char *src) {
    const GLuint shader = glCreateShader(type);
    if (!shader) {
        fprintf(stderr, "[SHADER::ERROR] glCreateShader failed.\n");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    const char *type_name = "SHADER";
    switch (type) {
        case GL_VERTEX_SHADER: type_name = "VERTEX";
            break;
        case GL_FRAGMENT_SHADER: type_name = "FRAGMENT";
            break;
#ifdef GL_GEOMETRY_SHADER
        case GL_GEOMETRY_SHADER: type_name = "GEOMETRY";
            break;
#endif
        default: break;
    }

    if (!check_compile_errors(shader, type_name)) {
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint glt_shader_compile_path(GLenum type, const char *path) {
    if (!path) {
        return 0;
    }

    size_t sz = 0;
    char *src = read_from_text_file(path, &sz);
    if (!src) {
        fprintf(stderr, "[SHADER::ERROR] read failed: %s\n", path);
        return 0;
    }

    const GLuint id = glt_shader_compile_src(type, src);
    if (!id) {
        fprintf(stderr, "[SHADER::ERROR] compile failed: %s\n", path);
    }
    free(src);
    return id;
}

glt_shader_t *glt_shader_prog_create(GLuint vertex_shader, GLuint fragment_shader) {
    if (vertex_shader == 0 || fragment_shader == 0) {
        fprintf(stderr, "[SHADER::ERROR] glt_shader_create failed.\n");
        return NULL;
    }
    set_globs();

    glt_shader_t *shader = malloc(sizeof(glt_shader_t));
    if (!shader) {
        fprintf(stderr, "[SHADER::ERROR] failed to allocate shader_t.\n");
        return NULL;
    }

    const GLuint program = glCreateProgram();
    if (program == 0) {
        fprintf(stderr, "[SHADER::ERROR] glCreateProgram failed.\n");
        free(shader);
        return NULL;
    }

    shader->id = program;

    glAttachShader(shader->id, vertex_shader);
    glAttachShader(shader->id, fragment_shader);
    glLinkProgram(shader->id);

    if (!check_link_errors(shader->id)) {
        glDeleteProgram(shader->id);
        free(shader);
        return NULL;
    }

    return shader;
}

glt_shader_t *glt_shader_prog_create_src(const char *vertex_shader_src, const char *fragment_shader_src) {
    if (!vertex_shader_src || !fragment_shader_src) {
        fprintf(stderr, "[SHADER::ERROR] null shader sources.\n");
        return NULL;
    }

    const GLuint vertex_shader = glt_shader_compile_src(GL_VERTEX_SHADER, vertex_shader_src);
    if (vertex_shader == 0) {
        return NULL;
    }

    const GLuint fragment_shader = glt_shader_compile_src(GL_FRAGMENT_SHADER, fragment_shader_src);
    if (fragment_shader == 0) {
        glDeleteShader(vertex_shader);
        return NULL;
    }

    glt_shader_t *shader = glt_shader_prog_create(vertex_shader, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader;
}

glt_shader_t *glt_shader_prog_create_path(const char *vertex_shader_path, const char *fragment_shader_path) {
    if (!vertex_shader_path || !fragment_shader_path) {
        fprintf(stderr, "[SHADER::ERROR] null file path(s).\n");
        return NULL;
    }

    const GLuint vertex_shader = glt_shader_compile_path(GL_VERTEX_SHADER, vertex_shader_path);
    if (vertex_shader == 0) {
        return NULL;
    }

    const GLuint fragment_shader = glt_shader_compile_path(GL_FRAGMENT_SHADER, fragment_shader_path);
    if (fragment_shader == 0) {
        glDeleteShader(vertex_shader);
        return NULL;
    }

    glt_shader_t *prog = glt_shader_prog_create(vertex_shader, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return prog;
}

void glt_shader_destroy(glt_shader_t *shader) {
    if (shader) {
        if (shader->id) {
            glDeleteProgram(shader->id);
            shader->id = 0;
        }
        free(shader);
    }
}

void glt_shader_use(const glt_shader_t *shader) {
    if (shader && shader->id) {
        glUseProgram(shader->id);
    }
}

GLuint glt_shader_get_id(const glt_shader_t *shader) {
    return shader ? shader->id : 0;
}

GLint glt_shader_get_uniform_loc(const glt_shader_t *shader, const char *name) {
    if (!shader || !shader->id || !name) {
        return -1;
    }
    return glGetUniformLocation(shader->id, name);
}

// int / ivec

void glt_shader_set_int(glt_shader_t *shader, const char *name, GLint value) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_int_loc(shader, loc, value);
    }
}

void glt_shader_set_int_loc(glt_shader_t *shader, GLint loc, GLint value) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform1i(shader->id, loc, value);
    } else {
        ensure_bounds(shader);
        glUniform1i(loc, value);
    }
}

void glt_shader_set_ivec2(glt_shader_t *shader, const char *name, GLint x, GLint y) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_ivec2_loc(shader, loc, x, y);
    }
}

void glt_shader_set_ivec2_loc(glt_shader_t *shader, GLint loc, GLint x, GLint y) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform2i(shader->id, loc, x, y);
    } else {
        ensure_bounds(shader);
        glUniform2i(loc, x, y);
    }
}

void glt_shader_set_ivec3(glt_shader_t *shader, const char *name, GLint x, GLint y, GLint z) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_ivec3_loc(shader, loc, x, y, z);
    }
}

void glt_shader_set_ivec3_loc(glt_shader_t *shader, GLint loc, GLint x, GLint y, GLint z) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform3i(shader->id, loc, x, y, z);
    } else {
        ensure_bounds(shader);
        glUniform3i(loc, x, y, z);
    }
}

void glt_shader_set_ivec4(glt_shader_t *shader, const char *name, GLint x, GLint y, GLint z, GLint w) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_ivec4_loc(shader, loc, x, y, z, w);
    }
}

void glt_shader_set_ivec4_loc(glt_shader_t *shader, GLint loc, GLint x, GLint y, GLint z, GLint w) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform4i(shader->id, loc, x, y, z, w);
    } else {
        ensure_bounds(shader);
        glUniform4i(loc, x, y, z, w);
    }
}

// uint / uvec

void glt_shader_set_uint(const glt_shader_t *shader, const char *name, GLuint value) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_uint_loc(shader, loc, value);
    }
}

void glt_shader_set_uint_loc(const glt_shader_t *shader, GLint loc, GLuint value) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform1ui(shader->id, loc, value);
    } else {
        ensure_bounds(shader);
        glUniform1ui(loc, value);
    }
}

void glt_shader_set_uvec2(const glt_shader_t *shader, const char *name, GLuint x, GLuint y) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_uvec2_loc(shader, loc, x, y);
    }
}

void glt_shader_set_uvec2_loc(const glt_shader_t *shader, GLint loc, GLuint x, GLuint y) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform2ui(shader->id, loc, x, y);
    } else {
        ensure_bounds(shader);
        glUniform2ui(loc, x, y);
    }
}

void glt_shader_set_uvec3(const glt_shader_t *shader, const char *name, GLuint x, GLuint y, GLuint z) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_uvec3_loc(shader, loc, x, y, z);
    }
}

void glt_shader_set_uvec3_loc(const glt_shader_t *shader, GLint loc, GLuint x, GLuint y, GLuint z) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform3ui(shader->id, loc, x, y, z);
    } else {
        ensure_bounds(shader);
        glUniform3ui(loc, x, y, z);
    }
}

void glt_shader_set_uvec4(const glt_shader_t *shader, const char *name, GLuint x, GLuint y, GLuint z, GLuint w) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_uvec4_loc(shader, loc, x, y, z, w);
    }
}

void glt_shader_set_uvec4_loc(const glt_shader_t *shader, GLint loc, GLuint x, GLuint y, GLuint z, GLuint w) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform4ui(shader->id, loc, x, y, z, w);
    } else {
        ensure_bounds(shader);
        glUniform4ui(loc, x, y, z, w);
    }
}

// float / vec

void glt_shader_set_float(const glt_shader_t *shader, const char *name, GLfloat v) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_float_loc(shader, loc, v);
    }
}

void glt_shader_set_float_loc(const glt_shader_t *shader, GLint loc, GLfloat v) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform1f(shader->id, loc, v);
    } else {
        ensure_bounds(shader);
        glUniform1f(loc, v);
    }
}

void glt_shader_set_vec2(const glt_shader_t *shader, const char *name, GLfloat x, GLfloat y) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_vec2_loc(shader, loc, x, y);
    }
}

void glt_shader_set_vec2_loc(const glt_shader_t *shader, GLint loc, GLfloat x, GLfloat y) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform2f(shader->id, loc, x, y);
    } else {
        ensure_bounds(shader);
        glUniform2f(loc, x, y);
    }
}

void glt_shader_set_vec3(const glt_shader_t *shader, const char *name, GLfloat x, GLfloat y, GLfloat z) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_vec3_loc(shader, loc, x, y, z);
    }
}

void glt_shader_set_vec3_loc(const glt_shader_t *shader, GLint loc, GLfloat x, GLfloat y, GLfloat z) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform3f(shader->id, loc, x, y, z);
    } else {
        ensure_bounds(shader);
        glUniform3f(loc, x, y, z);
    }
}

void glt_shader_set_vec4(const glt_shader_t *shader, const char *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_vec4_loc(shader, loc, x, y, z, w);
    }
}

void glt_shader_set_vec4_loc(const glt_shader_t *shader, GLint loc, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniform4f(shader->id, loc, x, y, z, w);
    } else {
        ensure_bounds(shader);
        glUniform4f(loc, x, y, z, w);
    }
}

// matrices (float)

void glt_shader_set_mat2(const glt_shader_t *shader, const char *name, const GLfloat *m2x2) {
    if (!shader || !shader->id || !name || !m2x2) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_mat2_loc(shader, loc, m2x2);
    }
}

void glt_shader_set_mat2_loc(const glt_shader_t *shader, GLint loc, const GLfloat *m2x2) {
    if (!shader || !shader->id || loc < 0 || !m2x2) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniformMatrix2fv(shader->id, loc, 1, GL_FALSE, m2x2);
    } else {
        ensure_bounds(shader);
        glUniformMatrix2fv(loc, 1, GL_FALSE, m2x2);
    }
}

void glt_shader_set_mat3(const glt_shader_t *shader, const char *name, const GLfloat *m3x3) {
    if (!shader || !shader->id || !name || !m3x3) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_mat3_loc(shader, loc, m3x3);
    }
}

void glt_shader_set_mat3_loc(const glt_shader_t *shader, GLint loc, const GLfloat *m3x3) {
    if (!shader || !shader->id || loc < 0 || !m3x3) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniformMatrix3fv(shader->id, loc, 1, GL_FALSE, m3x3);
    } else {
        ensure_bounds(shader);
        glUniformMatrix3fv(loc, 1, GL_FALSE, m3x3);
    }
}

void glt_shader_set_mat4(const glt_shader_t *shader, const char *name, const GLfloat *m4x4) {
    if (!shader || !shader->id || !name || !m4x4) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_mat4_loc(shader, loc, m4x4);
    }
}

void glt_shader_set_mat4_loc(const glt_shader_t *shader, GLint loc, const GLfloat *m4x4) {
    if (!shader || !shader->id || loc < 0 || !m4x4) {
        return;
    }
    if (g_has_prog_uniforms) {
        glProgramUniformMatrix4fv(shader->id, loc, 1, GL_FALSE, m4x4);
    } else {
        ensure_bounds(shader);
        glUniformMatrix4fv(loc, 1, GL_FALSE, m4x4);
    }
}

// helpers

static void set_globs(void) {
    if (globs_init) {
        return;
    }
    g_has_prog_uniforms = glProgramUniform1i != NULL;
    globs_init = true;
}

static GLboolean check_compile_errors(GLuint shader, const char *type) {
    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len < 1) {
            len = 1;
        }
        char *log = malloc(len);
        if (!log) {
            fprintf(stderr, "[SHADER::ERROR] %s compile failed (no mem for log)\n", type);
            return GL_FALSE;
        }
        glGetShaderInfoLog(shader, len, NULL, log);
        fprintf(stderr, "[SHADER::ERROR] %s compilation failed:\n%s\n", type, log);
        free(log);
        return GL_FALSE;
    }

    return GL_TRUE;
}

static GLboolean check_link_errors(GLuint program) {
    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        if (len < 1) {
            len = 1;
        }
        char *log = malloc(len);
        if (!log) {
            fprintf(stderr, "[SHADER::ERROR] link failed (no mem for log)\n");
            return GL_FALSE;
        }
        glGetProgramInfoLog(program, len, NULL, log);
        fprintf(stderr, "[SHADER::ERROR] program linking failed:\n%s\n", log);
        free(log);
        return GL_FALSE;
    }
    return GL_TRUE;
}

static void ensure_bounds(const glt_shader_t *shader) {
    GLint shader_id_cur = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shader_id_cur);
    if ((GLuint) shader_id_cur != shader->id) {
        glUseProgram(shader->id);
    }
}

static char *read_from_text_file(const char *path, size_t *out_size) {
    if (out_size) {
        *out_size = 0;
    }
    if (!path) {
        return NULL;
    }

    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "[SHADER::ERROR] can't open file: %s\n", path);
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    const long len = ftell(f);
    if (len < 0) {
        fclose(f);
        return NULL;
    }
    if (fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return NULL;
    }

    const size_t n = len;
    char *buf = malloc(n + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    const size_t rd = fread(buf, 1, n, f);
    fclose(f);
    if (rd != n) {
        free(buf);
        return NULL;
    }

    buf[n] = '\0';
    if (out_size) {
        *out_size = n;
    }
    return buf;
}
