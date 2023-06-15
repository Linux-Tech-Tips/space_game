#include "game.h"

/* Update functions */

void game_update(game_data_t * gameData) {

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
        gameData->velocity.x += tAccel * cos(game_toRad(gameData->rot)) * GetFrameTime();
        gameData->velocity.y += tAccel * sin(game_toRad(gameData->rot)) * GetFrameTime();
        angAccel += tAccel;
    } else if(IsKeyDown(KEY_S)) {
        gameData->velocity.x -= gameData->rcsAccel * cos(game_toRad(gameData->rot)) * GetFrameTime();
        gameData->velocity.y -= gameData->rcsAccel * sin(game_toRad(gameData->rot)) * GetFrameTime();
    }
    if(IsKeyDown(KEY_Q)) {
        gameData->velocity.x += gameData->rcsAccel * sin(game_toRad(gameData->rot)) * GetFrameTime();
        gameData->velocity.y -= gameData->rcsAccel * cos(game_toRad(gameData->rot)) * GetFrameTime();
    } else if(IsKeyDown(KEY_E)) {
        gameData->velocity.x -= gameData->rcsAccel * sin(game_toRad(gameData->rot)) * GetFrameTime();
        gameData->velocity.y += gameData->rcsAccel * cos(game_toRad(gameData->rot)) * GetFrameTime();
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
            gameData->velocity.x -= game_sign(gameData->velocity.x) * gameData->rcsAccel * GetFrameTime();
        else
            gameData->velocity.x = 0;
        if(abs(gameData->velocity.y) > 1)
            gameData->velocity.y -= game_sign(gameData->velocity.y) * gameData->rcsAccel * GetFrameTime();
        else
            gameData->velocity.y = 0;
        if(abs(gameData->angMoment) > 1)
            gameData->angMoment -= game_sign(gameData->angMoment) * gameData->angAccel * GetFrameTime();
        else
            gameData->angMoment = 0;
    } else if(IsKeyDown(KEY_R)) {
        if(abs(gameData->angMoment) > 1)
            gameData->angMoment -= game_sign(gameData->angMoment) * gameData->angAccel * GetFrameTime();
        else
            gameData->angMoment = 0;
    }

    gameData->posX += gameData->velocity.x * GetFrameTime();
    gameData->posY += gameData->velocity.y * GetFrameTime();
    gameData->rot += gameData->angMoment * GetFrameTime();

    /* Temporary out of screen checks */
    if(gameData->posX > (GetScreenWidth() + 128))
        gameData->posX = -128;
    if(gameData->posY > (GetScreenHeight() + 128))
        gameData->posY = -128;
    
    if(gameData->posX < -128)
        gameData->posX = (GetScreenWidth() + 128);
    if(gameData->posY < -128)
        gameData->posY = (GetScreenHeight() + 128);
    

}

/* Render functions */

void game_render(game_data_t * gameData, int scrX, int scrY) {

    BeginMode2D(gameData->camera);

        DrawTextureEx(gameData->background, (Vector2){0, 0}, 0, (scrX/1600.0f), WHITE);

        int posX = gameData->posX;
        int posY = gameData->posY;
        int shipW = gameData->ship.width;
        int shipH = gameData->ship.height;

        DrawTexturePro(gameData->ship, (Rectangle){0, 0, shipW, shipH}, (Rectangle){posX, posY, shipW, shipH}, (Vector2){shipW/2, shipH/2}, gameData->rot, WHITE);

    EndMode2D();
}

/* Load/Unload functions */

void game_initStructure(game_data_t * gameData) {
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

    gameData->camera = (Camera2D){0};
    gameData->camera.zoom = 1;
}

void game_loadTex(game_data_t * gameData) {
    gameData->background = LoadTexture("res/bg.png");
    gameData->ship = LoadTexture("res/ship.png");
}

void game_unloadTex(game_data_t * gameData) {
    UnloadTexture(gameData->background);
    UnloadTexture(gameData->ship);
}

/* Utility functions */

float game_toRad(float deg) {
    return (deg * (PI/180));
}

int game_sign(float val) {
    if(val > 0)
        return 1;
    else if(val < 0)
        return -1;
    else
        return 0;
}