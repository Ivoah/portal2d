#include "Menu.h"
#include "Text.h"
#include "Scene.h"

typedef struct {

} MenuSceneState;

void Menu_free(void* sceneState) {
    SDL_free(sceneState);
}

SDL_AppResult Menu_event(void* sceneState, SDL_Event* event) {
    return SDL_APP_CONTINUE;
}

void Menu_draw(void* sceneState, SDL_Renderer* renderer) {
    Text_draw(renderer, T_PORTALBAN, (Vec2){0, 0});
}

Scene Menu_scene() {
    return (Scene){
        .state = SDL_calloc(1, sizeof(void)),
        .freeState = &Menu_free,
        .event = &Menu_event,
        .draw = &Menu_draw
    };
}
