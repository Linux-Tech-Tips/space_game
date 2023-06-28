#include "game.h"

/* Update functions */

void game_update(game_data_t * gameData) {

    /* Player update */
    player_update(&gameData->playerData);
    /* Game over */
    if(gameData->playerData.health <= 0 && !gameData->over)
        gameData->over = 1;

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

    /* Player bullet creation */
    if(gameData->playerData.shoot) {
        bullet_t newBullet = {0};
        Vector2 posOffset = Vector2Scale(util_dirVector(gameData->playerData.rot), 48);
        Vector2 bPos = Vector2Add(gameData->playerData.pos, posOffset);
        Vector2 bVelocity = Vector2Scale(util_dirVector(gameData->playerData.rot), 2000);
        bullet_create(&newBullet, &gameData->textures.playerBullet, bPos, gameData->playerData.rot, gameData->playerData.bulletDamage, Vector2Add(gameData->playerData.velocity, bVelocity));
        bullet_list_add(gameData->bullets, newBullet, &gameData->bulletCount, BULLET_AMOUNT);
    }

    /* Bullet update */
    _game_update_bulletList(gameData);

    /* Enemy update */
    float closestAst = 0;
    _game_update_enemyList(gameData, &closestAst);

    /* Spawning asteroids */
    if(closestAst > ASTEROID_SPAWN_DIST || (closestAst == 0 && gameData->enemyCount == 0)) {
        Vector2 fieldPos = Vector2Rotate(Vector2Scale(Vector2Normalize(gameData->playerData.velocity), 4500), util_toRad(GetRandomValue(-5, 5)));
        game_spawnAsteroidField(gameData, Vector2Add(gameData->playerData.pos, fieldPos), 3000, 15);
    }

}

/* Bullet list update aux function */
void _game_update_bulletList(game_data_t * gameData) {
    for(int i = 0; i < gameData->bulletCount; i++) {
        /* Standard bullet update */
        bullet_t * cBullet = &gameData->bullets[i];
        bullet_update(cBullet);

        /* Removing bullets which have flown too far away from the player */
        if(Vector2Distance(cBullet->pos, gameData->playerData.pos) > BULLET_DESPAWN_DIST) {
            bullet_list_remove(gameData->bullets, i, &gameData->bulletCount);
            continue;
        }

        /* Collision testing */
        for(int j = 0; j < gameData->enemyCount; j++) {
            enemy_t * cEnemy = &gameData->enemies[j];
            if(util_circleCollision(cBullet->pos, cBullet->hitboxSize, cEnemy->pos, cEnemy->hitboxSize)) {
                cEnemy->health -= cBullet->damage;
                bullet_list_remove(gameData->bullets, i, &gameData->bulletCount);
                break;
            }
        }
    }
}

/* Enemy list update aux function */
void _game_update_enemyList(game_data_t * gameData, float * closestAsteroid) {
    for(int i = 0; i < gameData->enemyCount; i++) {
        /* Standard enemy update */
        enemy_t * cEnemy = &gameData->enemies[i];
        enemy_update(cEnemy);

        /* Removing enemies which are too far away from the player */
        if(Vector2Distance(cEnemy->pos, gameData->playerData.pos) > ENEMY_DESPAWN_DIST) {
            enemy_list_remove(gameData->enemies, i, &gameData->enemyCount);
            continue;
        }

        /* Removing enemies with 0 health */
        if(cEnemy->health <= 0) {
            enemy_list_remove(gameData->enemies, i, &gameData->enemyCount);
            continue;
        }

        /* Testing distance of asteroids */
        if(cEnemy->shipType == asteroid) {
            float dist = Vector2Distance(cEnemy->pos, gameData->playerData.pos);
            if(dist < *closestAsteroid)
                *closestAsteroid = dist;
        }
        
        /* Player collision */
        if(util_circleCollision(gameData->playerData.pos, gameData->playerData.hitboxSize, cEnemy->pos, cEnemy->hitboxSize)) {
            /* TODO Bounce + damage accordingly */
            Vector2 reflectNormal = Vector2Normalize(Vector2Subtract(gameData->playerData.pos, cEnemy->pos));
            gameData->playerData.velocity = Vector2Reflect(gameData->playerData.velocity, reflectNormal);
            cEnemy->velocity = Vector2Reflect(cEnemy->velocity, reflectNormal);

            float damage = Vector2Length(gameData->playerData.velocity) + Vector2Length(cEnemy->velocity);
            cEnemy->health -= damage;
            gameData->playerData.health -= damage;
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

        /* Drawing enemies under the player */
        for(int i = 0; i < gameData->enemyCount; i++)
            enemy_render(gameData->enemies[i]);

        player_render(gameData->playerData, (gameData->paused || gameData->over));

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

void game_resetStructure(game_data_t * gameData) {
    
    /* Initializing default values */
    gameData->paused = 0;
    gameData->over = 0;
    
    /* Player */
    player_resetData(&gameData->playerData);

    /* Bullets */
    gameData->bulletCount = 0;

    /* Backgrounds */
    for(int i = 0; i < 3; i++) {
        gameData->bgOffsetX[i] = 0;
        gameData->bgOffsetY[i] = 0;
    }

    /* Camera */
    gameData->camera.zoom = 1.0f;

    /* Enemies */
    gameData->enemyCount = 0;
}

void game_loadTex(game_textures_t * texData) {
    texData->background[0] = LoadTexture("res/bg_far.png");
    texData->background[1] = LoadTexture("res/bg_mid.png");
    texData->background[2] = LoadTexture("res/bg_near.png");

    texData->playerShip = LoadTexture("res/player.png");
    texData->playerExhaust = LoadTexture("res/player_exhaust.png");

    texData->playerBullet = LoadTexture("res/player_bullet.png");

    texData->asteroid = LoadTexture("res/asteroid.png");
}

void game_unloadTex(game_textures_t * texData) {
    UnloadTexture(texData->background[0]);
    UnloadTexture(texData->background[1]);
    UnloadTexture(texData->background[2]);

    UnloadTexture(texData->playerShip);
    UnloadTexture(texData->playerExhaust);
    UnloadTexture(texData->playerBullet);

    UnloadTexture(texData->asteroid);
}


/* Spawn functions */

void game_spawnAsteroidField(game_data_t * gameData, Vector2 pos, float radius, int asteroidCount) {
    for(int i = 0; i < asteroidCount; i++) {
        /* Angle and distance from center */
        int angle = GetRandomValue(0, 360);
        float dist = GetRandomValue(0, (int)(radius * 100)) / 100.0f;

        Vector2 finalPos = Vector2Add(pos, Vector2Scale(Vector2Rotate((Vector2){1, 0}, util_toRad(angle)), dist));

        if(Vector2Distance(finalPos, gameData->playerData.pos) < 128) {
            finalPos = Vector2Add(finalPos, Vector2Scale(Vector2Normalize(util_dirVector(gameData->playerData.rot)), 256));
        }

        enemy_t newEnemy = {0};
        enemy_initData(&newEnemy, finalPos, 0, Vector2Zero(), 0, asteroid, &gameData->textures.asteroid, 0);
        enemy_list_add(gameData->enemies, newEnemy, &gameData->enemyCount, ENEMY_AMOUNT);
    }
}