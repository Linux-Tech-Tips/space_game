#include "game.h"

/* Update functions */

void game_update(game_data_t * gameData) {

    /* Player logic - TODO Move into separate module */

    /* Throttle setting */
    switch(GetKeyPressed()) {
        case KEY_ONE:
            gameData->throttle = 20;
            break;
        case KEY_TWO:
            gameData->throttle = 40;
            break;
        case KEY_THREE:
            gameData->throttle = 60;
            break;
        case KEY_FOUR:
            gameData->throttle = 80;
            break;
        case KEY_FIVE:
            gameData->throttle = 100;
            break;
    }

    int angAccel = gameData->angAccel;
    int tAccel = gameData->accel * (gameData->throttle/100.0f);

    /* Moving ship */
    if(IsKeyDown(KEY_W)) {
        gameData->velocity.x += tAccel * cos(util_toRad(gameData->rot)) * GetFrameTime();
        gameData->velocity.y += tAccel * sin(util_toRad(gameData->rot)) * GetFrameTime();
        angAccel += tAccel;
    } else if(IsKeyDown(KEY_S)) {
        gameData->velocity.x -= gameData->rcsAccel * cos(util_toRad(gameData->rot)) * GetFrameTime();
        gameData->velocity.y -= gameData->rcsAccel * sin(util_toRad(gameData->rot)) * GetFrameTime();
    }
    if(IsKeyDown(KEY_Q)) {
        gameData->velocity.x += gameData->rcsAccel * sin(util_toRad(gameData->rot)) * GetFrameTime();
        gameData->velocity.y -= gameData->rcsAccel * cos(util_toRad(gameData->rot)) * GetFrameTime();
    } else if(IsKeyDown(KEY_E)) {
        gameData->velocity.x -= gameData->rcsAccel * sin(util_toRad(gameData->rot)) * GetFrameTime();
        gameData->velocity.y += gameData->rcsAccel * cos(util_toRad(gameData->rot)) * GetFrameTime();
    }

    /* Rotating ship */
    if(IsKeyDown(KEY_A)) {
        gameData->angMoment -= angAccel * GetFrameTime();
    }
    if(IsKeyDown(KEY_D)) {
        gameData->angMoment += angAccel * GetFrameTime();
    }

    /* Velocity dampening */
    if(IsKeyDown(KEY_LEFT_SHIFT)) {
        if(abs(gameData->velocity.x) > 1)
            gameData->velocity.x -= util_sign(gameData->velocity.x) * gameData->rcsAccel * GetFrameTime();
        else
            gameData->velocity.x = 0;
        if(abs(gameData->velocity.y) > 1)
            gameData->velocity.y -= util_sign(gameData->velocity.y) * gameData->rcsAccel * GetFrameTime();
        else
            gameData->velocity.y = 0;
        if(abs(gameData->angMoment) > 1)
            gameData->angMoment -= util_sign(gameData->angMoment) * gameData->angAccel * GetFrameTime();
        else
            gameData->angMoment = 0;
    } else if(IsKeyDown(KEY_R) || (gameData->dampenAng && !(IsKeyDown(KEY_A) || IsKeyDown(KEY_D)))) {
        if(abs(gameData->angMoment) > 1)
            gameData->angMoment -= util_sign(gameData->angMoment) * gameData->angAccel * GetFrameTime();
        else
            gameData->angMoment = 0;
    }

    gameData->posX += gameData->velocity.x * GetFrameTime();
    gameData->posY += gameData->velocity.y * GetFrameTime();
    gameData->rot += gameData->angMoment * GetFrameTime();


    /* Backgrounds scroll (offset updating) */
    for(int i = 0; i < 3; i++) {
        gameData->bgOffsetX[i] -= gameData->velocity.x * gameData->bgScrollMul[i] * GetFrameTime();
        gameData->bgOffsetY[i] -= gameData->velocity.y * gameData->bgScrollMul[i] * GetFrameTime();

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

void game_render(game_data_t * gameData, int scrX, int scrY) {

    /* Using a Raylib 2D camera mode for rendering */
    BeginMode2D(gameData->camera);

        /* Local variables to make background render more clear */
        float posX = gameData->posX;
        float posY = gameData->posY;
        int shipW = gameData->ship.width;
        int shipH = gameData->ship.height;

        /* Drawing backgrounds */
        for(int i = 0; i < 3; i++) {
            int bgW = gameData->background[i].width;
            int bgH = gameData->background[i].height;

            DrawTexturePro(gameData->background[i], (Rectangle){0, 0, 9*bgW, 9*bgH}, (Rectangle){posX - 4.5f*bgW + gameData->bgOffsetX[i], posY - 4.5f*bgH + gameData->bgOffsetY[i], 9*bgW, 9*bgH}, (Vector2){0, 0}, 0, WHITE);
        }

        /* Render everything on top of background only after background */

        /* Drawing player - TODO Move to separate module */
        DrawTexturePro(gameData->ship, (Rectangle){0, 0, shipW, shipH}, (Rectangle){posX, posY, shipW, shipH}, (Vector2){shipW/2, shipH/2}, gameData->rot, WHITE);

    EndMode2D();
}

/* Load/Unload functions */

void game_initStructure(game_data_t * gameData) {
    /* Default values - TODO Move player default values away */
    gameData->paused = 0;
    
    gameData->posX = 100;
    gameData->posY = 100;
    gameData->rot = 0;

    gameData->velocity = Vector2Zero();
    gameData->angMoment = 0;

    gameData->accel = 200;
    gameData->angAccel = 30;
    gameData->rcsAccel = 60;
    gameData->throttle = 100;

    gameData->dampenAng = 1;

    for(int i = 0; i < 3; i++) {
        gameData->bgOffsetX[i] = 0;
        gameData->bgOffsetY[i] = 0;
    }
    gameData->bgScrollMul[0] = 0.15f;
    gameData->bgScrollMul[1] = 0.25f;
    gameData->bgScrollMul[2] = 0.4f;

    gameData->camera = (Camera2D){0};
    gameData->camera.zoom = 1.0f;
}

void game_loadTex(game_data_t * gameData) {
    gameData->background[0] = LoadTexture("res/bg_far.png");
    gameData->background[1] = LoadTexture("res/bg_mid.png");
    gameData->background[2] = LoadTexture("res/bg_near.png");
    gameData->ship = LoadTexture("res/ship.png");
}

void game_unloadTex(game_data_t * gameData) {
    UnloadTexture(gameData->background[0]);
    UnloadTexture(gameData->background[1]);
    UnloadTexture(gameData->background[2]);
    UnloadTexture(gameData->ship);
}