#pragma once

#include <SDL3/SDL.h>
#include "Vec.h"

#define S_SIZE 32

typedef enum {
    S_UNKNOWN = -1,
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

extern SDL_Texture* Sprites_texture;

bool Sprites_init(SDL_Renderer*, const char*);
Vec4 Sprites_hitbox(SpriteId);
void Sprites_draw(SDL_Renderer*, SpriteId, const Vec2*);
void Sprites_drawPortal(SDL_Renderer*, bool, const Vec2*, double);
