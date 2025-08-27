#include "glt_info.h"

#include <string.h>
#include <stdio.h>

#include "glad/glad.h"

static int has_extension(const char *name) {
    if (!name) return 0;
    GLint count = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &count);
    for (GLint i = 0; i < count; ++i) {
        const char *ext = (const char *) glGetStringi(GL_EXTENSIONS, (GLuint) i);
        if (ext && strcmp(ext, name) == 0) return 1;
    }
    return 0;
}

void glt_info_print(void) {
    const char *vendor = (const char *) glGetString(GL_VENDOR);
    const char *renderer = (const char *) glGetString(GL_RENDERER);
    const char *version = (const char *) glGetString(GL_VERSION);
    const char *glsl = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLint major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    GLint profileMask = 0, flags = 0;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profileMask);
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    const char *profile =
            (profileMask & GL_CONTEXT_CORE_PROFILE_BIT)
                ? "Core"
                : (profileMask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
                      ? "Compatibility"
                      : "Unknown";

    const int isDebug = (flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0;
    const int isFwdComp = (flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT) != 0;


    printf("==== OpenGL Info ====\n");
    printf("Vendor      : %s\n", vendor ? vendor : "n/a");
    printf("Renderer    : %s\n", renderer ? renderer : "n/a");
    printf("Version     : %s (parsed %d.%d, %s profile%s%s)\n",
           version ? version : "n/a",
           major, minor, profile,
           isDebug ? ", debug" : "",
           isFwdComp ? ", forward-compatible" : "");
    printf("GLSL        : %s\n", glsl ? glsl : "n/a");

    printf("=====================\n");
}
