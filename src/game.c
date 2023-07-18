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
        bullet_create(&newBullet, &gameData->textures.playerBullet, bPos, gameData->playerData.rot, 0, gameData->playerData.bulletDamage, Vector2Add(gameData->playerData.velocity, bVelocity));
        bullet_list_add(gameData->bullets, newBullet, &gameData->bulletCount, BULLET_MAX_AMOUNT);
    }

    /* Bullet update */
    _game_update_bulletList(gameData);

    /* Enemy update */
    int asteroidCount = 0, basicCount = 0;
    _game_update_enemyList(gameData, &asteroidCount, &basicCount);

    game_regenAsteroids(gameData, ASTEROID_FIELD_X, ASTEROID_FIELD_Y, ASTEROID_FIELD_AMOUNT, asteroidCount);

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

        /* Collision testing (player if enemy bullet, enemy if not) */
        if(cBullet->isEnemy) {
            if(util_circleCollision(cBullet->pos, cBullet->hitboxSize, gameData->playerData.pos, gameData->playerData.hitboxSize)) {
                gameData->playerData.health -= cBullet->damage;
                bullet_list_remove(gameData->bullets, i, &gameData->bulletCount);
            }
        } else {
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
}

/* Enemy list update aux function */
void _game_update_enemyList(game_data_t * gameData, int * asteroidCount, int * basicCount) {
    for(int i = 0; i < gameData->enemyCount; i++) {
        /* Standard enemy update */
        enemy_t * cEnemy = &gameData->enemies[i];
        enemy_update(cEnemy, gameData->playerData.pos, gameData->playerData.rot);

        /* Removing enemies which are too far away from the player */
        if(!(cEnemy->persistent) && Vector2Distance(cEnemy->pos, gameData->playerData.pos) > ENEMY_DESPAWN_DIST) {
            enemy_list_remove(gameData->enemies, i, &gameData->enemyCount);
            continue;
        }

        /* Removing enemies with 0 health */
        if(cEnemy->health <= 0) {
            enemy_list_remove(gameData->enemies, i, &gameData->enemyCount);
            continue;
        }
        
        /* Asteroid-specific game update */
        if(cEnemy->shipType == asteroid) {
            (*asteroidCount)++;
            /* Moving to next field unit if far away enough */
            float xDiff = gameData->playerData.pos.x - cEnemy->pos.x;
            if(abs(xDiff) > ASTEROID_FIELD_X / 2) {
                cEnemy->pos.x += (ASTEROID_FIELD_X + GetRandomValue(-ASTEROID_MOVE_OFFSET, ASTEROID_MOVE_OFFSET)) * util_sign(xDiff);
            }
            float yDiff = gameData->playerData.pos.y - cEnemy->pos.y;
            if(abs(yDiff) > ASTEROID_FIELD_Y / 2) {
                cEnemy->pos.y += (ASTEROID_FIELD_Y + GetRandomValue(-ASTEROID_MOVE_OFFSET, ASTEROID_MOVE_OFFSET)) * util_sign(yDiff);
            }
        }
        
        /* Basic-specific game update */
        if(cEnemy->shipType == basic) {
            (*basicCount)++;

            /* Creating bullet if requested */
            if(cEnemy->shoot) {
                bullet_t newBullet = {0};
                /* New bullet position, with offset to spawn at the enemy's front */
                Vector2 bPos = Vector2Add(cEnemy->pos, Vector2Scale(util_dirVector(cEnemy->rot), 48));
                Vector2 bVelocity = Vector2Scale(util_dirVector(cEnemy->rot), 2000);
                bullet_create(&newBullet, &gameData->textures.enemyBullet, bPos, cEnemy->rot, 1, cEnemy->bulletDamage, Vector2Add(cEnemy->velocity, bVelocity));
                bullet_list_add(gameData->bullets, newBullet, &gameData->bulletCount, BULLET_MAX_AMOUNT);
            }

        }

        /* Player collision */
        if(util_circleCollision(gameData->playerData.pos, gameData->playerData.hitboxSize, cEnemy->pos, cEnemy->hitboxSize)) {
            /* Bounce colliding velocities */
            Vector2 reflectNormal = Vector2Normalize(Vector2Subtract(gameData->playerData.pos, cEnemy->pos));
            gameData->playerData.velocity = Vector2Reflect(gameData->playerData.velocity, reflectNormal);
            cEnemy->velocity = Vector2Reflect(cEnemy->velocity, reflectNormal);
            /* Damage colliding objects */
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

    /* Enemies */
    gameData->enemyCount = 0;
    game_genAsteroids(gameData, ASTEROID_FIELD_X, ASTEROID_FIELD_Y, ASTEROID_FIELD_AMOUNT);

    enemy_t testEnemy = {};
    enemy_initData(&testEnemy, (Vector2){-400, -400}, 0, Vector2Zero(), 1, basic, &gameData->textures.enemyShip, &gameData->textures.enemyExhaust);
    enemy_list_add(gameData->enemies, testEnemy, &gameData->enemyCount, ENEMY_MAX_AMOUNT);
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
    game_genAsteroids(gameData, ASTEROID_FIELD_X, ASTEROID_FIELD_Y, ASTEROID_FIELD_AMOUNT);

    enemy_t testEnemy = {};
    enemy_initData(&testEnemy, (Vector2){-400, -400}, 0, Vector2Zero(), 1, basic, &gameData->textures.enemyShip, &gameData->textures.enemyExhaust);
    enemy_list_add(gameData->enemies, testEnemy, &gameData->enemyCount, ENEMY_MAX_AMOUNT);
}

void game_loadTex(game_textures_t * texData) {
    texData->background[0] = LoadTexture("res/bg_far.png");
    texData->background[1] = LoadTexture("res/bg_mid.png");
    texData->background[2] = LoadTexture("res/bg_near.png");

    texData->playerShip = LoadTexture("res/player.png");
    texData->playerExhaust = LoadTexture("res/player_exhaust.png");

    texData->playerBullet = LoadTexture("res/player_bullet.png");

    texData->asteroid = LoadTexture("res/asteroid.png");

    texData->enemyShip = LoadTexture("res/enemy.png");
    texData->enemyExhaust = LoadTexture("res/enemy_exhaust.png");
    texData->enemyBullet = LoadTexture("res/enemy_bullet.png");
}

void game_unloadTex(game_textures_t * texData) {
    UnloadTexture(texData->background[0]);
    UnloadTexture(texData->background[1]);
    UnloadTexture(texData->background[2]);

    UnloadTexture(texData->playerShip);
    UnloadTexture(texData->playerExhaust);
    UnloadTexture(texData->playerBullet);

    UnloadTexture(texData->asteroid);

    UnloadTexture(texData->enemyShip);
    UnloadTexture(texData->enemyExhaust);
    UnloadTexture(texData->enemyBullet);
}


/* Generation functions */

void game_genAsteroids(game_data_t * gameData, int fieldSizeX, int fieldSizeY, int amount) {
    for(int i = 0; i < amount; i++) {
        Vector2 pos = (Vector2){GetRandomValue(-fieldSizeX/2, fieldSizeX/2), GetRandomValue(-fieldSizeY/2, fieldSizeY/2)};

        if(Vector2Distance(gameData->playerData.pos, pos) < 128) {
            pos = Vector2Add(pos, Vector2Rotate((Vector2){128, 0}, GetRandomValue(0, 2*PI)));
        }

        enemy_t newEnemy = {};
        enemy_initData(&newEnemy, pos, GetRandomValue(0, 360), Vector2Zero(), 1, asteroid, &gameData->textures.asteroid, 0);
        enemy_list_add(gameData->enemies, newEnemy, &gameData->enemyCount, ENEMY_MAX_AMOUNT);
    }
}

void game_regenAsteroids(game_data_t * gameData, int fieldSizeX, int fieldSizeY, int maxAmount, int cAmount) {
    /* Checking if there is space for another asteroid */
    int diff = maxAmount - cAmount;
    if(diff >= ASTEROID_REGEN_TRIGGER) {
        /* Generating a random amount of asteroids to fit */
        int toRegen = GetRandomValue(1, diff);
        for(int i = 0; i < toRegen; i++) {
            /* Here, arbitrary default screen size is used as offset, could be changed for more control later */
            Vector2 pos = (Vector2){gameData->playerData.pos.x + GetRandomValue(1600, 1600 + fieldSizeX), gameData->playerData.pos.y + GetRandomValue(900, 900 + fieldSizeY)};

            enemy_t newEnemy = {};
            enemy_initData(&newEnemy, pos, GetRandomValue(0, 360), Vector2Zero(), 1, asteroid, &gameData->textures.asteroid, 0);
            enemy_list_add(gameData->enemies, newEnemy, &gameData->enemyCount, ENEMY_MAX_AMOUNT);
        }
    }
}