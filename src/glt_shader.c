#include "glt_shader.h"

#include <stdio.h>
#include <stdlib.h>

struct glt_shader_t {
    GLuint id;
};

static GLuint shader_compile(GLenum type, const char *src);

static GLboolean check_compile_errors(GLuint shader, const char *type);

static GLboolean check_link_errors(GLuint program);

static void ensure_bounds(const glt_shader_t *shader) __attribute__((unused));

glt_shader_t *glt_shader_create(const char *vertex_shader_src, const char *fragment_shader_src) {
    if (!vertex_shader_src || !fragment_shader_src) {
        fprintf(stderr, "[SHADER::ERROR] null shader sources.\n");
        return NULL;
    }

    const GLuint vertex_shader = shader_compile(GL_VERTEX_SHADER, vertex_shader_src);
    if (!vertex_shader) return NULL;

    const GLuint fragment_shader = shader_compile(GL_FRAGMENT_SHADER, fragment_shader_src);
    if (!fragment_shader) {
        glDeleteShader(vertex_shader);
        return NULL;
    }

    glt_shader_t *shader = (glt_shader_t *) malloc(sizeof(glt_shader_t));
    if (!shader) {
        fprintf(stderr, "[SHADER::ERROR] failed to allocate shader_t.\n");
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return NULL;
    }

    shader->id = glCreateProgram();
    if (!shader->id) {
        fprintf(stderr, "[SHADER::ERROR] glCreateProgram failed.\n");
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        free(shader);
        return NULL;
    }

    glAttachShader(shader->id, vertex_shader);
    glAttachShader(shader->id, fragment_shader);
    glLinkProgram(shader->id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    if (!check_link_errors(shader->id)) {
        glDeleteProgram(shader->id);
        free(shader);
        return NULL;
    }

    return shader;
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

static GLuint shader_compile(GLenum type, const char *src) {
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

void glt_shader_use(const glt_shader_t *shader) {
    if (shader && shader->id) {
        glUseProgram(shader->id);
    }
}

GLuint glt_shader_get_id(const glt_shader_t *shader) {
    return shader ? shader->id : 0;
}

/* ---- int / ivec ---- */

void glt_shader_set_int(glt_shader_t *shader, const char *name, GLint value) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) { glt_shader_set_int_loc(shader, loc, value); }
}

void glt_shader_set_int_loc(glt_shader_t *shader, GLint loc, GLint value) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform1i(shader->id, loc, value);
#else
    ensure_bounds(shader);
    glUniform1i(loc, value);
#endif
}

void glt_shader_set_ivec2(glt_shader_t *shader, const char *name, GLint x, GLint y) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) { glt_shader_set_ivec2_loc(shader, loc, x, y); }
}

void glt_shader_set_ivec2_loc(glt_shader_t *shader, GLint loc, GLint x, GLint y) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform2i(shader->id, loc, x, y);
#else
    ensure_bounds(shader);
    glUniform2i(loc, x, y);
#endif
}

void glt_shader_set_ivec3(glt_shader_t *shader, const char *name, GLint x, GLint y, GLint z) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) { glt_shader_set_ivec3_loc(shader, loc, x, y, z); }
}

void glt_shader_set_ivec3_loc(glt_shader_t *shader, GLint loc, GLint x, GLint y, GLint z) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform3i(shader->id, loc, x, y, z);
#else
    ensure_bounds(shader);
    glUniform3i(loc, x, y, z);
#endif
}

void glt_shader_set_ivec4(glt_shader_t *shader, const char *name, GLint x, GLint y, GLint z, GLint w) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) { glt_shader_set_ivec4_loc(shader, loc, x, y, z, w); }
}

void glt_shader_set_ivec4_loc(glt_shader_t *shader, GLint loc, GLint x, GLint y, GLint z, GLint w) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform4i(shader->id, loc, x, y, z, w);
#else
    ensure_bounds(shader);
    glUniform4i(loc, x, y, z, w);
#endif
}

