#include "glt_math.h"

float glt_clamp(float val, float min_val, float max_val) {
    if (val < min_val) {
        return min_val;
    }
    if (val > max_val) {
        return max_val;
    }
    return val;
}

float glt_lepr(float a, float b, float t) {
    return a + (b - a) * t;
}
