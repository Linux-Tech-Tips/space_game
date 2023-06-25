#include "player.h"


/* Update functions */

void player_update(player_t * playerData) {

    /* Resetting single-use variables */
    playerData->accel = Vector2Zero();
    playerData->shoot = 0;

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

    /* Shooting control */
    if(playerData->shootCounter > 0) {
        playerData->shootCounter -= GetFrameTime();
    } else {
        if(IsKeyDown(KEY_SPACE)) {
            playerData->shoot = 1;
            playerData->shootCounter = playerData->shootDelay;
        }
    }


    /* Acceleration control */
    
    /* Current throttled acceleration size */
    int tAccelVal = playerData->accelVal * (playerData->throttle/100.0f);
    /* Angular acceleration size in the current frame (can change for the frame based on player input) */
    int fAngAccelVal = playerData->angAccelVal;

    /* WSQE acceleration input processing */
    if(IsKeyDown(KEY_W)) {
        playerData->accel.x += tAccelVal * cos(util_toRad(playerData->rot));
        playerData->accel.y += tAccelVal * sin(util_toRad(playerData->rot));
        fAngAccelVal += (tAccelVal * cos(util_toRad(playerData->gimbal)));
    } else if(IsKeyDown(KEY_S)) {
        playerData->accel.x -= playerData->rcsAccelVal * cos(util_toRad(playerData->rot));
        playerData->accel.y -= playerData->rcsAccelVal * sin(util_toRad(playerData->rot));
    }
    if(IsKeyDown(KEY_Q)) {
        playerData->accel.x += playerData->rcsAccelVal * sin(util_toRad(playerData->rot));
        playerData->accel.y -= playerData->rcsAccelVal * cos(util_toRad(playerData->rot));
    } else if(IsKeyDown(KEY_E)) {
        playerData->accel.x -= playerData->rcsAccelVal * sin(util_toRad(playerData->rot));
        playerData->accel.y += playerData->rcsAccelVal * cos(util_toRad(playerData->rot));
    }

    /* AD rotation input processing */
    if(IsKeyDown(KEY_A)) {
        playerData->angVelocity -= fAngAccelVal * GetFrameTime();
    }
    if(IsKeyDown(KEY_D)) {
        playerData->angVelocity += fAngAccelVal * GetFrameTime();
    }


    /* Velocity dampening */

    /* Manual velocity killing - blocks all other forms of acceleration */
    if(IsKeyDown(KEY_LEFT_SHIFT)) {
        /* Dampening velocity */
        Vector2 decel = Vector2Scale(Vector2Normalize(playerData->velocity), playerData->rcsAccelVal);
        /* Decelerating only as much as the velocity of the ship */
        if(Vector2Length(playerData->velocity) > Vector2Length(decel)) {
            playerData->accel = Vector2Negate(decel);
        } else {
            playerData->accel = Vector2Negate(playerData->velocity);
        }
        /* Dampening angular velocity */
        if(abs(playerData->angVelocity) > VELOCITY_CUTOFF_VAL)
            playerData->angVelocity -= util_sign(playerData->angVelocity) * playerData->angAccelVal * GetFrameTime();
        else
            playerData->angVelocity = 0;
    /* Dampening only angular velocity - manual/automatic */
    } else if(IsKeyDown(KEY_R) || (playerData->angDampen && !(IsKeyDown(KEY_A) || IsKeyDown(KEY_D)))) {
        if(abs(playerData->angVelocity) > VELOCITY_CUTOFF_VAL)
            playerData->angVelocity -= util_sign(playerData->angVelocity) * playerData->angAccelVal * GetFrameTime();
        else
            playerData->angVelocity = 0;
    }


    /* Velocity, position and rotation update */

    playerData->velocity = Vector2Add(playerData->velocity, Vector2Scale(playerData->accel, GetFrameTime()));

    /* If velocity too small, automatically kill */
    if(Vector2Length(playerData->velocity) <= VELOCITY_CUTOFF_VAL) {
        playerData->velocity = Vector2Zero();
    }

    playerData->pos = Vector2Add(playerData->pos, Vector2Scale(playerData->velocity, GetFrameTime()));
    playerData->rot += playerData->angVelocity * GetFrameTime();

}


/* Render functions */

void player_render(player_t playerData) {

    int shipWidth = playerData.ship->width;
    int shipHeight = playerData.ship->height;

    /* Drawing the player texture */
    DrawTexturePro(
        *playerData.ship,
        (Rectangle){0, 0, shipWidth, shipHeight}, 
        (Rectangle){playerData.pos.x, playerData.pos.y, shipWidth, shipHeight}, 
        (Vector2){shipWidth/2.0f, shipHeight/2.0f}, 
        playerData.rot, 
        WHITE);

}


/* Other functions */

void player_initData(player_t * playerData, Texture2D * playerTex) {
    
    playerData->pos = (Vector2){100, 100};
    playerData->rot = 0;

    playerData->velocity = Vector2Zero();
    playerData->angVelocity = 0;

    playerData->accelVal = 200;
    playerData->angAccelVal = 30;
    playerData->rcsAccelVal = 80;
    playerData->gimbal = 15;
    playerData->throttle = 100;

    playerData->dampen = 1;
    playerData->angDampen = 1;

    playerData->shoot = 0;
    playerData->shootDelay = 0.15f;
    playerData->shootCounter = playerData->shootDelay;

    playerData->ship = playerTex;

}
