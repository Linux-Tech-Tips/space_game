#include "player.h"


/* Update functions */

void player_update(player_t * playerData) {

    /* Throttle control */

    if(IsKeyPressed(KEY_RIGHT) && playerData->throttle < 100) {
        playerData->throttle += 10;
    } else if(IsKeyPressed(KEY_LEFT) && playerData->throttle > 0) {
        playerData->throttle -= 10;
    } else if(IsKeyPressed(KEY_END)) {
        playerData->throttle = 100;
    } else if(IsKeyPressed(KEY_HOME)) {
        playerData->throttle = 0;
    }


    /* Acceleration control */
    
    /* Current throttled acceleration size */
    int tAccel = playerData->accel * (playerData->throttle/100.0f);
    /* Current angular acceleration (can change for the frame based on player input) */
    int cAngAccel = playerData->angAccel;

    /* The actual acceleration vector as a local variable, based on the acceleration size and player input */
    Vector2 accel = Vector2Zero();

    /* WSQE acceleration input processing */
    if(IsKeyDown(KEY_W)) {
        accel.x += tAccel * cos(util_toRad(playerData->rot));
        accel.y += tAccel * sin(util_toRad(playerData->rot));
        cAngAccel += (tAccel * cos(util_toRad(playerData->gimbal)));
    } else if(IsKeyDown(KEY_S)) {
        accel.x -= playerData->rcsAccel * cos(util_toRad(playerData->rot));
        accel.y -= playerData->rcsAccel * sin(util_toRad(playerData->rot));
    }
    if(IsKeyDown(KEY_Q)) {
        accel.x += playerData->rcsAccel * sin(util_toRad(playerData->rot));
        accel.y -= playerData->rcsAccel * cos(util_toRad(playerData->rot));
    } else if(IsKeyDown(KEY_E)) {
        accel.x -= playerData->rcsAccel * sin(util_toRad(playerData->rot));
        accel.y += playerData->rcsAccel * cos(util_toRad(playerData->rot));
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
        /* Dampening velocity */
        Vector2 decel = Vector2Scale(Vector2Normalize(playerData->velocity), playerData->rcsAccel * GetFrameTime());
        if(Vector2Length(playerData->velocity) > Vector2Length(decel)) {
            playerData->velocity = Vector2Subtract(playerData->velocity, decel);
        } else {
            playerData->velocity = Vector2Zero();
        }
        /* Dampening angular velocity */
        if(abs(playerData->angVelocity) > 1)
            playerData->angVelocity -= util_sign(playerData->angVelocity) * playerData->angAccel * GetFrameTime();
        else
            playerData->angVelocity = 0;
        /* Stopping the ship from accelerating */
        accel = Vector2Zero();
    } else if(IsKeyDown(KEY_R) || (playerData->angDampen && !(IsKeyDown(KEY_A) || IsKeyDown(KEY_D)))) {
        if(abs(playerData->angVelocity) > 1)
            playerData->angVelocity -= util_sign(playerData->angVelocity) * playerData->angAccel * GetFrameTime();
        else
            playerData->angVelocity = 0;
    }


    /* Velocity, position and rotation update */

    playerData->velocity = Vector2Add(playerData->velocity, Vector2Scale(accel, GetFrameTime()));

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
    playerData->rcsAccel = 80;
    playerData->gimbal = 15;
    playerData->throttle = 100;

    playerData->dampen = 1;
    playerData->angDampen = 1;

}

void player_loadTex(player_t * playerData) {
    playerData->ship = LoadTexture("res/ship.png");
}

void player_unloadTex(player_t * playerData) {
    UnloadTexture(playerData->ship);
}
