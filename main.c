#include <raylib.h>

#include "screen.h"
#include "game.h"

/* 
    TODO
        main.c -> general application data + general application update/render
        screen.c/screen.h -> functions to update and render specific screens, perhaps if the functions got too long, could be split into a file for each screen
        game.c/game.h -> general game data, anything that's needed for the gameplay specifically
        player.c/player.h -> player data structure, update/render + make sure to think through and include ship customization and damage
        enemy.c/enemy.h -> enemy data structure, update/render, different enemies, varying stats based on structure data
    
    - WIP Version 2306_alpha14
    - Anything here is subject to change
*/

/** 
 * Main application data structure, contains data related to the app itself and not gameplay
*/
typedef struct {

    short runWindow;
    screen_id_t screenId;
    short screenLoaded;

    int scrX, scrY;

} main_appData_t;

/**
 * Main application update function
 */
void main_update(main_appData_t * appData, screen_guiData_t * guiData, game_data_t * gameData) {
    
    /* Closing window if requested */
    appData->runWindow = !WindowShouldClose();

    /* Getting render size */
    appData->scrX = GetRenderWidth();
    appData->scrY = GetRenderHeight();

    /* Updating screen based on the current screenId */
    screen_update(&appData->screenId, &appData->runWindow, &appData->screenLoaded, appData->scrX, appData->scrY, guiData, gameData);
}

/** 
 * Main application render function
*/
void main_render(main_appData_t appData, screen_guiData_t * guiData, game_data_t * gameData) {
    
    BeginDrawing();

        ClearBackground((Color){10, 10, 10, 255});

        screen_render(appData.screenId, appData.screenLoaded, appData.scrX, appData.scrY, guiData, gameData);

    EndDrawing();
}


int main() {

    /* Raylib init */
    InitWindow(1600, 900, "Space test");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(0);
    SetTargetFPS(120);

    main_appData_t appData = { .runWindow = 1, .screenId = title };

    screen_guiData_t guiData = {0};

    //game_data_t gameData = { .paused = 0, .posX = 100, .posY = 100, .rot = 0, .accel = 200, .angAccel = 30, .rcsAccel = 60, .throttle = 100 };
    game_data_t gameData = {0};
    game_initStructure(&gameData);

    while(appData.runWindow) {
        main_update(&appData, &guiData, &gameData);
        main_render(appData, &guiData, &gameData);
    }

    /* Raylib exit */
    CloseWindow();

}
