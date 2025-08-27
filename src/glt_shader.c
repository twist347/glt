#include "glt_shader.h"

#include <stdio.h>
#include <stdlib.h>

struct glt_shader_t {
    GLuint id;
};

static GLuint shader_compile(GLenum type, const char *src);

static GLboolean check_compile_errors(GLuint shader, const char *type);

static GLboolean check_link_errors(GLuint program);

static void ensure_bounds(const glt_shader_t *shader);

glt_shader_t *glt_shader_create(const char *vertex_shader_src, const char *fragment_shader_src) {
    if (!vertex_shader_src || !fragment_shader_src) {
        fprintf(stderr, "[ERROR::SHADER] null shader sources.\n");
        return NULL;
    }

    const GLuint vertex_shader = shader_compile(GL_VERTEX_SHADER, vertex_shader_src);
    if (!vertex_shader) {
        return NULL;
    }

    const GLuint fragment_shader = shader_compile(GL_FRAGMENT_SHADER, fragment_shader_src);
    if (!fragment_shader) {
        glDeleteShader(vertex_shader);
        return NULL;
    }

    glt_shader_t *shader = malloc(sizeof(glt_shader_t));
    if (!shader) {
        fprintf(stderr, "[ERROR::SHADER] failed to allocate shader_t.\n");
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return NULL;
    }

    shader->id = glCreateProgram();
    if (!shader->id) {
        fprintf(stderr, "[ERROR::SHADER] glCreateProgram failed.\n");
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
        fprintf(stderr, "[ERROR::SHADER] glCreateShader failed.\n");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    const char *type_name = "SHADER";
    switch (type) {
        case GL_VERTEX_SHADER:
            type_name = "VERTEX";
            break;
        case GL_FRAGMENT_SHADER:
            type_name = "FRAGMENT";
            break;
        case GL_GEOMETRY_SHADER:
            type_name = "GEOMETRY";
            break;
        default:
            break;
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
#ifdef GL_VERSION_4_1
    glProgramUniform1i(shader->id, loc, value);
#else
    ensure_bounds(shader);
    glUniform1i(loc, value);
#endif
}

void glt_shader_set_vec4(const glt_shader_t *shader, const char *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    if (!shader || !shader->id || !name) {
        return;
    }
    const GLint loc = glGetUniformLocation(shader->id, name);
    if (loc >= 0) {
        glt_shader_set_vec4_loс(shader, loc, x, y, z, w);
    }
}

void glt_shader_set_vec4_loс(const glt_shader_t *shader, GLint loc, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    if (!shader || !shader->id || loc < 0) {
        return;
    }
#ifdef GL_VERSION_4_1
    glProgramUniform4f(shader->id, loc, x, y, z, w);
#else
    ensure_bounds(shader);
    glUniform4f(loc, x, y, z, w);
#endif
}

static GLboolean check_compile_errors(GLuint shader, const char *type) {
    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        char *log = malloc(len);
        if (!log) {
            fprintf(stderr, "[ERROR::SHADER] %s compile failed (no mem for log)\n", type);
            return GL_FALSE;
        }
        glGetShaderInfoLog(shader, len, NULL, log);
        fprintf(stderr, "[ERROR::SHADER] %s compilation failed:\n%s\n", type, log);
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
            fprintf(stderr, "[ERROR::SHADER] link failed (no mem for log)\n");
            return GL_FALSE;
        }
        glGetProgramInfoLog(program, len, NULL, log);
        fprintf(stderr, "[ERROR::SHADER] program linking failed:\n%s\n", log);
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
