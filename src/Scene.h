#pragma once
#include <SDL3/SDL.h>

typedef struct Scene {
    void* state;
    struct Scene* previousScene;
    struct {
        Uint8 r;
        Uint8 g;
        Uint8 b;
    } clearColor;
    SDL_AppResult (*event)(struct Scene*, SDL_Event*);
    void (*update)(struct Scene*, int);
    void (*draw)(struct Scene*, SDL_Renderer*);
    void (*free)(struct Scene*);
} Scene;
