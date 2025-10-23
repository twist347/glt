#pragma once

#include "glad/glad.h"

// TODO: unpack color macro

#define GLT_UNPACK_COLOR(color) ((color)[0]), ((color)[1]), ((color)[2])

#define GLT_RED   ((GLfloat[3]){1.f, 0.f, 0.f})
#define GLT_GREEN ((GLfloat[3]){0.f, 1.f, 0.f})
#define GLT_BLUE  ((GLfloat[3]){0.f, 0.f, 1.f})
#define GLT_BLACK ((GLfloat[3]){0.f, 0.f, 0.f})
#define GLT_WHITE ((GLfloat[3]){1.f, 1.f, 1.f})