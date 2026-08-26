#pragma once

#include <SDL3/SDL.h>

#include "Vec2.h"

#define L_MAX_WIDTH 32
#define L_MAX_HEIGHT 32
#define L_MAX_CUBES 32

typedef struct {
    bool exists;
    Vec2 pos;
    int r;
} Portal;

typedef struct LevelState {
    int moves;
    Vec2 playerLocation;
    Vec2 cubes[L_MAX_CUBES];
    Portal orangePortal;
    Portal bluePortal;
    bool lastShotBlue;
    struct LevelState* lastState;
} LevelState;

typedef struct {
    int levelNum;
    int tiles[L_MAX_HEIGHT][L_MAX_WIDTH];
    int width;
    int height;
    int numCubes;
    LevelState* state;
} Level;

SDL_Texture** Level_loadTextures(SDL_Renderer*);
void Level_freeTextures(SDL_Texture**);
Level* Level_load(int);
void Level_free(Level*);
void Level_draw(SDL_Renderer*, Level*, SDL_Texture**);
void Level_move(Level*, Vec2);
void Level_shoot(Level*, Vec2);
void Level_undo(Level*, int);
bool Level_isWon(Level*);
