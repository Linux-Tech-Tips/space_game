#ifndef SCREEN_H
#define SCREEN_H

#include <raylib.h>
#include <raymath.h>

#include "game.h"
#include "util.h"

/* Structures and typedefs */

/** The screen ID type containing all recognized screens (with update/render) */
typedef enum {
    title, settings, game
} screen_id_t;

/** GUI Interaction type, serves as a container to transmit GUI data between render (which creates GUI) and update (which executes GUI functionality) */
typedef struct {
    /** Title screen buttons */    
    short title_buttonPlay, title_buttonSettings, title_buttonExit;
    /** Game screen buttons */
    short game_buttonContinue, game_buttonBack, game_buttonNew;
} screen_guiData_t;


/* Update functions */

/** Base screen update, updates whichever specific screen the game is in */
void screen_update(screen_id_t * id, short * runWindow, short * loaded, int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData);

/** Internal function, the separate update logic of the title screen */
void _screen_update_title(screen_id_t * id, short * runWindow, short * loaded, screen_guiData_t * guiData);

/** Internal function, the separate update logic of the game screen */
void _screen_update_game(screen_id_t * id, short * runWindow, short * loaded, int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData);


/* Render functions */

/** Base screen render, draws whichever specific screen the game is in */
void screen_render(screen_id_t id, short loaded, int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData);

/** Internal function, the separate render code of the title screen */
void _screen_render_title(int scrX, int scrY, screen_guiData_t * guiData);

/** Internal function, the separate render code of the game screen */
void _screen_render_game(int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData);


/* Load/unload functions */

/** Internal function, loads anything necessary for the title screen */
short _screen_load_title(screen_guiData_t * guiData);

/** Internal function, loads anything necessary for the game screen */
short _screen_load_game(screen_guiData_t * guiData, game_data_t * gameData);

/** Internal function, unloads anything used for the game screen */
short _screen_unload_game(game_data_t * gameData);


/* Utility functions */

/** Returns the x position of centered text with the given arguments, including offset */
int screen_textCenter_o(const char * text, int textSize, int scrX, int offset);

/** Returns the x position of centered text with the given arguments */
int screen_textCenter(const char * text, int textSize, int scrX);

/** 
 * Returns the x/y position of the given element centered along the desired axis, with offset
 * @param elSize the size of the element along the desired axis on which to center
 * @param scrSize the viewport dimensions of the desired axis
 */
int screen_elCenter_o(int elSize, int scrSize, int offset);

/** The x/y position of the given element centered along the desired axis */
int screen_elCenter(int elSize, int scrSize);

#endif /* SCREEN_H */