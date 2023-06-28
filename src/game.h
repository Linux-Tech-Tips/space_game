#ifndef GAME_H
#define GAME_H

#include <stdlib.h>

#include <raylib.h>
#include <raymath.h>
#include <math.h>

#include "util.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"

/** The maximum amount of bullets allowed to exist in the world at once */
#define BULLET_AMOUNT 100
/** The maximum amount of enemies allowed to exist in the world at once */
#define ENEMY_AMOUNT 100

/** The distance from the player after which a bullet despawns */
#define BULLET_DESPAWN_DIST 2500
/** The distance from the player after which a non-persistent enemy despawns */
#define ENEMY_DESPAWN_DIST 3000

/* NOTE: Could later be changed to be more dynamic, though for demo purposes, this is sufficient */
/** Holds all required textures, loads the ones which are currently necessary for the game context on game load */
typedef struct {

    /** Array of 3 scrolling backgrounds */
    Texture2D background [3];

    /** The player's ship */
    Texture2D playerShip;
    /** The exhaust of the player ship's thrusters */
    Texture2D playerExhaust;
    /** The player's bullet */
    Texture2D playerBullet;

    /** An asteroid */
    Texture2D asteroid;

} game_textures_t;

/** The game's main data structure, includes all data relevant to gameplay */
typedef struct {
    /** The pause state of the game */
    short paused;

    /** The data of the current player */
    player_t playerData;

    /** All the currently existing bullets */
    bullet_t bullets [BULLET_AMOUNT];
    /** The amount of the currently existing bullets */
    int bulletCount;

    /** All the currently existing enemies */
    enemy_t enemies [ENEMY_AMOUNT];
    /** The amount of currently existing enemies */
    int enemyCount;

    /** The x/y offset of the background texture in relation to the camera */
    float bgOffsetX [3], bgOffsetY [3];
    /** Multiplier of scroll speed (in relation to player velocity) for the backgrounds */
    float bgScrollMul [3];

    /** The collection of textures loaded for the purposes of the game */
    game_textures_t textures;

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
void game_loadTex(game_textures_t * texData);

/** Unload all textures used for the game */
void game_unloadTex(game_textures_t * texData);

#endif /* GAME_H */