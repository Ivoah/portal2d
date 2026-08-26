#pragma once

#include <SDL3/SDL.h>
#include "Entity.h"

#define L_MAX_WIDTH 30
#define L_MAX_HEIGHT 17
#define L_MAX_ENTITIES 32

typedef struct {
    int width;
    int height;
    int tiles[L_MAX_HEIGHT][L_MAX_WIDTH];
    Entity* entities[L_MAX_ENTITIES];
    Entity player;
} Level;

Level* Level_load(int);
void Level_free(Level*);
void Level_draw(SDL_Renderer*, SDL_Texture*, Level*);
