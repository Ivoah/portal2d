#pragma once

#include <SDL3/SDL.h>

extern Uint32 EVENT_REPLACE_SCENE;
extern Uint32 EVENT_PUSH_SCENE;
extern Uint32 EVENT_POP_SCENE;

bool Event_init();
