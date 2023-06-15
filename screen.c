#include "screen.h"

/* Should be included here to prevent linker errors - only in the .c file in which raygui is used */
#define RAYGUI_IMPLEMENTATION
#include "../raygui/src/raygui.h"

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
    if(IsKeyPressed(KEY_ESCAPE) || guiData->title_button3) {
        *runWindow = 0;
    }

    if(guiData->title_button1) {
        *id = game;
        *loaded = 0;
    }
}

void _screen_update_game(screen_id_t * id, short * runWindow, short * loaded, int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData) {
    if(guiData->game_button2) {
        *id = title;
        *loaded = 0;
        _screen_unload_game(gameData);
    }

    if(IsKeyPressed(KEY_ESCAPE))
        gameData->paused = !gameData->paused;
    
    if(guiData->game_button1) {
        gameData->paused = 0;
        guiData->game_button1 = 0;
    }

    /* Camera update */
    gameData->camera.target.x = gameData->posX;
    gameData->camera.target.y = gameData->posY;
    gameData->camera.offset.x = scrX/2.0f;
    gameData->camera.offset.y = scrY/2.0f;
    gameData->camera.rotation = -90.0f - gameData->rot;

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

    DrawText("Space Game Demo Title Screen", screen_textCenter("Space Game Demo Title Screen", 50, scrX), 15, 50, WHITE);

    guiData->title_button1 = GuiButton((Rectangle){screen_elCenter(160, scrX), 80, 160, 50}, "Play demo");
    
    GuiDisable();
    guiData->title_button2 = GuiButton((Rectangle){screen_elCenter(160, scrX), 160, 160, 50}, "Settings (WIP)");
    GuiEnable();
    
    guiData->title_button3 = GuiButton((Rectangle){screen_elCenter(160, scrX), 240, 160, 50}, "Exit");

}

void _screen_render_game(int scrX, int scrY, screen_guiData_t * guiData, game_data_t * gameData) {

    game_render(gameData, scrX, scrY);

    DrawText("INCOMPLETE (DEMONSTRATION) VERSION", screen_textCenter("INCOMPLETE (DEMONSTRATION) VERSION", 40, scrX), 15, 40, WHITE);

    DrawText("Controls:", 10, scrY-120, 20, WHITE);
    DrawText("W/S - Accelerate forwards/backwards", 10, scrY-100, 20, WHITE);
    DrawText("A/D - Turn ship around", 10, scrY-80, 20, WHITE);
    DrawText("Q/E - Accelerate sideways", 10, scrY-60, 20, WHITE);
    DrawText("SHIFT - Kill velocity and angular momentum", 10, scrY-40, 20, WHITE);
    DrawText("R - Kill only angular momentum", 10, scrY-20, 20, WHITE);

    DrawText("Velocity direction:", scrX-(MeasureText("Velocity direction:", 20)+10), scrY-185, 20, WHITE);
    DrawCircle(scrX-80, scrY-120, 40, LIGHTGRAY);
    Vector2 startPoint = {scrX-80, scrY-120};
    Vector2 endPoint = Vector2Add(startPoint, Vector2ClampValue(Vector2Scale(gameData->velocity, 0.15f), 0.0f, 40.0f));
    DrawLineEx(startPoint, endPoint, 8, DARKBLUE);
    
    DrawText(TextFormat("Current velocity: %.2f", Vector2Length(gameData->velocity)), scrX - (MeasureText("Current velocity: 00000", 20)+10), scrY-60, 20, WHITE);
    DrawText(TextFormat("Current angular momentum: %.2f", gameData->angMoment), scrX-(MeasureText("Current angular momentum: 00000", 20)+10), scrY-40, 20, WHITE);
    DrawText(TextFormat("Current throttle: %i%%", gameData->throttle), scrX-(MeasureText("Current throttle: 100%%", 20)+10), scrY-20, 20, WHITE);

    if(gameData->paused) {
        DrawRectangle(0, 0, scrX, scrY, (Color){10, 10, 10, 100});
        DrawText("GAME PAUSED", screen_textCenter("GAME PAUSED", 40, scrX), 100, 40, RAYWHITE);

        guiData->game_button1 = GuiButton((Rectangle){screen_elCenter(100, scrX), 200, 100, 40}, "Continue");
        guiData->game_button2 = GuiButton((Rectangle){screen_elCenter(100, scrX), 300, 100, 40}, "Back");
    }


}

/* Load/unload functions */

short _screen_load_title(screen_guiData_t * guiData) {

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    /* Resetting GUI data */
    guiData->title_button1 = 0;
    guiData->title_button2 = 0;
    guiData->title_button3 = 0;

    /* Returning true after loaded successfully */
    return 1;
}

short _screen_load_game(screen_guiData_t * guiData, game_data_t * gameData) {

    GuiSetStyle(DEFAULT, TEXT_SIZE, 15);

    /* Resetting GUI data */
    guiData->game_button1 = 0;
    guiData->game_button2 = 0;

    gameData->paused = 0;

    game_loadTex(gameData);

    /* Returning true after loaded successfully */
    return 1;
}

short _screen_unload_game(game_data_t * gameData) {
    game_unloadTex(gameData);
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