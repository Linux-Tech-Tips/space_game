#include "screen.h"

/* Should be included here to prevent linker errors - only in the .c file in which Raygui is used */
#define RAYGUI_IMPLEMENTATION
#ifdef RAYGUI_H_LOC
#include RAYGUI_H_LOC /* Custom Raygui path if passed during compilation */
#else
#include "../../raygui/src/raygui.h"
#endif


/* Update functions */

void screen_update(screen_id_t * id, short * runWindow, short * loaded, int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData) {
    switch(*id) {
        case title:
            if(!*loaded)
                *loaded = _screen_load_title(guiData);
            _screen_update_title(id, runWindow, loaded, guiData);
            break;
        case game:
            if(!*loaded)
                *loaded = _screen_load_game(guiData, gameData);
            _screen_update_game(id, runWindow, loaded, scrX, scrY, guiData, gameData);
            break;
    }
}

void _screen_update_title(screen_id_t * id, short * runWindow, short * loaded, screen_guiData_t * guiData) {
    /* Exiting the app if desired */
    if(IsKeyPressed(KEY_ESCAPE) || guiData->title_buttonExit) {
        *runWindow = 0;
    }

    /* Switching to game screen if desired */
    if(guiData->title_buttonPlay) {
        *id = game;
        *loaded = 0; /* Make sure loaded set to false on switch to load the next screen */
    }
}

void _screen_update_game(screen_id_t * id, short * runWindow, short * loaded, int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData) {
    /* Exiting the game screen if desired */
    if(guiData->game_buttonBack) {
        *id = title;
        *loaded = 0;
        _screen_unload_game(gameData);
    }

    /* Pause menu switching logic */
    if(IsKeyPressed(KEY_ESCAPE))
        gameData->paused = !gameData->paused;

    if(guiData->game_buttonContinue) {
        gameData->paused = 0;
        guiData->game_buttonContinue = 0; /* Make sure to reset the button once pressed */
    }

    /* Camera update according to game */
    gameData->camera.target.x = gameData->playerData.pos.x;
    gameData->camera.target.y = gameData->playerData.pos.y;
    gameData->camera.offset.x = scrX/2.0f;
    gameData->camera.offset.y = scrY/2.0f;
    //gameData->camera.rotation = -90.0f - gameData->rot;

    /* Camera zoom controls */
    if(IsKeyDown(KEY_UP) && (gameData->camera.zoom < 2)) {
        gameData->camera.zoom += 0.5f * GetFrameTime();
    }
    if(IsKeyDown(KEY_DOWN) && (gameData->camera.zoom > 0.5f)) {
        gameData->camera.zoom -= 0.5f * GetFrameTime();
    }

    if(!gameData->paused)
        game_update(gameData);
}


/* Render functions */

void screen_render(screen_id_t id, short loaded, int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData) {
    if(loaded) {
        switch(id) {
            case title:
                _screen_render_title(scrX, scrY, guiData);
                break;
            case game:
                _screen_render_game(scrX, scrY, guiData, gameData);
                break;
        }
    }
}

void _screen_render_title(int scrX, int scrY, screen_guiData_t * guiData) {

    /* Title text */
    DrawText("Space Game Demo Title Screen", screen_textCenter("Space Game Demo Title Screen", 50, scrX), 15, 50, WHITE);

    guiData->title_buttonPlay = GuiButton((Rectangle){screen_elCenter(160, scrX), 80, 160, 50}, "Play demo");
    
    /* Settings disabled in this version of the demo */
    GuiDisable();
    guiData->title_buttonSettings = GuiButton((Rectangle){screen_elCenter(160, scrX), 160, 160, 50}, "Settings (WIP)");
    GuiEnable();
    
    guiData->title_buttonExit = GuiButton((Rectangle){screen_elCenter(160, scrX), 240, 160, 50}, "Exit");

}

