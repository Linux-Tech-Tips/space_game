/**
 * @file game.h
 * @author Linux-Tech-Tips
 * @brief General game data and logic
 *
 * Contains general game data (the @ref game_data_t data structure) and logic.
 * Encapsulates the player, enemies and bullets, calls their logic and allows their interaction.
 * Not responsible for specific logic of the structures contained within.
 */

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
#define BULLET_MAX_AMOUNT 100
/** The maximum amount of enemies allowed to exist in the world at once */
#define ENEMY_MAX_AMOUNT 100

/** The distance from the player after which a bullet despawns */
#define BULLET_DESPAWN_DIST 2500
/** The distance from the player after which a non-persistent enemy despawns */
#define ENEMY_DESPAWN_DIST 5000

/** How strongly should object collisions damage the objects */
#define COLLISION_DAMAGE_MUL 0.2f

/** The X unit dimension of the repeating asteroid field */
#define ASTEROID_FIELD_X 8000
/** The Y unit dimension of the repeating asteroid field */
#define ASTEROID_FIELD_Y 8000
/** The amount of asteroids in the unit quad */
#define ASTEROID_FIELD_AMOUNT 20
/** By how much an asteroid can shift when changing position between field units */
#define ASTEROID_MOVE_OFFSET 64
/** How many asteroids have to be missing to regenerate the field */
#define ASTEROID_REGEN_TRIGGER 10

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

    /** Basic enemy ship */
    Texture2D enemyShip;
    /** Basic enemy ship thrusters exhaust */
    Texture2D enemyExhaust;
    /** Basic enemy bullet */
    Texture2D enemyBullet;

} game_textures_t;

/** The game's main data structure, includes all data relevant to gameplay */
typedef struct {
    /** The pause state of the game */
    short paused;
    /** If true, the player is destroyed and the game is over */
    short over;

    /** The data of the current player */
    player_t playerData;

    /** All the currently existing bullets */
    bullet_t bullets [BULLET_MAX_AMOUNT];
    /** The amount of the currently existing bullets */
    int bulletCount;

    /** All the currently existing enemies */
    enemy_t enemies [ENEMY_MAX_AMOUNT];
    /** The amount of currently existing enemies */
    int enemyCount;
    /** The amount of the enemies of the basic type */
    int basicCount;

    /** The timer for the spawning of new enemies */
    float enemySpawnCooldown;
    /** The delay between enemy waves, decreases over time */
    float enemySpawnDelay;
    /** The default delay between waves */
    float defEnemySpawnDelay;
    /** Which wave the player is on */
    int wave;
    /** How many enemies the player has killed */
    int score;
    /** The best score so far */
    int highScore;

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

/** Updates all the bullets in the list (including collisions) */
void _game_update_bulletList(game_data_t * gameData);
/** Updates all the enemies in the list (including collisions) */
void _game_update_enemyList(game_data_t * gameData, int * asteroidCount, int * basicCount);


/* Render functions */

/** The main render function of the game, draws (or calls the draw of) everything game-related */
void game_render(game_data_t * gameData);


/* Load/Unload functions */

/** Initialize an empty game_data_t structure to default values */
void game_initStructure(game_data_t * gameData);

/** Resets a game structure to new game values */
void game_resetStructure(game_data_t * gameData);

/** Load all the textures necessary for the game */
void game_loadTex(game_textures_t * texData);

/** Unload all textures used for the game */
void game_unloadTex(game_textures_t * texData);


/* Generation functions */

/** 
 * Generates an asteroid field of a rectangle with the given parameters, with the center at (0,0).
 * Corrects asteroid positions to not generate within an existing player instance.
 */
void game_genAsteroids(game_data_t * gameData, int fieldSizeX, int fieldSizeY, int amount);

/** 
 * If less asteroids than the given threshold @ref ASTEROID_REGEN_TRIGGER exist, a randomly selected amount are recreated. 
 * The newly regenerated asteroids are contained within a rectangle with the center at the position of the player.
 * Positions of the structures are corrected to not appear within the camera's viewport.
 */
void game_regenAsteroids(game_data_t * gameData, int fieldSizeX, int fieldSizeY, int maxAmount, int cAmount);

/** Creates a wave of enemies with the given parameters */
void game_genEnemies(game_data_t * gameData, int minDist, int radius, int amount, enemy_type_t enemyType, Vector2 defVelocity);

#endif /* GAME_H */