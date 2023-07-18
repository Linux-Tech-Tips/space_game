/**
 * @file util.h
 * @author Linux-Tech-Tips
 * @brief Various utility functions
 *
 * Contains various utility functions used within the rest of the Space Demo code.
 * Most functions are related to mathematical concepts not implemented in the @ref math.h or @ref raymath.h modules.
 */

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


/** 
 * Provides a normal vector to the given Vector2, in a specified direction 
 * @param dir left normal vector if -1, right if +1
 */
Vector2 util_normalVector2(Vector2 vec, int dir);

/** Returns a normalized vector with the direction of the specified rotation */
Vector2 util_dirVector(float rot);


/** Returns whether 2 circles with the given parameters are colliding */
short util_circleCollision(Vector2 c1Pos, float c1Rad, Vector2 c2Pos, float c2Rad);


#endif /* SG_UTIL_H */