#pragma once
#include <SDL3/SDL.h>

#include "Scene.h"

typedef struct {
    int level;
    int menuItem;
    SDL_Texture* logo;
    SDL_Renderer* renderer;
} MenuSceneState;

void Menu_free(void*);
SDL_AppResult Menu_event(void*, SDL_Event*);
void Menu_draw(void*, SDL_Renderer*);

Scene Menu_scene(SDL_Renderer*, int, int);
