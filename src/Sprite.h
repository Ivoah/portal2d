#pragma once

#include <SDL3/SDL.h>
#include "Vec2.h"

#define S_SIZE 32

typedef enum {
    S_AIR,
    S_BUTTON,
    S_CUBE,
    S_BENDY,
    S_FLOOR,
    S_PWALL,
    S_NPWALL,
    S_ORANGEPORTAL1,
    S_ORANGEPORTAL2,
    S_BLUEPORTAL1,
    S_BLUEPORTAL2,
    S_NUM_TX
} SpriteId;

void Sprite_draw(SDL_Renderer*, SDL_Texture*, SpriteId, const Vec2*);
void Sprite_drawPortal(SDL_Renderer*, SDL_Texture*, bool, const Vec2*, double);
