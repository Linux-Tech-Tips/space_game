#ifndef SCREEN_H
#define SCREEN_H

#include <raylib.h>
#include <raymath.h>

#include "game.h"

/* Structures and typedefs */

typedef enum {
    title, settings, game
} screen_id_t;

/** 
 * GUI Interaction type, serves as a container to transmit GUI data between render (which creates GUI) and update (which executes GUI functionality)
*/
typedef struct {
    
    short title_button1, title_button2, title_button3;

    short game_button1;

} screen_guiData_t;

/* Update functions */

void screen_update(screen_id_t * id, short * runWindow, short * loaded, screen_guiData_t * guiData, game_data_t * gameData);

void _screen_update_title(screen_id_t * id, short * runWindow, short * loaded, screen_guiData_t * guiData);

void _screen_update_game(screen_id_t * id, short * runWindow, short * loaded, screen_guiData_t * guiData, game_data_t * gameData);

/* Render functions */

void screen_render(screen_id_t id, int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData);

void _screen_render_title(int scrX, int scrY, screen_guiData_t * guiData);

void _screen_render_game(int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData);

/* Load/unload functions */

short _screen_load_title(screen_guiData_t * guiData);

short _screen_load_game(screen_guiData_t * guiData, game_data_t * gameData);

short _screen_unload_game(game_data_t * gameData);

/* Utility functions */

int screen_textCenter_o(const char * text, int textSize, int scrX, int offset);

int screen_textCenter(const char * text, int textSize, int scrX);

int screen_elCenter_o(int elSize, int scrSize, int offset);

int screen_elCenter(int elSize, int scrSize);

#endif