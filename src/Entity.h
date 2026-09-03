#pragma once

#include <SDL3/SDL.h>
#include "Vec.h"
#include "Sprites.h"
#include "EntityStruct.h"
#include "Level.h"

void Entity_update(Entity*, Level*, int);
void Entity_draw(SDL_Renderer*, Entity*);
Entity Entity_create(SpriteId, Vec2);
