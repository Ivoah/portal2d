#include "ControlsScene.h"
#include "Text.h"
#include "Save.h"
#include "Event.h"
#include "Input.h"
#include "main.h"

const char* messages[] = {
    "Keyboard controls:",
    "WSAD/arrow keys to move",
    "IKJL to shoot portals",
    "U to undo",
    "R to reset",
    "",
    "Gamepad controls:",
    "D-pad to move",
    "Face buttons to shoot portals",
    "Left bumper to undo",
    "Right bumper to reset"
};

SDL_AppResult ControlsScene_event(Scene* scene, SDL_Event* event) {

    switch (Input_fromEvent(event, true)) {
        case I_BACK: SDL_PushEvent(&(SDL_Event){.type = EVENT_POP_SCENE}); break;
        default: break;
    }

    return SDL_APP_CONTINUE;
}

void ControlsScene_draw(Scene* scene, SDL_Renderer* renderer) {
    int numMsgs = sizeof(messages)/sizeof(char*);

    for (int i = 0; i < numMsgs; i++) {
        Text_drawCentered(renderer, messages[i], WINDOW_HEIGHT/2 - (numMsgs*T_SIZE)/2 + i*T_SIZE);
    }
}

void ControlsScene_free(Scene* scene) {
    SDL_free(scene);
}

Scene* ControlsScene_create() {
    Scene* scene = SDL_calloc(1, sizeof(Scene));
    *scene = (Scene){
        .state = NULL,
        .event = ControlsScene_event,
        .draw = ControlsScene_draw,
        .free = ControlsScene_free
    };

    return scene;
}
