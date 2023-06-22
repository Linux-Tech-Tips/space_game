#ifndef GAME_H
#define GAME_H

#include <stdlib.h>

#include <raylib.h>
#include <raymath.h>
#include <math.h>

#include "util.h"
#include "player.h"

/** The game's main data structure, includes all data relevant to gameplay */
typedef struct {
    /** The pause state of the game */
    short paused;

    /** The data of the current player */
    player_t playerData;

    /** Array of 3 scrolling background textures for an enhanced depth effect */
    Texture2D background [3];
    /** The x/y offset of the background texture in relation to the camera */
    float bgOffsetX [3], bgOffsetY [3];
    /** Multiplier of scroll speed (in relation to player velocity) for the backgrounds */
    float bgScrollMul [3];

    Camera2D camera;

} game_data_t;


/* Update functions */

/** The main update function of the game, calls logic of, and allows interactions between, game objects */
void game_update(game_data_t * gameData);


/* Render functions */

/** The main render function of the game, draws (or calls the draw of) everything game-related */
void game_render(game_data_t * gameData);


/* Load/Unload functions */

/** Initialize an empty game_data_t structure to default values */
void game_initStructure(game_data_t * gameData);

/** Load all the textures necessary for the game */
void game_loadTex(game_data_t * gameData);

/** Unload all textures used for the game */
void game_unloadTex(game_data_t * gameData);

#endif /* GAME_H */