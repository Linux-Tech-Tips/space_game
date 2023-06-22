#include "game.h"

/* Update functions */

void game_update(game_data_t * gameData) {

    /* Player update */
    player_update(&gameData->playerData);

    /* Background scroll (offset updating) */
    for(int i = 0; i < 3; i++) {
        gameData->bgOffsetX[i] -= gameData->playerData.velocity.x * gameData->bgScrollMul[i] * GetFrameTime();
        gameData->bgOffsetY[i] -= gameData->playerData.velocity.y * gameData->bgScrollMul[i] * GetFrameTime();

        int bgW = gameData->background[i].width;
        int bgH = gameData->background[i].height;

        if(gameData->bgOffsetX[i] > bgW)
            gameData->bgOffsetX[i] -= bgW;
        else if(gameData->bgOffsetX[i] < 0)
            gameData->bgOffsetX[i] += bgW;
        if(gameData->bgOffsetY[i] > bgH)
            gameData->bgOffsetY[i] -= bgH;
        else if(gameData->bgOffsetY[i] < 0)
            gameData->bgOffsetY[i] += bgH;
    }

}

/* Render functions */

void game_render(game_data_t * gameData) {

    /* Using a Raylib 2D camera mode for rendering */
    BeginMode2D(gameData->camera);

        /* Local variables to make background render more clear */
        float playerPosX = gameData->playerData.posX;
        float playerPosY = gameData->playerData.posY;

        /* Drawing backgrounds */
        for(int i = 0; i < 3; i++) {
            int bgW = gameData->background[i].width;
            int bgH = gameData->background[i].height;

            DrawTexturePro(gameData->background[i], (Rectangle){0, 0, 9*bgW, 9*bgH}, (Rectangle){playerPosX - 4.5f*bgW + gameData->bgOffsetX[i], playerPosY - 4.5f*bgH + gameData->bgOffsetY[i], 9*bgW, 9*bgH}, (Vector2){0, 0}, 0, WHITE);
        }

        /* Render everything on top of background only after background */

        player_render(gameData->playerData);

    EndMode2D();
}

/* Load/Unload functions */

void game_initStructure(game_data_t * gameData) {

    /* Initializing default values */
    
    gameData->paused = 0;
    
    /* Player */
    player_initData(&gameData->playerData);

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

void game_loadTex(game_data_t * gameData) {
    gameData->background[0] = LoadTexture("res/bg_far.png");
    gameData->background[1] = LoadTexture("res/bg_mid.png");
    gameData->background[2] = LoadTexture("res/bg_near.png");
    
    player_loadTex(&gameData->playerData);
}

void game_unloadTex(game_data_t * gameData) {
    UnloadTexture(gameData->background[0]);
    UnloadTexture(gameData->background[1]);
    UnloadTexture(gameData->background[2]);
    
    player_unloadTex(&gameData->playerData);
}