/* ---- uint / uvec ---- */

void glt_shader_set_uint(const glt_shader_t *shader, const char *name, GLuint value) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
#ifdef GL_VERSION_4_1
        glProgramUniform1ui(shader->id, loc, value);
#else
        ensure_bounds(shader);
        glUniform1ui(loc, value);
#endif
    }
}

void glt_shader_set_uint_loc(const glt_shader_t *shader, GLint loc, GLuint value) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform1ui(shader->id, loc, value);
#else
    ensure_bounds(shader);
    glUniform1ui(loc, value);
#endif
}

void glt_shader_set_uvec2(const glt_shader_t *shader, const char *name, GLuint x, GLuint y) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
#ifdef GL_VERSION_4_1
        glProgramUniform2ui(shader->id, loc, x, y);
#else
        ensure_bounds(shader);
        glUniform2ui(loc, x, y);
#endif
    }
}

void glt_shader_set_uvec2_loc(const glt_shader_t *shader, GLint loc, GLuint x, GLuint y) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform2ui(shader->id, loc, x, y);
#else
    ensure_bounds(shader);
    glUniform2ui(loc, x, y);
#endif
}

void glt_shader_set_uvec3(const glt_shader_t *shader, const char *name, GLuint x, GLuint y, GLuint z) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
#ifdef GL_VERSION_4_1
        glProgramUniform3ui(shader->id, loc, x, y, z);
#else
        ensure_bounds(shader);
        glUniform3ui(loc, x, y, z);
#endif
    }
}

void glt_shader_set_uvec3_loc(const glt_shader_t *shader, GLint loc, GLuint x, GLuint y, GLuint z) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform3ui(shader->id, loc, x, y, z);
#else
    ensure_bounds(shader);
    glUniform3ui(loc, x, y, z);
#endif
}

void glt_shader_set_uvec4(const glt_shader_t *shader, const char *name, GLuint x, GLuint y, GLuint z, GLuint w) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
#ifdef GL_VERSION_4_1
        glProgramUniform4ui(shader->id, loc, x, y, z, w);
#else
        ensure_bounds(shader);
        glUniform4ui(loc, x, y, z, w);
#endif
    }
}

void glt_shader_set_uvec4_loc(const glt_shader_t *shader, GLint loc, GLuint x, GLuint y, GLuint z, GLuint w) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform4ui(shader->id, loc, x, y, z, w);
#else
    ensure_bounds(shader);
    glUniform4ui(loc, x, y, z, w);
#endif
}

/* ---- float / vec ---- */

void glt_shader_set_float(const glt_shader_t *shader, const char *name, GLfloat v) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
#ifdef GL_VERSION_4_1
        glProgramUniform1f(shader->id, loc, v);
#else
        ensure_bounds(shader);
        glUniform1f(loc, v);
#endif
    }
}

void glt_shader_set_float_loc(const glt_shader_t *shader, GLint loc, GLfloat v) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform1f(shader->id, loc, v);
#else
    ensure_bounds(shader);
    glUniform1f(loc, v);
#endif
}

void glt_shader_set_vec2(const glt_shader_t *shader, const char *name, GLfloat x, GLfloat y) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
#ifdef GL_VERSION_4_1
        glProgramUniform2f(shader->id, loc, x, y);
#else
        ensure_bounds(shader);
        glUniform2f(loc, x, y);
#endif
    }
}

void glt_shader_set_vec2_loc(const glt_shader_t *shader, GLint loc, GLfloat x, GLfloat y) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform2f(shader->id, loc, x, y);
#else
    ensure_bounds(shader);
    glUniform2f(loc, x, y);
#endif
}

void glt_shader_set_vec3(const glt_shader_t *shader, const char *name, GLfloat x, GLfloat y, GLfloat z) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
#ifdef GL_VERSION_4_1
        glProgramUniform3f(shader->id, loc, x, y, z);
#else
        ensure_bounds(shader);
        glUniform3f(loc, x, y, z);
#endif
    }
}

