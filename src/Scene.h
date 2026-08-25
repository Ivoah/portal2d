#pragma once
#include <SDL3/SDL.h>

typedef struct {
    void* state;
    void (*freeState)(void*);
    SDL_AppResult (*event)(void*, SDL_Event*);
    void (*draw)(void*, SDL_Renderer*);
} Scene;
