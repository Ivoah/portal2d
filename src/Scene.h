#pragma once
#include <SDL3/SDL.h>

typedef struct Scene {
    void* state;
    void (*free)(struct Scene*);
    SDL_AppResult (*event)(struct Scene*, SDL_Event*);
    void (*draw)(struct Scene*, SDL_Renderer*);
} Scene;
