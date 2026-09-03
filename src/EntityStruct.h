#pragma once

#include "Sprites.h"
#include "Vec.h"

typedef struct {
    SpriteId sprite;
    Vec2 pos;
    Vec2 velocity;
    Vec4 hitbox;
} Entity;
