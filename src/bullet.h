/**
 * @file bullet.h
 * @author Linux-Tech-Tips
 * @brief Bullet data and functions
 *
 * Contains the @ref bullet_t data structure and its functions.
 * The functions provide update logic and draw calls for any singular bullet instance.
 * Functions prefixed with bullet_list_ contain logic tied to a C array of bullet instances.
 * Interaction logic between bullets and other structures is contained within @ref game.h
 */

#ifndef BULLET_H
#define BULLET_H

#include <raylib.h>
#include <raymath.h>

/** Bullet data structure containing information relevant to bullets */
typedef struct {

    /** The current position of the bullet */
    Vector2 pos;
    /** The current rotation of the bullet */
    float rot;

    /** Whether the bullet was shot by an enemy */
    short isEnemy;

    /** The current velocity of the bullet */
    Vector2 velocity;

    /** The pointer to a texture which the bullet should use, is to be loaded elsewhere and shared between bullet instances */
    Texture2D * bulletTex;

    /** The size of the circular hitbox of the bullet */
    float hitboxSize;

    /* The damage the bullet applies on hit */
    float damage;

} bullet_t;


/* Update + render functions */

/** The base bullet update logic */
void bullet_update(bullet_t * bData);

/** The base bullet draw calls */
void bullet_render(bullet_t bData);


/* Other functions */

/** Fills an empty bullet data structure with the given parameters */
void bullet_create(bullet_t * bData, Texture2D * tex, Vector2 pos, float rot, short isEnemy, float damage, Vector2 velocity);

/* Bullet list functions */

/** Removes a bullet from a list at the given index, shrinks the passed list size variable */
void bullet_list_remove(bullet_t bList [], int index, int * lSize);

/** Adds a bullet to the end of a list, increases the passed list size, removes the first entry if the list is too big */
void bullet_list_add(bullet_t bList [], bullet_t bullet, int * lSize, int maxSize);

#endif /* BULLET_H */