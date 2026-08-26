#pragma once
#include <SDL3/SDL.h>

typedef struct Scene {
    void* state;
    SDL_AppResult (*event)(struct Scene*, SDL_Event*);
    void (*draw)(struct Scene*, SDL_Renderer*);
    void (*free)(struct Scene*);
} Scene;
