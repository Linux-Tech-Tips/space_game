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


Vector2 util_normalVector2(Vector2 vec, int dir) {
    return (Vector2){dir * vec.y, dir * -vec.x};
}

Vector2 util_dirVector(float rot) {
    return (Vector2){cos(util_toRad(rot)), sin(util_toRad(rot))};
}


short util_circleCollision(Vector2 c1Pos, float c1Rad, Vector2 c2Pos, float c2Rad) {
    return (Vector2Distance(c1Pos, c2Pos) < (c1Rad + c2Rad));
}