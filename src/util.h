#ifndef SG_UTIL_H
#define SG_UTIL_H

#include <math.h>
#include <raymath.h>

/* Various potentially useful utility functions */

/** Converts float value in degrees to radians */
float util_toRad(float deg);

/** Converts float value in radians to degrees */
float util_toDeg(float rad);

/** 
 * Mathematical signum function
 * @returns 1 for positive numbers, -1 for negative numbers, 0 for 0
 */
int util_sign(float val);

/** Returns the greater integer */
int util_iMax(int a, int b);
/** Returns the smaller integer*/
int util_iMin(int a, int b);

/** Returns the greater float */
float util_fMax(float a, float b);
/** Returns the smaller float */
float util_fMin(float a, float b);

#endif /* SG_UTIL_H */