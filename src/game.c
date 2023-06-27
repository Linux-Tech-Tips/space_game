#include "game.h"

/* Update functions */

void game_update(game_data_t * gameData) {

    /* Player update */
    player_update(&gameData->playerData);

    /* Background scroll (offset updating) */
    for(int i = 0; i < 3; i++) {
        gameData->bgOffsetX[i] -= gameData->playerData.velocity.x * gameData->bgScrollMul[i] * GetFrameTime();
        gameData->bgOffsetY[i] -= gameData->playerData.velocity.y * gameData->bgScrollMul[i] * GetFrameTime();

        int bgW = gameData->textures.background[i].width;
        int bgH = gameData->textures.background[i].height;

        if(gameData->bgOffsetX[i] > bgW)
            gameData->bgOffsetX[i] -= bgW;
        else if(gameData->bgOffsetX[i] < 0)
            gameData->bgOffsetX[i] += bgW;
        if(gameData->bgOffsetY[i] > bgH)
            gameData->bgOffsetY[i] -= bgH;
        else if(gameData->bgOffsetY[i] < 0)
            gameData->bgOffsetY[i] += bgH;
    }

    /* Bullet creation */
    if(gameData->playerData.shoot) {
        bullet_t newBullet = {0};
        Vector2 posOffset = Vector2Scale((Vector2){cos(util_toRad(gameData->playerData.rot)), sin(util_toRad(gameData->playerData.rot))}, 48);
        Vector2 bPos = Vector2Add(gameData->playerData.pos, posOffset);
        Vector2 bVelocity = Vector2Scale((Vector2){cos(util_toRad(gameData->playerData.rot)), sin(util_toRad(gameData->playerData.rot))}, 2000);
        bullet_create(&newBullet, &gameData->textures.playerBullet, bPos, gameData->playerData.rot, Vector2Add(gameData->playerData.velocity, bVelocity));
        bullet_list_add(gameData->bullets, newBullet, &gameData->bulletCount, BULLET_AMOUNT);
    }

    /* Bullet update */
    for(int i = 0; i < gameData->bulletCount; i++) {
        bullet_update(&gameData->bullets[i]);
        /* Removing bullets which have flown too far away from the player */
        if(Vector2Distance(gameData->bullets[i].pos, gameData->playerData.pos) > 2500) {
            bullet_list_remove(gameData->bullets, i, &gameData->bulletCount);
        }
    }

}

/* Render functions */

void game_render(game_data_t * gameData) {

    /* Using a Raylib 2D camera mode for rendering */
    BeginMode2D(gameData->camera);

        /* Local variables to make background render more clear */
        float playerPosX = gameData->playerData.pos.x;
        float playerPosY = gameData->playerData.pos.y;

        /* Drawing backgrounds */
        for(int i = 0; i < 3; i++) {
            int bgW = gameData->textures.background[i].width;
            int bgH = gameData->textures.background[i].height;

            DrawTexturePro(
                gameData->textures.background[i], 
                (Rectangle){0, 0, 9*bgW, 9*bgH}, 
                (Rectangle){playerPosX - 4.5f*bgW + gameData->bgOffsetX[i], playerPosY - 4.5f*bgH + gameData->bgOffsetY[i], 9*bgW, 9*bgH}, 
                (Vector2){0, 0}, 
                0, 
                WHITE);
        }

        /* Render everything on top of background only after background */

        /* Drawing bullets under the player */
        for(int i = 0; i < gameData->bulletCount; i++)
            bullet_render(gameData->bullets[i]);

        player_render(gameData->playerData);

    EndMode2D();
}

/* Load/Unload functions */

void game_initStructure(game_data_t * gameData) {

    /* Initializing default values */
    
    gameData->paused = 0;
    
    /* Player */
    player_initData(&gameData->playerData, &gameData->textures.playerShip, &gameData->textures.playerExhaust);

    /* Bullets */
    gameData->bulletCount = 0;

    /* Backgrounds */
    for(int i = 0; i < 3; i++) {
        gameData->bgOffsetX[i] = 0;
        gameData->bgOffsetY[i] = 0;
    }
    gameData->bgScrollMul[0] = 0.15f;
    gameData->bgScrollMul[1] = 0.25f;
    gameData->bgScrollMul[2] = 0.4f;

    /* Camera */
    gameData->camera = (Camera2D){0};
    gameData->camera.zoom = 1.0f;
}

void game_loadTex(game_textures_t * texData) {
    texData->background[0] = LoadTexture("res/bg_far.png");
    texData->background[1] = LoadTexture("res/bg_mid.png");
    texData->background[2] = LoadTexture("res/bg_near.png");

    texData->playerShip = LoadTexture("res/player.png");
    texData->playerExhaust = LoadTexture("res/player_exhaust.png");

    texData->playerBullet = LoadTexture("res/player_bullet.png");
}

void game_unloadTex(game_textures_t * texData) {
    UnloadTexture(texData->background[0]);
    UnloadTexture(texData->background[1]);
    UnloadTexture(texData->background[2]);

    UnloadTexture(texData->playerShip);
    UnloadTexture(texData->playerExhaust);
    UnloadTexture(texData->playerBullet);
}