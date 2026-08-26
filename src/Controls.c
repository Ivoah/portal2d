#include "Menu.h"
#include "Text.h"
#include "Scene.h"
#include "Save.h"
#include "Util.h"
#include "Event.h"
#include "Level.h"
#include "main.h"

const char* messages[] = {
    "Keyboard controls:",
    "WSAD/arrow keys to move",
    "IKJL to shoot portals",
    "R to reset",
    "U to undo"
};

typedef struct {
    SDL_Renderer* renderer;
    int level;
    int menuItem;
} ControlsSceneState;

void Controls_free(void* sceneState) {
    ControlsSceneState* css = (ControlsSceneState*)sceneState;
    SDL_free(sceneState);
}

SDL_AppResult Controls_event(void* sceneState, SDL_Event* event) {
    ControlsSceneState* css = (ControlsSceneState*)sceneState;

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.scancode) {
                case SDL_SCANCODE_ESCAPE: {}
                    Scene* scene = SDL_calloc(1, sizeof(Scene));
                    *scene = Menu_scene(css->renderer, css->level, css->menuItem);
                    SDL_PushEvent(&(SDL_Event){.user.type = EVENT_LOAD_SCENE, .user.data1 = scene});
                    break;
                default: break;
            }
    }

    return SDL_APP_CONTINUE;
}

void Controls_draw(void* sceneState, SDL_Renderer* renderer) {
    int numMsgs = sizeof(messages)/sizeof(char*);

    for (int i = 0; i < numMsgs; i++) {
        Text_drawCentered(renderer, messages[i], WINDOW_HEIGHT/2 - (numMsgs*T_SIZE)/2 + i*T_SIZE);
    }
}

Scene Controls_scene(SDL_Renderer* renderer, int level, int menuItem) {
    ControlsSceneState* css = SDL_calloc(1, sizeof(ControlsSceneState));

    css->renderer = renderer;
    css->level = level;
    css->menuItem = menuItem;

    return (Scene){
        .state = css,
        .freeState = Controls_free,
        .event = Controls_event,
        .draw = Controls_draw
    };
}
