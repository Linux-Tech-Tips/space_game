#include "bullet.h"

/* Update + render functions */

void bullet_update(bullet_t * bData) {

    bData->pos = Vector2Add(bData->pos, Vector2Scale(bData->velocity, GetFrameTime()));

}

void bullet_render(bullet_t bData) {

    int bulW = bData.bulletTex->width;
    int bulH = bData.bulletTex->height;

    DrawTexturePro(
        *bData.bulletTex,
        (Rectangle){0, 0, bulW, bulH}, 
        (Rectangle){bData.pos.x, bData.pos.y, bulW, bulH},
        (Vector2){bulW/2.0f, bulH/2.0f},
        bData.rot,
        WHITE);

}


/* Other functions */

void bullet_create(bullet_t * bData, Texture2D * tex, Vector2 pos, float rot, Vector2 velocity) {
    
    bData->pos = pos;
    bData->rot = rot;
    bData->velocity = velocity;
    
    bData->bulletTex = tex;
}


/* Bullet list functions */

void bullet_list_remove(bullet_t bList [], int index, int * lSize) {
    for(int i = index; i < *lSize-1; i++) {
        bList[i] = bList[i+1];
    }
    *lSize -= 1;
}

void bullet_list_add(bullet_t bList [], bullet_t bullet, int * lSize, int maxSize) {
    if(*lSize >= maxSize) {
        bullet_list_remove(bList, 0, lSize);
    }
    bList[*lSize] = bullet;
    *lSize += 1;
}