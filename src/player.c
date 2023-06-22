#include "player.h"


/* Update functions */

void player_update(player_t * playerData) {

    /* Throttle control */

    if(IsKeyPressed(KEY_RIGHT) && playerData->throttle < 100) {
        playerData->throttle += 10;
    } else if(IsKeyPressed(KEY_LEFT) && playerData->throttle > 0) {
        playerData->throttle -= 10;
    } else if(IsKeyPressed(KEY_RIGHT) && IsKeyDown(KEY_LEFT_CONTROL)) {
        playerData->throttle = 100;
    } else if(IsKeyPressed(KEY_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)) {
        playerData->throttle = 0;
    }


    /* Acceleration control */
    
    /* Current throttled acceleration value */
    int tAccel = playerData->accel * (playerData->throttle/100.0f);
    /* Current angular acceleration (can change for the frame based on player input) */
    int cAngAccel = playerData->angAccel;

    /* WSQE acceleration input processing */
    if(IsKeyDown(KEY_W)) {
        playerData->velocity.x += tAccel * cos(util_toRad(playerData->rot)) * GetFrameTime();
        playerData->velocity.y += tAccel * sin(util_toRad(playerData->rot)) * GetFrameTime();
        cAngAccel += (tAccel * cos(util_toRad(playerData->gimbal)));
    } else if(IsKeyDown(KEY_S)) {
        playerData->velocity.x -= playerData->rcsAccel * cos(util_toRad(playerData->rot)) * GetFrameTime();
        playerData->velocity.y -= playerData->rcsAccel * sin(util_toRad(playerData->rot)) * GetFrameTime();
    }
    if(IsKeyDown(KEY_Q)) {
        playerData->velocity.x += playerData->rcsAccel * sin(util_toRad(playerData->rot)) * GetFrameTime();
        playerData->velocity.y -= playerData->rcsAccel * cos(util_toRad(playerData->rot)) * GetFrameTime();
    } else if(IsKeyDown(KEY_E)) {
        playerData->velocity.x -= playerData->rcsAccel * sin(util_toRad(playerData->rot)) * GetFrameTime();
        playerData->velocity.y += playerData->rcsAccel * cos(util_toRad(playerData->rot)) * GetFrameTime();
    }

    /* AD rotation input processing */
    if(IsKeyDown(KEY_A)) {
        playerData->angVelocity -= cAngAccel * GetFrameTime();
    }
    if(IsKeyDown(KEY_D)) {
        playerData->angVelocity += cAngAccel * GetFrameTime();
    }


    /* Velocity dampening */

    /* Manual velocity killing - blocks all other forms of acceleration */
    if(IsKeyDown(KEY_LEFT_SHIFT)) {
        if(abs(playerData->velocity.x) > 1)
            playerData->velocity.x -= util_sign(playerData->velocity.x) * playerData->rcsAccel * GetFrameTime();
        else
            playerData->velocity.x = 0;
        if(abs(playerData->velocity.y) > 1)
            playerData->velocity.y -= util_sign(playerData->velocity.y) * playerData->rcsAccel * GetFrameTime();
        else
            playerData->velocity.y = 0;
        if(abs(playerData->angVelocity) > 1)
            playerData->angVelocity -= util_sign(playerData->angVelocity) * playerData->angAccel * GetFrameTime();
        else
            playerData->angVelocity = 0;
    } else if(IsKeyDown(KEY_R) || (playerData->angDampen && !(IsKeyDown(KEY_A) || IsKeyDown(KEY_D)))) {
        if(abs(playerData->angVelocity) > 1)
            playerData->angVelocity -= util_sign(playerData->angVelocity) * playerData->angAccel * GetFrameTime();
        else
            playerData->angVelocity = 0;
    }

    /* Automatic dampening (if turned on) - dampens everything except desired acceleration */
    // TODO Implement


    /* Position and rotation update */

    playerData->posX += playerData->velocity.x * GetFrameTime();
    playerData->posY += playerData->velocity.y * GetFrameTime();
    playerData->rot += playerData->angVelocity * GetFrameTime();

}


/* Render functions */

void player_render(player_t playerData) {
    
    /* Drawing the player texture */
    DrawTexturePro(
        playerData.ship, 
        (Rectangle){0, 0, playerData.ship.width, playerData.ship.height}, 
        (Rectangle){playerData.posX, playerData.posY, playerData.ship.width, playerData.ship.height}, 
        (Vector2){playerData.ship.width/2, playerData.ship.height/2}, 
        playerData.rot, 
        WHITE);

}


/* Other functions */

void player_initData(player_t * playerData) {
    
    playerData->posX = 100;
    playerData->posY = 100;
    playerData->rot = 0;

    playerData->velocity = Vector2Zero();
    playerData->angVelocity = 0;

    playerData->accel = 200;
    playerData->angAccel = 30;
    playerData->rcsAccel = 60;
    playerData->gimbal = 15;
    playerData->throttle = 100;

    playerData->dampen = 0;
    playerData->angDampen = 1;

}

void player_loadTex(player_t * playerData) {
    playerData->ship = LoadTexture("res/ship.png");
}

void player_unloadTex(player_t * playerData) {
    UnloadTexture(playerData->ship);
}
