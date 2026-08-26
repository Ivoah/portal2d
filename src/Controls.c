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
    Scene* previousScene;
} ControlsSceneState;

void ControlsScene_free(Scene* scene) {
    SDL_free(scene->state);
    SDL_free(scene);
}

SDL_AppResult ControlsScene_event(Scene* scene, SDL_Event* event) {
    ControlsSceneState* css = (ControlsSceneState*)scene->state;

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.scancode) {
                case SDL_SCANCODE_ESCAPE: {}
                    SDL_PushEvent(&(SDL_Event){.user.type = EVENT_LOAD_SCENE, .user.data1 = css->previousScene});
                    break;
                default: break;
            }
    }

    return SDL_APP_CONTINUE;
}

void ControlsScene_draw(Scene* scene, SDL_Renderer* renderer) {
    int numMsgs = sizeof(messages)/sizeof(char*);

    for (int i = 0; i < numMsgs; i++) {
        Text_drawCentered(renderer, messages[i], WINDOW_HEIGHT/2 - (numMsgs*T_SIZE)/2 + i*T_SIZE);
    }
}

Scene* Controls_scene(Scene* previousScene) {
    Scene* scene = SDL_calloc(1, sizeof(Scene));
    *scene = (Scene){
        .state = SDL_calloc(1, sizeof(ControlsSceneState)),
        .free = ControlsScene_free,
        .event = ControlsScene_event,
        .draw = ControlsScene_draw
    };

    *((ControlsSceneState*)scene->state) = (ControlsSceneState){
        .previousScene = previousScene
    };

    return scene;
}
