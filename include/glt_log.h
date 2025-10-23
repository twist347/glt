#pragma once

typedef enum {
    GLT_LOG_INFO = 0,
    GLT_LOG_WARNING,
    GLT_LOG_ERROR,
    GLT_LOG_FATAL,
    GLT_LOG__COUNT
} glt_log_level_e;

void glt_log(glt_log_level_e level, const char *msg, ...);