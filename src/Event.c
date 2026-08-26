#include "Event.h"

bool Event_init() {
    EVENT_LOAD_LEVEL = SDL_RegisterEvents(1);
    EVENT_LOAD_MENU = SDL_RegisterEvents(1);

    return EVENT_LOAD_LEVEL | EVENT_LOAD_MENU;
}
