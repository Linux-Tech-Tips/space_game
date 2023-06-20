#include "util.h"

/* Various potentially useful utility functions */

float util_toRad(float deg) {
    return (deg * (PI/180));
}

float util_toDeg(float rad) {
    return (rad * (180/PI));
}

int util_sign(float val) {
    if(val > 0)
        return 1;
    else if(val < 0)
        return -1;
    else
        return 0;
}

int util_iMax(int a, int b) {
    return (a > b ? a : b);
}
int util_iMin(int a, int b) {
    return (a < b ? a : b);
}

float util_fMax(float a, float b) {
    return (a > b ? a : b);
}
float util_fMin(float a, float b) {
    return (a < b ? a : b);
}