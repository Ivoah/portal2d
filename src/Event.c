#include "Event.h"

Uint32 EVENT_REPLACE_SCENE;
Uint32 EVENT_PUSH_SCENE;
Uint32 EVENT_POP_SCENE;

bool Event_init() {
    EVENT_REPLACE_SCENE = SDL_RegisterEvents(1);
    EVENT_PUSH_SCENE = SDL_RegisterEvents(1);
    EVENT_POP_SCENE = SDL_RegisterEvents(1);

    return EVENT_REPLACE_SCENE | EVENT_PUSH_SCENE | EVENT_POP_SCENE;
}