void glt_shader_set_vec3_loc(const glt_shader_t *shader, GLint loc, GLfloat x, GLfloat y, GLfloat z) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform3f(shader->id, loc, x, y, z);
#else
    ensure_bounds(shader);
    glUniform3f(loc, x, y, z);
#endif
}

void glt_shader_set_vec4(const glt_shader_t *shader, const char *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    if (!shader || !shader->id || !name) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) { glt_shader_set_vec4_loc(shader, loc, x, y, z, w); }
}

void glt_shader_set_vec4_loc(const glt_shader_t *shader, GLint loc, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    if (!shader || !shader->id || loc < 0) return;
#ifdef GL_VERSION_4_1
    glProgramUniform4f(shader->id, loc, x, y, z, w);
#else
    ensure_bounds(shader);
    glUniform4f(loc, x, y, z, w);
#endif
}

/* ---- matrices (float) ---- */

void glt_shader_set_mat2(const glt_shader_t *shader, const char *name, const GLfloat *m2x2) {
    if (!shader || !shader->id || !name || !m2x2) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
#ifdef GL_VERSION_4_1
        glProgramUniformMatrix2fv(shader->id, loc, 1, GL_FALSE, m2x2);
#else
        ensure_bounds(shader);
        glUniformMatrix2fv(loc, 1, GL_FALSE, m2x2);
#endif
    }
}

void glt_shader_set_mat2_loc(const glt_shader_t *shader, GLint loc, const GLfloat *m2x2) {
    if (!shader || !shader->id || loc < 0 || !m2x2) return;
#ifdef GL_VERSION_4_1
    glProgramUniformMatrix2fv(shader->id, loc, 1, GL_FALSE, m2x2);
#else
    ensure_bounds(shader);
    glUniformMatrix2fv(loc, 1, GL_FALSE, m2x2);
#endif
}

void glt_shader_set_mat3(const glt_shader_t *shader, const char *name, const GLfloat *m3x3) {
    if (!shader || !shader->id || !name || !m3x3) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
#ifdef GL_VERSION_4_1
        glProgramUniformMatrix3fv(shader->id, loc, 1, GL_FALSE, m3x3);
#else
        ensure_bounds(shader);
        glUniformMatrix3fv(loc, 1, GL_FALSE, m3x3);
#endif
    }
}

void glt_shader_set_mat3_loc(const glt_shader_t *shader, GLint loc, const GLfloat *m3x3) {
    if (!shader || !shader->id || loc < 0 || !m3x3) return;
#ifdef GL_VERSION_4_1
    glProgramUniformMatrix3fv(shader->id, loc, 1, GL_FALSE, m3x3);
#else
    ensure_bounds(shader);
    glUniformMatrix3fv(loc, 1, GL_FALSE, m3x3);
#endif
}

void glt_shader_set_mat4(const glt_shader_t *shader, const char *name, const GLfloat *m4x4) {
    if (!shader || !shader->id || !name || !m4x4) return;
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
#ifdef GL_VERSION_4_1
        glProgramUniformMatrix4fv(shader->id, loc, 1, GL_FALSE, m4x4);
#else
        ensure_bounds(shader);
        glUniformMatrix4fv(loc, 1, GL_FALSE, m4x4);
#endif
    }
}

void glt_shader_set_mat4_loc(const glt_shader_t *shader, GLint loc, const GLfloat *m4x4) {
    if (!shader || !shader->id || loc < 0 || !m4x4) return;
#ifdef GL_VERSION_4_1
    glProgramUniformMatrix4fv(shader->id, loc, 1, GL_FALSE, m4x4);
#else
    ensure_bounds(shader);
    glUniformMatrix4fv(loc, 1, GL_FALSE, m4x4);
#endif
}

/* ---- helpers ---- */

static GLboolean check_compile_errors(GLuint shader, const char *type) {
    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        char *log = (char *) malloc(len);
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
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    if ((GLuint) current != shader->id) {
        glUseProgram(shader->id);
    }
}
