#ifndef GAME_H
#define GAME_H

#include <stdlib.h>

#include <raylib.h>
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

    Texture2D ship;
    Texture2D background;

} game_data_t;

/* Update functions */

void game_update(game_data_t * gameData);

/* Render functions */

void game_render(game_data_t * gameData, int scrX, int scrY);

/* Load/Unload functions */

void game_loadTex(game_data_t * gameData);

void game_unloadTex(game_data_t * gameData);

/* Utility functions */

float game_toRad(float deg);

int game_sign(float val);

#endif