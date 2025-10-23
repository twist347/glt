#pragma once

#define GLT_PI 3.14159265358979323846f
#define GLT_DEG2RAD (GLT_PI / 180.f)
#define GLT_RAD2DEG (180.f / GLT_PI)

typedef struct {
    float x;
    float y;
} glt_vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} glt_vec3_t;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} glt_vec4_t;

typedef struct {
    float pos[3];
    float color[3];
    float tex_coord[2];
} glt_vert_t;

float glt_clamp(float val, float min_val, float max_val);

float glt_lepr(float a, float b, float t);