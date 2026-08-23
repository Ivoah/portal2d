#pragma once

#include <SDL3/SDL.h>
#include "Vec2.h"

#define T_WOBBLE_SPEED 500
#define T_TEXT_HEIGHT 32

typedef enum {
    T_LEVEL = 0,
    T_MOVES,
    T_PLAY,
    T_NUM_TX
} Text_TxId;

bool Text_init(SDL_Renderer*);
void Text_deinit(); 
void Text_draw(SDL_Renderer*, Text_TxId, Vec2);
void Text_drawNumber(SDL_Renderer*, int, int, int, Vec2);
