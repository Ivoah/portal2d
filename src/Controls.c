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

void Controls_free(void* sceneState) {
    SDL_free(sceneState);
}

SDL_AppResult Controls_event(void* sceneState, SDL_Event* event) {
    MenuSceneState* mss = (MenuSceneState*)sceneState;

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.scancode) {
                case SDL_SCANCODE_ESCAPE: {}
                    Scene* newScene = SDL_calloc(1, sizeof(Scene));
                    *newScene = (Scene){
                        .state = mss,
                        .freeState = Menu_free,
                        .event = Menu_event,
                        .draw = Menu_draw
                    };
                    SDL_PushEvent(&(SDL_Event){.user.type = EVENT_LOAD_SCENE, .user.code = E_KEEP_STATE, .user.data1 = newScene});
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

Scene Controls_scene(MenuSceneState* mss) {
    return (Scene){
        .state = mss,
        .freeState = Controls_free,
        .event = Controls_event,
        .draw = Controls_draw
    };
}
