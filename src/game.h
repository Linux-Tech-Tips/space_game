#ifndef GAME_H
#define GAME_H

#include <stdlib.h>

#include <raylib.h>
#include <raymath.h>
#include <math.h>

/* Temporarily containing all the relevant game data for the demo version
    TBD Should be moved and split up into more relevant parts
 */
typedef struct {
    short paused;

    float posX, posY;
    float rot;
    Vector2 velocity;
    float angMoment;
    float accel;
    float angAccel;
    float rcsAccel;
    int throttle;

    short dampenAng;

    Texture2D ship;

    /* Backgrounds + scroll */
    Texture2D background [3];
    float bgOffsetX [3], bgOffsetY [3];
    float bgScrollMul [3];

    Camera2D camera;

} game_data_t;

/* Update functions */

void game_update(game_data_t * gameData);

/* Render functions */

void game_render(game_data_t * gameData, int scrX, int scrY);

/* Load/Unload functions */

void game_initStructure(game_data_t * gameData);

void game_loadTex(game_data_t * gameData);

void game_unloadTex(game_data_t * gameData);

/* Utility functions */

float game_toRad(float deg);

int game_sign(float val);

#endif