#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include <raymath.h>

#include "util.h"

/** The value at which any velocity should automatically be clipped to 0 */
#define VELOCITY_CUTOFF_VAL 0.5f

/** Defines various kinds of enemies, based on which stats are assigned */
typedef enum {
    /** Doesn't do anything */
    dummy,
    /** Basic enemy, follows the player and shoots */
    basic
} enemy_type_t;

/** The base enemy data type */
typedef struct {

    /** Enemy position */
    Vector2 pos;
    /** Enemy orientation */
    float rot;

    /** Enemy velocity */
    Vector2 velocity;
    /** Enemy angular velocity */
    float angVelocity;
    /** Current enemy acceleration between frames */
    Vector2 accel;

    /** Size of the acceleration of the enemy's main thrusters */
    float accelVal;
    /** Size of the angular acceleration of the enemy ship */
    float angAccelVal;
    /** Size of the RCS acceleration of the enemy ship */
    float rcsAccelVal;

    /** How much the enemy's main thrusters move when rotating the ship */
    float gimbal;

    /** If true, it signals to create a bullet */
    short shoot;
    /** The delay between shots */
    float shootDelay;
    /** The timer for the shooting delay */
    float shootCounter;

    /** State variable, if true, the enemy's main thrusters are on */
    short thrustersOn;
    /** State variable, defines the enemy ship's desired rotation (-1 / 0 / +1) */
    int rotating;

    /** If true, the enemy won't despawn */
    short persistent;

    /** The pointer to the enemy ship texture, to be loaded elsewhere */
    Texture2D * ship;
    /** The pointer to the enemy ship thruster exhaust texture, to be loaded elsewhere */
    Texture2D * shipExhaust;

    /** Defines which type of ship the enemy is (influences stats and AI) */
    enemy_type_t shipType;

} enemy_t;


/* Update and render functions */

/** Contains the base enemy update logic, updates the enemy's movement and AI */
void enemy_update(enemy_t * enemy);

/** Contains the base enemy draw calls */
void enemy_render(enemy_t enemy);


/* AI functions */

/** The AI of the basic enemy type */
void enemy_basicAI(enemy_t * enemy);


/* Enemy list functions */

/** Removes an enemy from a list at the given index, shrinks the passed list size variable */
void enemy_list_remove(enemy_t eList [], int index, int * lSize);

/** Adds an enemy to the end of a list, increases the passed list size, removes the first entry if the list is too big */
void enemy_list_add(enemy_t eList [], enemy_t enemy, int * lSize, int maxSize);


/* Other functions */

/** 
 * Initializes the data of an empty enemy structure to the desired values. 
 * @param shipExhaustTex if the enemy type is dummy and uses no exhaust tex, can be set to 0
 */
void enemy_initData(enemy_t * enemy, Vector2 pos, float rot, Vector2 velocity, short persistent, enemy_type_t shipType, Texture2D * shipTex, Texture2D * shipExhaustTex);


#endif /* ENEMY_H */