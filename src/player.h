#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>

#include <raylib.h>
#include <raymath.h>
#include <math.h>

#include "util.h"

/** The value at which any velocity should automatically be clipped to 0 */
#define VELOCITY_CUTOFF_VAL 0.5f

/** Player data structure containing all player-relevant information */
typedef struct {

    /** Player position */
    Vector2 pos;
    /** Player orientation */
    float rot;

    /** Current velocity of the player */
    Vector2 velocity;
    /** Current angular velocity of the player */
    float angVelocity;
    /** Acceleration value of the current frame, reset at the start of each frame */
    Vector2 accel;

    /** The acceleration size of the main thrusters of the player */
    float accelVal;
    /** The angular acceleration size of the ship */
    float angAccelVal;
    /** The acceleration size of the player's RCS thrusters (sideways and backwards) */
    float rcsAccelVal;

    /** The amount by which the main engines tilt to turn the ship */
    float gimbal;
    /** The current throttle of the main thrusters, percentage value of 0-100 */
    int throttle;

    /** If true, the spacial velocity is automatically dampened (using RCS thrusters) */
    short dampen;
    /** If true, the angular velocity is automatically dampened */
    short angDampen;

    /** If true, the game should create a bullet */
    short shoot;
    /** The delay between shots */
    float shootDelay;
    /** The timer deciding whether shooting available or not */
    float shootCounter;
    /** The damage player bullets do */
    float bulletDamage;

    /** The current health of the player */
    float health;
    /** The maximum health of the player */
    float maxHealth;

    /** The size of the circles hitbox of the player */
    float hitboxSize;

    /** The pointer to the ship's current texture, is to be loaded elsewhere */
    Texture2D * ship;
    /** The pointer to the ship's thruster exhaust texture, is to be loaded elsewhere */
    Texture2D * shipExhaust;

} player_t;


/* Update functions */

/** Contains the base player update logic */
void player_update(player_t * playerData);


/* Render functions */

/** 
 * Contains the base player draw calls 
 * @param noDynamicRender if false, doesn't render thrusters based on player input
 */
void player_render(player_t playerData, short noDynamicRender);


/* Other functions */

/** Initializes an empty player_t data structure */
void player_initData(player_t * playerData, Texture2D * playerTex, Texture2D * playerExhaustTex);

/** Resets a player structure to new game data */
void player_resetData(player_t * playerData);

#endif /* PLAYER_H */