void _screen_render_game(int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData) {

    game_render(gameData);

    /* Demo reminder up top */
    DrawText("INCOMPLETE (DEMONSTRATION) VERSION", screen_textCenter("INCOMPLETE (DEMONSTRATION) VERSION", 40, scrX), 15, 40, WHITE);

    /* Controls text */
    DrawText("Controls:", 10, scrY-160, 20, WHITE);
    DrawText("W/S - Accelerate forwards/backwards", 10, scrY-140, 20, WHITE);
    DrawText("A/D - Turn ship around", 10, scrY-120, 20, WHITE);
    DrawText("Q/E - Accelerate sideways", 10, scrY-100, 20, WHITE);
    DrawText("Arrow UP/DOWN - zoom in/out", 10, scrY-80, 20, WHITE);
    DrawText("Arrow LEFT/RIGHT - Throttle down/up", 10, scrY-60, 20, WHITE);
    DrawText("SHIFT - Kill velocity and angular momentum", 10, scrY-40, 20, WHITE);
    DrawText("R - Kill only angular momentum", 10, scrY-20, 20, WHITE);

    /* Velocity circle display */
    DrawText("Velocity direction:", scrX-(MeasureText("Velocity direction:", 20)+10), scrY-185, 20, WHITE);
    DrawCircle(scrX-80, scrY-120, 40, LIGHTGRAY);
    Vector2 startPoint = {scrX-80, scrY-120};
    Vector2 endPoint = Vector2Add(startPoint, Vector2ClampValue(Vector2Scale(gameData->playerData.velocity, 0.15f), 0.0f, 40.0f));
    DrawLineEx(startPoint, endPoint, 8, DARKBLUE);
    /* Velocity text display */
    DrawText(TextFormat("Current velocity: %.2f", Vector2Length(gameData->playerData.velocity)), scrX - (MeasureText("Current velocity: 00000", 20)+10), scrY-60, 20, WHITE);
    DrawText(TextFormat("Current angular momentum: %.2f", gameData->playerData.angVelocity), scrX-(MeasureText("Current angular momentum: 00000", 20)+10), scrY-40, 20, WHITE);
    DrawText(TextFormat("Current throttle: %i%%", gameData->playerData.throttle), scrX-(MeasureText("Current throttle: 100%%", 20)+10), scrY-20, 20, WHITE);

    /* Pause menu */
    if(gameData->paused) {
        DrawRectangle(0, 0, scrX, scrY, (Color){10, 10, 10, 100});
        DrawText("GAME PAUSED", screen_textCenter("GAME PAUSED", 40, scrX), 100, 40, RAYWHITE);

        guiData->game_buttonContinue = GuiButton((Rectangle){screen_elCenter(100, scrX), 200, 100, 40}, "Continue");
        guiData->game_buttonBack = GuiButton((Rectangle){screen_elCenter(100, scrX), 300, 100, 40}, "Back");
    }


}


/* Load/unload functions */

short _screen_load_title(screen_guiData_t * guiData) {

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    /* Resetting GUI data */
    guiData->title_buttonPlay = 0;
    guiData->title_buttonSettings = 0;
    guiData->title_buttonExit = 0;

    /* Returning true after loaded successfully */
    return 1;
}

short _screen_load_game(screen_guiData_t * guiData, game_data_t * gameData) {

    GuiSetStyle(DEFAULT, TEXT_SIZE, 15);

    /* Resetting GUI data */
    guiData->game_buttonContinue = 0;
    guiData->game_buttonBack = 0;

    /* Resetting pause state */
    gameData->paused = 0;

    /* Loading textures necessary for the game */
    game_loadTex(&gameData->textures);

    /* Returning true after loaded successfully */
    return 1;
}

short _screen_unload_game(game_data_t * gameData) {
    /* Unloading textures from game after use */
    game_unloadTex(&gameData->textures);
}


/* Utility functions */

int screen_textCenter_o(const char * text, int textSize, int scrX, int offset) {
    return (scrX/2 - MeasureText(text, textSize)/2 + offset);
}

int screen_textCenter(const char * text, int textSize, int scrX) {
    return screen_textCenter_o(text, textSize, scrX, 0);
}

int screen_elCenter_o(int elSize, int scrSize, int offset) {
    return (scrSize/2 - elSize/2 + offset);
}

int screen_elCenter(int elSize, int scrSize) {
    return screen_elCenter_o(elSize, scrSize, 0);
}