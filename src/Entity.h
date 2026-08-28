#pragma once

#include <SDL3/SDL.h>
#include "Vec.h"
#include "Sprite.h"

typedef struct {
    SpriteId sprite;
    Vec2 pos;
    Vec4 hitbox;
} Entity;

void Entity_update(Entity*, int);
void Entity_draw(SDL_Renderer*, SDL_Texture*, Entity*);
