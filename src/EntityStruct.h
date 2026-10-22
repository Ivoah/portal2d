#pragma once

#include <box2d/box2d.h>
#include "Sprites.h"
#include "Vec.h"

typedef struct {
    SpriteId sprite;
    Vec2 pos;
    Vec2 velocity;
    Vec4 hitbox;
    b2BodyId bodyId;
} Entity;
