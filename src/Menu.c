#include "Menu.h"
#include "Text.h"
#include "Scene.h"
#include "Save.h"
#include "Util.h"
#include "Event.h"
#include "Level.h"
#include "Controls.h"
#include "main.h"

typedef struct {
    int level;
    int menuItem;
    SDL_Texture* logo;
    SDL_Renderer* renderer;
} MenuSceneState;

void Menu_free(void*);
SDL_AppResult Menu_event(void*, SDL_Event*);
void Menu_draw(void*, SDL_Renderer*);

void Menu_free(void* sceneState) {
    MenuSceneState* mss = (MenuSceneState*)sceneState;
    
    SDL_DestroyTexture(mss->logo);
    SDL_free(sceneState);
}

SDL_AppResult Menu_event(void* sceneState, SDL_Event* event) {
    MenuSceneState* mss = (MenuSceneState*)sceneState;
    Vec2* dir = &(Vec2){0, 0};
    Scene* newScene;

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.scancode) {
                case SDL_SCANCODE_ESCAPE: return SDL_APP_SUCCESS;
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_W:      dir = &V_UP; break;
                case SDL_SCANCODE_DOWN:
                case SDL_SCANCODE_S:      dir = &V_DOWN; break;
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_A:      dir = &V_LEFT; break;
                case SDL_SCANCODE_RIGHT:
                case SDL_SCANCODE_D:      dir = &V_RIGHT; break;
                case SDL_SCANCODE_RETURN:
                    switch (mss->menuItem) {
                        case 1: {}
                            newScene = SDL_calloc(1, sizeof(Scene));
                            *newScene = Level_scene(mss->renderer, mss->level);
                            SDL_PushEvent(&(SDL_Event){.user.type = EVENT_LOAD_SCENE, .user.data1 = newScene});
                            break;
                        case 2: {}
                            newScene = SDL_calloc(1, sizeof(Scene));
                            *newScene = Controls_scene(mss->renderer, mss->level, mss->menuItem);
                            SDL_PushEvent(&(SDL_Event){.user.type = EVENT_LOAD_SCENE, .user.data1 = newScene});
                            break;
                        case 3: return SDL_APP_SUCCESS;
                    }
                default: break;
            }
    }

    mss->menuItem += dir->y;
    Util_wrap(&mss->menuItem, 4);

    if (mss->menuItem == 0) {
        mss->level += dir->x;
        Util_wrap(&mss->level, 10);
    }

    return SDL_APP_CONTINUE;
}

void Menu_draw(void* sceneState, SDL_Renderer* renderer) {
    MenuSceneState* mss = (MenuSceneState*)sceneState;

    char fmtStr[100];

    Util_drawWobbly(renderer, mss->logo, (Vec2){WINDOW_WIDTH/2 - mss->logo->w/2, 100});

    SDL_snprintf(fmtStr, sizeof(fmtStr)/sizeof(char), mss->menuItem == 0 ? "<Level %d>" : "Level %d", mss->level + 1);
    Text_drawCentered(renderer, fmtStr, 200);
    Text_drawCentered(renderer, mss->menuItem == 1 ? "<Play>" : "Play", 250);
    Text_drawCentered(renderer, mss->menuItem == 2 ? "<Controls?>" : "Controls?", 300);
    Text_drawCentered(renderer, mss->menuItem == 3 ? "<Quit>" : "Quit", 350);
}

Scene Menu_scene(SDL_Renderer* renderer, int level, int menuItem) {
    MenuSceneState* mss = SDL_calloc(1, sizeof(MenuSceneState));

    mss->renderer = renderer;
    mss->level = level;
    mss->menuItem = menuItem;
    mss->logo = Util_loadTexture(renderer, "logo.png");

    return (Scene){
        .state = mss,
        .freeState = Menu_free,
        .event = Menu_event,
        .draw = Menu_draw
    };
}
