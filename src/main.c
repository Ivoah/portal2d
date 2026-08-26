#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "main.h"

#include "Scene.h"
#include "Level.h"
#include "Menu.h"
#include "Event.h"
#include "Text.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Scene scene;
    SDL_Gamepad* gamepad;
} GameState;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    *appstate = SDL_calloc(1, sizeof(GameState));
    GameState* gamestate = (GameState*)(*appstate);
    SDL_zero(*gamestate);

    SDL_SetAppMetadata("Portalban", "0.1", "net.ivoah.portalban");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Portalban", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &gamestate->window, &gamestate->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(gamestate->renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    if (!SDL_SetRenderVSync(gamestate->renderer, 1)) SDL_Log("Could not set vsync: %s", SDL_GetError());

    if (!Text_init(gamestate->renderer)) return SDL_APP_FAILURE;
    if (!Event_init()) return SDL_APP_FAILURE;

    gamestate->scene = Menu_scene(gamestate->renderer, 0, 0);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    GameState* gamestate = (GameState*)appstate;

    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_GAMEPAD_ADDED:
            if (gamestate->gamepad == NULL) gamestate->gamepad = SDL_OpenGamepad(event->gbutton.which);
            return SDL_APP_CONTINUE;
        default:
            if (event->type == EVENT_LOAD_SCENE) {
                gamestate->scene.freeState(gamestate->scene.state);
                gamestate->scene = *(Scene*)event->user.data1;
                SDL_free(event->user.data1);
                return SDL_APP_CONTINUE;
             } else return gamestate->scene.event(gamestate->scene.state, event);
    }
}

Uint64 lastTime = 0, currentTime = 0;
SDL_AppResult SDL_AppIterate(void* appstate) {
    GameState* gamestate = (GameState*)appstate;

    currentTime = SDL_GetTicks();
    Uint64 delta = currentTime - lastTime;
    lastTime = currentTime;

    // Clear screen
    SDL_SetRenderDrawColor(gamestate->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(gamestate->renderer);

    gamestate->scene.draw(gamestate->scene.state, gamestate->renderer);

    SDL_RenderPresent(gamestate->renderer);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    GameState* gamestate = (GameState*)appstate;

    if (gamestate->gamepad != NULL) SDL_CloseGamepad(gamestate->gamepad);
    
    if (gamestate->scene.state != NULL) gamestate->scene.freeState(gamestate->scene.state);
    SDL_free(gamestate);
    
    Text_deinit();
    /* SDL will clean up the window/renderer for us. */
}
