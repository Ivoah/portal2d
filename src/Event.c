#include "Event.h"

bool Event_init() {
    EVENT_LOAD_SCENE = SDL_RegisterEvents(1);

    return EVENT_LOAD_SCENE;
}
