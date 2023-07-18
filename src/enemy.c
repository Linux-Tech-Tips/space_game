#include "enemy.h"

/* Update and render functions */

void enemy_update(enemy_t * enemy, Vector2 playerPos, float playerRot) {

    /* Resetting frame variables */
    enemy->accel = Vector2Zero();
    enemy->shoot = 0;
    enemy->thrustersOn = 0;
    enemy->rotating = 0;
    
    /* Updating AI */
    switch(enemy->shipType) {
        case asteroid:
            /* Do nothing */
            break;
        case basic:
            enemy_basicAI(enemy, playerPos, playerRot);
            break;
    }

    /* Updating movement */
    
    enemy->velocity = Vector2Add(enemy->velocity, Vector2Scale(enemy->accel, GetFrameTime()));

    /* If velocity too small, automatically kill */
    if(Vector2Length(enemy->velocity) <= VELOCITY_CUTOFF_VAL) {
        enemy->velocity = Vector2Zero();
    }

    enemy->pos = Vector2Add(enemy->pos, Vector2Scale(enemy->velocity, GetFrameTime()));
    enemy->rot += enemy->angVelocity * GetFrameTime();

    /* Shooting logic */
    if(enemy->shootCounter > 0) {
        enemy->shootCounter -= GetFrameTime();
    }

}

void enemy_render(enemy_t enemy) {

    int shipWidth = enemy.ship->width;
    int shipHeight = enemy.ship->height;

    /* Drawing the texture of the exhaust if W held */
    if(enemy.thrustersOn && enemy.shipType != asteroid) {

        /* Rotating exhaust if player rotates ship */
        float exhaustRot = enemy.rot;
        float exhaustX = enemy.pos.x; 
        float exhaustY = enemy.pos.y;
        if(enemy.rotating > 0) {
            exhaustRot += enemy.gimbal;
            exhaustX -= 10.0f * cos(util_toRad(90.0f - enemy.rot));
            exhaustY += 10.0f * sin(util_toRad(90.0f - enemy.rot));
        }
        if(enemy.rotating < 0) {
            exhaustRot -= enemy.gimbal;
            exhaustX += 10.0f * cos(util_toRad(90.0f - enemy.rot));
            exhaustY -= 10.0f * sin(util_toRad(90.0f - enemy.rot));
        }

        DrawTexturePro(
            *enemy.shipExhaust,
            (Rectangle){0, 0, shipWidth, shipHeight}, 
            (Rectangle){exhaustX, exhaustY, shipWidth, shipHeight}, 
            (Vector2){shipWidth/2.0f, shipHeight/2.0f},
            exhaustRot,
            WHITE);
    }

    /* Drawing the player texture */
    DrawTexturePro(
        *enemy.ship,
        (Rectangle){0, 0, shipWidth, shipHeight}, 
        (Rectangle){enemy.pos.x, enemy.pos.y, shipWidth, shipHeight}, 
        (Vector2){shipWidth/2.0f, shipHeight/2.0f}, 
        enemy.rot, 
        WHITE);

}


/* AI functions */

void enemy_basicAI(enemy_t * enemy, Vector2 playerPos, float playerRot) {
    
    float rotDiff = util_toDeg(Vector2Angle(util_dirVector(enemy->rot), Vector2Subtract(playerPos, enemy->pos)));
    /* The rotation difference should always be the smaller of the 2 possible angles */
    if(abs(rotDiff) > 180)
        rotDiff -= 360 * util_sign(rotDiff);

    /* Always rotate towards player */
    if(abs(rotDiff) > abs(enemy->angVelocity))
        enemy->angVelocity += (util_sign(rotDiff) * enemy->angAccelVal) * GetFrameTime();
    else
        enemy->angVelocity -= util_sign(enemy->angVelocity) * enemy->angAccelVal * GetFrameTime();
    
    
    float dist = Vector2Distance(enemy->pos, playerPos);
    /* The distance the enemy would spend decelerating with the current velocity and accel value */
    float decelDist = pow(Vector2Length(enemy->velocity), 2) / (2 * enemy->accelVal);

    /* If far enough away, accelerate, otherwise decelerate */
    if(decelDist < (dist - BASIC_STOP_DIST) && abs(rotDiff) < 25) {
        enemy->accel = Vector2Scale(util_dirVector(enemy->rot), enemy->accelVal);
        enemy->thrustersOn = 1;
    } else {
        enemy->accel = Vector2Scale(Vector2Normalize(enemy->velocity), -enemy->rcsAccelVal);
    }

    /* Shoot if close enough */
    if(dist < (10 * BASIC_STOP_DIST) && abs(rotDiff) < 90) {
        if(enemy->shootCounter <= 0) {
            enemy->shoot = 1;
            enemy->shootCounter = enemy->shootDelay;
        }
    }

}


/* Enemy list functions */

void enemy_list_remove(enemy_t eList [], int index, int * lSize) {
    for(int i = index; i < *lSize-1; i++) {
        eList[i] = eList[i+1];
    }
    *lSize -= 1;
}

void enemy_list_add(enemy_t eList [], enemy_t enemy, int * lSize, int maxSize) {
    if(*lSize >= maxSize) {
        enemy_list_remove(eList, 0, lSize);
    }
    eList[*lSize] = enemy;
    *lSize += 1;
}


/* Other functions */

void enemy_initData(enemy_t * enemy, Vector2 pos, float rot, Vector2 velocity, short persistent, enemy_type_t shipType, Texture2D * shipTex, Texture2D * shipExhaustTex) {
    
    /* Textures */
    enemy->ship = shipTex;
    if(shipType != asteroid)
        enemy->shipExhaust = shipExhaustTex;

    /* Given stats */
    enemy->pos = pos;
    enemy->rot = rot;
    enemy->velocity = velocity;
    enemy->persistent = persistent;

    /* Type */
    enemy->shipType = shipType;

    switch(shipType) {
        case asteroid:
            enemy->health = 400;
            enemy->hitboxSize = 64;
            break;

        case basic:

            enemy->accelVal = 195;
            enemy->angAccelVal = 150;
            enemy->rcsAccelVal = 100;

            enemy->hitboxSize = 64;
            
            enemy->gimbal = 15;
            
            enemy->shootDelay = 0.2f;
            enemy->bulletDamage = 50;

            enemy->health = 250;

            break;
    }
}