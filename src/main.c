#include <raylib.h>

#include "screen.h"
#include "game.h"

/** 
 * Main application data structure, contains data related to the app itself and not gameplay
*/
typedef struct {
    /** Set to false anywhere to close the window */
    short runWindow;
    /** The current screen (screen_id_t enum) to update and draw */
    screen_id_t screenId;
    /** Is set to true once loading of the current screen is completed */
    short screenLoaded;

    /** Viewport dimensions */
    int scrX, scrY;

} main_appData_t;

/** Main application update function, all further updating within */
void main_update(main_appData_t * appData, screen_guiData_t * guiData, game_data_t * gameData) {
    
    /* Closing window if requested */
    appData->runWindow = !WindowShouldClose();

    appData->scrX = GetRenderWidth();
    appData->scrY = GetRenderHeight();

    screen_update(&appData->screenId, &appData->runWindow, &appData->screenLoaded, appData->scrX, appData->scrY, guiData, gameData);
}

/** Main application render function, all further drawing within */
void main_render(main_appData_t appData, screen_guiData_t * guiData, game_data_t * gameData) {

    /* All drawing should be enclosed in Raylib BeginDrawing() and EndDrawing() calls */
    BeginDrawing();

        ClearBackground((Color){10, 10, 10, 255});

        screen_render(appData.screenId, appData.screenLoaded, appData.scrX, appData.scrY, guiData, gameData);

    EndDrawing();
}


int main() {

    /* Raylib initialization */
    InitWindow(1600, 900, "Space test");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(0);
    SetTargetFPS(120);
    
    /* Seed for random generation */
    SetRandomSeed((int)GetTime());

    /* All important data structure instances */
    main_appData_t appData = { .runWindow = 1, .screenId = title };
    screen_guiData_t guiData = {0};
    game_data_t gameData = {0};
    game_initStructure(&gameData);

    /* Main loop */
    while(appData.runWindow) {
        main_update(&appData, &guiData, &gameData);
        main_render(appData, &guiData, &gameData);
    }

    /* Raylib exit */
    CloseWindow();

}
