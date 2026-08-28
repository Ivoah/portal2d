#pragma once

#include <SDL3/SDL.h>
#include "Vec.h"

#define T_SIZE 32
#define T_SPACING 8

bool Text_init(SDL_Renderer*);
void Text_deinit(); 
void Text_draw(SDL_Renderer*, const char*, Vec2);
void Text_drawCentered(SDL_Renderer*, const char*, int);
int Text_getWidth(const char*);
