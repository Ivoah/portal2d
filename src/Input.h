#pragma once

#include <SDL3/SDL.h>

typedef enum {
    I_UNKNOWN,
    I_UP,
    I_DOWN,
    I_LEFT,
    I_RIGHT,
    I_SHOOT_UP,
    I_SHOOT_DOWN,
    I_SHOOT_LEFT,
    I_SHOOT_RIGHT,
    I_CONFIRM,
    I_BACK
} InputButton;

InputButton Input_fromEvent(SDL_Event*, bool);
