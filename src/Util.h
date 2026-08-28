#pragma once

#include <SDL3/SDL.h>
#include "Vec.h"

#define U_WOBBLE_SPEED 500

SDL_Surface* Util_loadSurface(const char*);
SDL_Texture* Util_loadTexture(SDL_Renderer*, const char*);
void Util_drawWobbly(SDL_Renderer*, SDL_Texture*, Vec2);
void Util_wrap(int*, int);
